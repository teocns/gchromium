// demo/fpreader.cc
#include <fstream>
#include <streambuf>
#include <string>
#include "base/json/json_reader.h"
#include "base/time/time.h"
#include "base/files/file_util.h"
#include "base/logging.h"
#include "base/strings/string_util.h"
#include "base/synchronization/lock.h"
#include "base/values.h"
#include "base/values.h"
#include "fingerprinting/core/device_descriptor/fingerprint_impl.h"

std::unique_ptr<fingerprinting::Fingerprint> LoadFingerprint(const std::string& fingerprint_file_path) {
  // Convert std::string to base::FilePath
  std::unique_ptr<fingerprinting::Fingerprint> value;
  std::string fingerprint_str;
  base::FilePath fingerprint_file_path_o =
      base::FilePath(fingerprint_file_path);
  if (!base::ReadFileToString(fingerprint_file_path_o, &fingerprint_str)) {
    LOG(ERROR) << "Failed reading: " << fingerprint_file_path;
  }

  if (!fingerprinting::Fingerprint::FromString(fingerprint_str, &value)) {
    LOG(ERROR) << "Failed to parse JSON.\n";
  }

  return value;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    LOG(ERROR) << "Usage: " << argv[0] << " <json file path>\n";
    return 1;
  }

  std::ifstream t(argv[1]);
  std::string str((std::istreambuf_iterator<char>(t)),
                  std::istreambuf_iterator<char>());

  base::TimeTicks start = base::TimeTicks::Now();

  auto value = LoadFingerprint(argv[1]);

  base::TimeTicks end = base::TimeTicks::Now();

  if (!value) {
    LOG(INFO) << "Failed to parse JSON.\n";
    return 1;
  }

  base::TimeDelta delta = end - start;
  LOG(INFO) << "Time taken to parse JSON: " << delta.InMillisecondsF()
            << "ms\n";

  // Ensure the fingerprint is valid

  base::Value* navigator;
  (void)value->Find({"navigator", "userAgent"},navigator);
  LOG(INFO) << "Fingerprint is valid: " << navigator->GetString();
  return 0;
}
