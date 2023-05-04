// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/extras/sqlite/sqlite_persistent_shared_dictionary_store.h"

#include <tuple>

#include "base/files/file_util.h"
#include "base/files/scoped_temp_dir.h"
#include "base/logging.h"
#include "base/memory/scoped_refptr.h"
#include "base/task/sequenced_task_runner.h"
#include "base/task/thread_pool.h"
#include "base/test/bind.h"
#include "base/test/test_file_util.h"
#include "build/build_config.h"
#include "net/base/schemeful_site.h"
#include "net/extras/shared_dictionary/shared_dictionary_info.h"
#include "net/extras/shared_dictionary/shared_dictionary_storage_isolation_key.h"
#include "net/test/test_with_task_environment.h"
#include "sql/database.h"
#include "sql/meta_table.h"
#include "sql/test/test_helpers.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

using ::testing::ElementsAre;
using ::testing::ElementsAreArray;
using ::testing::Pair;
using ::testing::UnorderedElementsAreArray;

namespace net {

namespace {

const int kCurrentVersionNumber = 1;

const base::FilePath::CharType kSharedDictionaryStoreFilename[] =
    FILE_PATH_LITERAL("SharedDictionary");
}  // namespace

SharedDictionaryStorageIsolationKey CreateIsolationKey(
    const std::string& frame_origin_str,
    const absl::optional<std::string>& top_frame_site_str = absl::nullopt) {
  return SharedDictionaryStorageIsolationKey(
      url::Origin::Create(GURL(frame_origin_str)),
      top_frame_site_str ? net::SchemefulSite(GURL(*top_frame_site_str))
                         : net::SchemefulSite(GURL(frame_origin_str)));
}

class SQLitePersistentSharedDictionaryStoreTest
    : public TestWithTaskEnvironment {
 public:
  SQLitePersistentSharedDictionaryStoreTest()
      : isolation_key_(CreateIsolationKey("https://origin.test/")),
        dictionary_info_(
            GURL("https://origin.test/dict"),
            /*response_time=*/base::Time::Now() - base::Seconds(10),
            /*expiration*/ base::Seconds(100),
            "/pattern*",
            /*last_used_time*/ base::Time::Now(),
            /*size=*/1000,
            net::SHA256HashValue({{0x00, 0x01}}),
            /*disk_cache_key_token=*/base::UnguessableToken::Create(),
            /*primary_key_in_database=*/absl::nullopt) {}

  SQLitePersistentSharedDictionaryStoreTest(
      const SQLitePersistentSharedDictionaryStoreTest&) = delete;
  SQLitePersistentSharedDictionaryStoreTest& operator=(
      const SQLitePersistentSharedDictionaryStoreTest&) = delete;

  void SetUp() override { ASSERT_TRUE(temp_dir_.CreateUniqueTempDir()); }

  void TearDown() override { DestroyStore(); }

 protected:
  base::FilePath GetStroeFilePath() const {
    return temp_dir_.GetPath().Append(kSharedDictionaryStoreFilename);
  }

  void CreateStore() {
    CHECK(!store_);
    store_ = std::make_unique<SQLitePersistentSharedDictionaryStore>(
        GetStroeFilePath(), client_task_runner_, background_task_runner_);
  }

  void DestroyStore() {
    store_.reset();
    // Make sure we wait until the destructor has run by running all
    // TaskEnvironment tasks.
    RunUntilIdle();
  }

  int64_t RegisterDictionary(
      const SharedDictionaryStorageIsolationKey& isolation_key,
      SharedDictionaryInfo dictionary_info) {
    int64_t result_primary_key_in_database = 0;
    base::RunLoop run_loop;
    store_->RegisterDictionary(
        isolation_key, std::move(dictionary_info),
        base::BindLambdaForTesting(
            [&](SQLitePersistentSharedDictionaryStore::Error error,
                absl::optional<int64_t> primary_key_in_database) {
              EXPECT_EQ(SQLitePersistentSharedDictionaryStore::Error::kOk,
                        error);
              ASSERT_TRUE(primary_key_in_database);
              result_primary_key_in_database = *primary_key_in_database;
              run_loop.Quit();
            }));
    run_loop.Run();
    return result_primary_key_in_database;
  }

  std::vector<SharedDictionaryInfo> GetDictionaries(
      const SharedDictionaryStorageIsolationKey& isolation_key) {
    std::vector<SharedDictionaryInfo> result_dictionaries;
    base::RunLoop run_loop;
    store_->GetDictionaries(
        isolation_key,
        base::BindLambdaForTesting(
            [&](SQLitePersistentSharedDictionaryStore::Error error,
                std::vector<SharedDictionaryInfo> dictionaries) {
              EXPECT_EQ(SQLitePersistentSharedDictionaryStore::Error::kOk,
                        error);
              result_dictionaries = std::move(dictionaries);
              run_loop.Quit();
            }));
    run_loop.Run();
    return result_dictionaries;
  }

  std::map<SharedDictionaryStorageIsolationKey,
           std::vector<SharedDictionaryInfo>>
  GetAllDictionaries() {
    std::map<SharedDictionaryStorageIsolationKey,
             std::vector<SharedDictionaryInfo>>
        result_all_dictionaries;
    base::RunLoop run_loop;
    store_->GetAllDictionaries(base::BindLambdaForTesting(
        [&](SQLitePersistentSharedDictionaryStore::Error error,
            std::map<SharedDictionaryStorageIsolationKey,
                     std::vector<SharedDictionaryInfo>> all_dictionaries) {
          EXPECT_EQ(SQLitePersistentSharedDictionaryStore::Error::kOk, error);
          result_all_dictionaries = std::move(all_dictionaries);
          run_loop.Quit();
        }));
    run_loop.Run();
    return result_all_dictionaries;
  }

  void ClearAllDictionaries() {
    base::RunLoop run_loop;
    store_->ClearAllDictionaries(base::BindLambdaForTesting(
        [&](SQLitePersistentSharedDictionaryStore::Error error) {
          EXPECT_EQ(SQLitePersistentSharedDictionaryStore::Error::kOk, error);
          run_loop.Quit();
        }));
    run_loop.Run();
  }

  void CorruptDatabaseFile() {
    // Execute CreateStore(), ClearAllDictionaries() and DestroyStore() to
    // create a database file.
    CreateStore();
    ClearAllDictionaries();
    DestroyStore();

    // Corrupt the database.
    CHECK(sql::test::CorruptSizeInHeader(GetStroeFilePath()));
  }

  void ManipulateDatabase(
      const std::vector<std::string>& create_table_queries) {
    // We don't allow manipulating the database while `store_` exists.
    ASSERT_FALSE(store_);

    std::unique_ptr<sql::Database> db =
        std::make_unique<sql::Database>(sql::DatabaseOptions{});
    ASSERT_TRUE(db->Open(GetStroeFilePath()));

    sql::MetaTable meta_table;
    ASSERT_TRUE(meta_table.Init(db.get(), kCurrentVersionNumber,
                                kCurrentVersionNumber));
    for (const std::string& query : create_table_queries) {
      ASSERT_TRUE(db->Execute(query.c_str()));
    }
    db->Close();
  }

  void MakeFileUnwritable() {
    file_permissions_restorer_ =
        std::make_unique<base::FilePermissionRestorer>(GetStroeFilePath());
    ASSERT_TRUE(base::MakeFileUnwritable(GetStroeFilePath()));
  }

  void CheckStoreRecovered() {
    CreateStore();
    EXPECT_TRUE(GetDictionaries(isolation_key_).empty());
    EXPECT_TRUE(GetAllDictionaries().empty());
    DestroyStore();
  }

  void RunMultipleDictionariesTest(
      const SharedDictionaryStorageIsolationKey isolation_key1,
      const SharedDictionaryInfo dictionary_info1,
      const SharedDictionaryStorageIsolationKey isolation_key2,
      const SharedDictionaryInfo dictionary_info2,
      bool expect_merged);

  void RunRegisterDictionaryFailureTest(
      SQLitePersistentSharedDictionaryStore::Error expected_error);
  void RunGetDictionariesFailureTest(
      SQLitePersistentSharedDictionaryStore::Error expected_error);
  void RunGetAllDictionariesFailureTest(
      SQLitePersistentSharedDictionaryStore::Error expected_error);
  void RunClearAllDictionariesFailureTest(
      SQLitePersistentSharedDictionaryStore::Error expected_error);

  base::ScopedTempDir temp_dir_;
  std::unique_ptr<SQLitePersistentSharedDictionaryStore> store_;
  const scoped_refptr<base::SequencedTaskRunner> client_task_runner_ =
      base::SingleThreadTaskRunner::GetCurrentDefault();
  const scoped_refptr<base::SequencedTaskRunner> background_task_runner_ =
      base::ThreadPool::CreateSequencedTaskRunner({base::MayBlock()});
  // `file_permissions_restorer_` must be below `temp_dir_` to restore the
  // file permission correctly.
  std::unique_ptr<base::FilePermissionRestorer> file_permissions_restorer_;

  const SharedDictionaryStorageIsolationKey isolation_key_;
  const SharedDictionaryInfo dictionary_info_;
};

TEST_F(SQLitePersistentSharedDictionaryStoreTest, SingleDictionary) {
  CreateStore();

  int64_t primary_key_in_database =
      RegisterDictionary(isolation_key_, dictionary_info_);
  SharedDictionaryInfo expected_info = dictionary_info_;
  expected_info.set_primary_key_in_database(primary_key_in_database);

  EXPECT_THAT(GetDictionaries(isolation_key_),
              ElementsAreArray({expected_info}));
  EXPECT_THAT(
      GetAllDictionaries(),
      ElementsAre(Pair(isolation_key_, ElementsAreArray({expected_info}))));

  ClearAllDictionaries();
  EXPECT_TRUE(GetDictionaries(isolation_key_).empty());
  EXPECT_TRUE(GetAllDictionaries().empty());
}

void SQLitePersistentSharedDictionaryStoreTest::RunMultipleDictionariesTest(
    const SharedDictionaryStorageIsolationKey isolation_key1,
    const SharedDictionaryInfo dictionary_info1,
    const SharedDictionaryStorageIsolationKey isolation_key2,
    const SharedDictionaryInfo dictionary_info2,
    bool expect_merged) {
  CreateStore();

  int64_t primary_key_in_database1 =
      RegisterDictionary(isolation_key1, dictionary_info1);
  int64_t primary_key_in_database2 =
      RegisterDictionary(isolation_key2, dictionary_info2);

  EXPECT_NE(primary_key_in_database1, primary_key_in_database2);

  SharedDictionaryInfo expected_info1 = dictionary_info1;
  SharedDictionaryInfo expected_info2 = dictionary_info2;
  expected_info1.set_primary_key_in_database(primary_key_in_database1);
  expected_info2.set_primary_key_in_database(primary_key_in_database2);

  if (isolation_key1 == isolation_key2) {
    if (expect_merged) {
      EXPECT_THAT(GetDictionaries(isolation_key1),
                  ElementsAreArray({expected_info2}));
      EXPECT_THAT(GetAllDictionaries(),
                  ElementsAre(Pair(isolation_key1,
                                   ElementsAreArray({expected_info2}))));

    } else {
      EXPECT_THAT(GetDictionaries(isolation_key1),
                  UnorderedElementsAreArray({expected_info1, expected_info2}));
      EXPECT_THAT(GetAllDictionaries(),
                  ElementsAre(Pair(isolation_key1,
                                   UnorderedElementsAreArray(
                                       {expected_info1, expected_info2}))));
    }
  } else {
    EXPECT_THAT(GetDictionaries(isolation_key1),
                ElementsAreArray({expected_info1}));
    EXPECT_THAT(GetDictionaries(isolation_key2),
                ElementsAreArray({expected_info2}));
    EXPECT_THAT(
        GetAllDictionaries(),
        ElementsAre(Pair(isolation_key1, ElementsAreArray({expected_info1})),
                    Pair(isolation_key2, ElementsAreArray({expected_info2}))));
  }

  ClearAllDictionaries();
  EXPECT_TRUE(GetDictionaries(isolation_key_).empty());
  EXPECT_TRUE(GetAllDictionaries().empty());
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       MultipleDictionariesDifferentOriginSameSite) {
  SharedDictionaryStorageIsolationKey isolation_key1 =
      CreateIsolationKey("https://www1.origin.test/");
  SharedDictionaryStorageIsolationKey isolation_key2 =
      CreateIsolationKey("https://www2.origin.test/");
  EXPECT_NE(isolation_key1, isolation_key2);
  EXPECT_NE(isolation_key1.frame_origin(), isolation_key2.frame_origin());
  EXPECT_EQ(isolation_key1.top_frame_site(), isolation_key2.top_frame_site());
  RunMultipleDictionariesTest(isolation_key1, dictionary_info_, isolation_key2,
                              dictionary_info_, /*expect_merged=*/false);
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       MultipleDictionariesDifferentSite) {
  SharedDictionaryStorageIsolationKey isolation_key1 =
      CreateIsolationKey("https://origin1.test/");
  SharedDictionaryStorageIsolationKey isolation_key2 =
      CreateIsolationKey("https://origin2.test/");
  EXPECT_NE(isolation_key1, isolation_key2);
  EXPECT_NE(isolation_key1.frame_origin(), isolation_key2.frame_origin());
  EXPECT_NE(isolation_key1.top_frame_site(), isolation_key2.top_frame_site());
  RunMultipleDictionariesTest(isolation_key1, dictionary_info_, isolation_key2,
                              dictionary_info_, /*expect_merged=*/false);
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       MultipleDictionariesDifferentHostAndPathPattern) {
  RunMultipleDictionariesTest(
      isolation_key_,
      SharedDictionaryInfo(
          GURL("https://origin1.test/dict"),
          /*response_time=*/base::Time::Now() - base::Seconds(10),
          /*expiration*/ base::Seconds(100), "/pattern1*",
          /*last_used_time*/ base::Time::Now(),
          /*size=*/1000, net::SHA256HashValue({{0x00, 0x01}}),
          /*disk_cache_key_token=*/base::UnguessableToken::Create(),
          /*primary_key_in_database=*/absl::nullopt),
      isolation_key_,
      SharedDictionaryInfo(
          GURL("https://origin2.test/dict"),
          /*response_time=*/base::Time::Now() - base::Seconds(20),
          /*expiration*/ base::Seconds(200), "/pattern2*",
          /*last_used_time*/ base::Time::Now(),
          /*size=*/2000, net::SHA256HashValue({{0x00, 0x02}}),
          /*disk_cache_key_token=*/base::UnguessableToken::Create(),
          /*primary_key_in_database=*/absl::nullopt),
      /*expect_merged=*/false);
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       SameIsolationKeySameHostDifferentPathPattern) {
  RunMultipleDictionariesTest(
      isolation_key_,
      SharedDictionaryInfo(
          GURL("https://origin.test/dict"),
          /*response_time=*/base::Time::Now() - base::Seconds(10),
          /*expiration*/ base::Seconds(100), "/pattern1*",
          /*last_used_time*/ base::Time::Now(),
          /*size=*/1000, net::SHA256HashValue({{0x00, 0x01}}),
          /*disk_cache_key_token=*/base::UnguessableToken::Create(),
          /*primary_key_in_database=*/absl::nullopt),
      isolation_key_,
      SharedDictionaryInfo(
          GURL("https://origin.test/dict"),
          /*response_time=*/base::Time::Now() - base::Seconds(20),
          /*expiration*/ base::Seconds(200), "/pattern2*",
          /*last_used_time*/ base::Time::Now(),
          /*size=*/2000, net::SHA256HashValue({{0x00, 0x02}}),
          /*disk_cache_key_token=*/base::UnguessableToken::Create(),
          /*primary_key_in_database=*/absl::nullopt),
      /*expect_merged=*/false);
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       SameIsolationKeySameHostSamePathPattern) {
  RunMultipleDictionariesTest(
      isolation_key_,
      SharedDictionaryInfo(
          GURL("https://origin.test/dict"),
          /*response_time=*/base::Time::Now() - base::Seconds(10),
          /*expiration*/ base::Seconds(100), "/pattern*",
          /*last_used_time*/ base::Time::Now(),
          /*size=*/1000, net::SHA256HashValue({{0x00, 0x01}}),
          /*disk_cache_key_token=*/base::UnguessableToken::Create(),
          /*primary_key_in_database=*/absl::nullopt),
      isolation_key_,
      SharedDictionaryInfo(
          GURL("https://origin.test/dict"),
          /*response_time=*/base::Time::Now() - base::Seconds(20),
          /*expiration*/ base::Seconds(200), "/pattern*",
          /*last_used_time*/ base::Time::Now(),
          /*size=*/2000, net::SHA256HashValue({{0x00, 0x02}}),
          /*disk_cache_key_token=*/base::UnguessableToken::Create(),
          /*primary_key_in_database=*/absl::nullopt),
      /*expect_merged=*/true);
}

void SQLitePersistentSharedDictionaryStoreTest::
    RunRegisterDictionaryFailureTest(
        SQLitePersistentSharedDictionaryStore::Error expected_error) {
  CreateStore();
  base::RunLoop run_loop;
  store_->RegisterDictionary(
      isolation_key_, dictionary_info_,
      base::BindLambdaForTesting(
          [&](SQLitePersistentSharedDictionaryStore::Error error,
              absl::optional<int64_t> primary_key_in_database) {
            EXPECT_EQ(expected_error, error);
            EXPECT_FALSE(primary_key_in_database);
            run_loop.Quit();
          }));
  run_loop.Run();
  DestroyStore();
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       RegisterDictionaryErrorDatabaseInitializationFailure) {
  CorruptDatabaseFile();
  RunRegisterDictionaryFailureTest(SQLitePersistentSharedDictionaryStore::
                                       Error::kFailedToInitializeDatabase);
  CheckStoreRecovered();
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       RegisterDictionaryErrorInvalidSql) {
  ManipulateDatabase({"CREATE TABLE dictionaries (dummy TEST NOT NULL)"});
  RunRegisterDictionaryFailureTest(
      SQLitePersistentSharedDictionaryStore::Error::kInvalidSql);
}

#if !BUILDFLAG(IS_FUCHSIA) && !BUILDFLAG(IS_WIN)
// MakeFileUnwritable() doesn't cause the failure on Fuchsia and Windows. So
// disabling the test on Fuchsia and Windows.
TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       RegisterDictionaryErrorSqlExecutionFailure) {
  CreateStore();
  ClearAllDictionaries();
  DestroyStore();
  MakeFileUnwritable();
  RunRegisterDictionaryFailureTest(
      SQLitePersistentSharedDictionaryStore::Error::kFailedToExecuteSql);
}
#endif  // !BUILDFLAG(IS_FUCHSIA)

void SQLitePersistentSharedDictionaryStoreTest::RunGetDictionariesFailureTest(
    SQLitePersistentSharedDictionaryStore::Error expected_error) {
  CreateStore();
  base::RunLoop run_loop;
  store_->GetDictionaries(
      isolation_key_,
      base::BindLambdaForTesting(
          [&](SQLitePersistentSharedDictionaryStore::Error error,
              std::vector<SharedDictionaryInfo> dictionaries) {
            EXPECT_EQ(expected_error, error);
            EXPECT_TRUE(dictionaries.empty());
            run_loop.Quit();
          }));
  run_loop.Run();
  DestroyStore();
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       GetDictionariesErrorDatabaseInitializationFailure) {
  CorruptDatabaseFile();
  RunGetDictionariesFailureTest(SQLitePersistentSharedDictionaryStore::Error::
                                    kFailedToInitializeDatabase);
  CheckStoreRecovered();
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       GetDictionariesErrorInvalidSql) {
  ManipulateDatabase({"CREATE TABLE dictionaries (dummy TEST NOT NULL)"});
  RunGetDictionariesFailureTest(
      SQLitePersistentSharedDictionaryStore::Error::kInvalidSql);
}

void SQLitePersistentSharedDictionaryStoreTest::
    RunGetAllDictionariesFailureTest(
        SQLitePersistentSharedDictionaryStore::Error expected_error) {
  CreateStore();
  base::RunLoop run_loop;
  store_->GetAllDictionaries(base::BindLambdaForTesting(
      [&](SQLitePersistentSharedDictionaryStore::Error error,
          std::map<SharedDictionaryStorageIsolationKey,
                   std::vector<SharedDictionaryInfo>> all_dictionaries) {
        EXPECT_EQ(expected_error, error);
        EXPECT_TRUE(all_dictionaries.empty());
        run_loop.Quit();
      }));
  run_loop.Run();
  DestroyStore();
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       GetAllDictionariesErrorDatabaseInitializationFailure) {
  CorruptDatabaseFile();
  RunGetAllDictionariesFailureTest(SQLitePersistentSharedDictionaryStore::
                                       Error::kFailedToInitializeDatabase);
  CheckStoreRecovered();
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       GetAllDictionariesErrorInvalidSql) {
  ManipulateDatabase({"CREATE TABLE dictionaries (dummy TEST NOT NULL)"});
  RunGetAllDictionariesFailureTest(
      SQLitePersistentSharedDictionaryStore::Error::kInvalidSql);
}

void SQLitePersistentSharedDictionaryStoreTest::
    RunClearAllDictionariesFailureTest(
        SQLitePersistentSharedDictionaryStore::Error expected_error) {
  CreateStore();
  base::RunLoop run_loop;
  store_->ClearAllDictionaries(base::BindLambdaForTesting(
      [&](SQLitePersistentSharedDictionaryStore::Error error) {
        EXPECT_EQ(expected_error, error);
        run_loop.Quit();
      }));
  run_loop.Run();
  DestroyStore();
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       ClearAllDictionariesErrorDatabaseInitializationFailure) {
  CorruptDatabaseFile();
  RunClearAllDictionariesFailureTest(SQLitePersistentSharedDictionaryStore::
                                         Error::kFailedToInitializeDatabase);
  CheckStoreRecovered();
}

#if !BUILDFLAG(IS_FUCHSIA) && !BUILDFLAG(IS_WIN)
// MakeFileUnwritable() doesn't cause the failure on Fuchsia and Windows. So
// disabling the test on Fuchsia and Windows.
TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       ClearAllDictionariesErrorSqlExecutionFailure) {
  CreateStore();
  ClearAllDictionaries();
  DestroyStore();
  MakeFileUnwritable();
  RunClearAllDictionariesFailureTest(
      SQLitePersistentSharedDictionaryStore::Error::kFailedToExecuteSql);
}
#endif  // !BUILDFLAG(IS_FUCHSIA)

TEST_F(SQLitePersistentSharedDictionaryStoreTest, InvalidHash) {
  CreateStore();
  int64_t primary_key_in_database =
      RegisterDictionary(isolation_key_, dictionary_info_);
  SharedDictionaryInfo expected_info = dictionary_info_;
  expected_info.set_primary_key_in_database(primary_key_in_database);
  EXPECT_THAT(GetDictionaries(isolation_key_),
              ElementsAreArray({expected_info}));
  DestroyStore();

  ManipulateDatabase({"UPDATE dictionaries set sha256='DUMMY'"});

  CreateStore();
  EXPECT_TRUE(GetDictionaries(isolation_key_).empty());
  EXPECT_TRUE(GetAllDictionaries().empty());
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest, InvalidToken) {
  CreateStore();
  int64_t primary_key_in_database =
      RegisterDictionary(isolation_key_, dictionary_info_);
  SharedDictionaryInfo expected_info = dictionary_info_;
  expected_info.set_primary_key_in_database(primary_key_in_database);
  EXPECT_THAT(GetDictionaries(isolation_key_),
              ElementsAreArray({expected_info}));
  DestroyStore();

  // {token_low=0, token_high=0} token is treated as invalid.
  ManipulateDatabase({"UPDATE dictionaries set token_low=0, token_high=0"});

  CreateStore();
  EXPECT_TRUE(GetDictionaries(isolation_key_).empty());
  EXPECT_TRUE(GetAllDictionaries().empty());
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       RegisterDictionaryCallbackNotCalledAfterStoreDeleted) {
  CreateStore();
  store_->RegisterDictionary(
      isolation_key_, dictionary_info_,
      base::BindLambdaForTesting(
          [](SQLitePersistentSharedDictionaryStore::Error error,
             absl::optional<int64_t> primary_key_in_database) {
            EXPECT_TRUE(false) << "Should not be reached.";
          }));
  store_.reset();
  RunUntilIdle();
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       GetDictionariesCallbackNotCalledAfterStoreDeleted) {
  CreateStore();
  store_->GetDictionaries(
      isolation_key_, base::BindLambdaForTesting(
                          [](SQLitePersistentSharedDictionaryStore::Error error,
                             std::vector<SharedDictionaryInfo> dictionaries) {
                            EXPECT_TRUE(false) << "Should not be reached.";
                          }));
  store_.reset();
  RunUntilIdle();
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       GetAllDictionariesCallbackNotCalledAfterStoreDeleted) {
  CreateStore();
  store_->GetAllDictionaries(base::BindLambdaForTesting(
      [](SQLitePersistentSharedDictionaryStore::Error error,
         std::map<SharedDictionaryStorageIsolationKey,
                  std::vector<SharedDictionaryInfo>> all_dictionaries) {
        EXPECT_TRUE(false) << "Should not be reached.";
      }));
  store_.reset();
  RunUntilIdle();
}

TEST_F(SQLitePersistentSharedDictionaryStoreTest,
       ClearAllDictionariesCallbackNotCalledAfterStoreDeleted) {
  CreateStore();
  store_->ClearAllDictionaries(base::BindLambdaForTesting(
      [](SQLitePersistentSharedDictionaryStore::Error error) {
        EXPECT_TRUE(false) << "Should not be reached.";
      }));
  store_.reset();
  RunUntilIdle();
}

}  // namespace net
