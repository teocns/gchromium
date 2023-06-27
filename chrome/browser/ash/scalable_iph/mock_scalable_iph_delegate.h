// Copyright 2023 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_ASH_SCALABLE_IPH_MOCK_SCALABLE_IPH_DELEGATE_H_
#define CHROME_BROWSER_ASH_SCALABLE_IPH_MOCK_SCALABLE_IPH_DELEGATE_H_

#include <memory>

#include "chromeos/ash/components/scalable_iph/iph_session.h"
#include "chromeos/ash/components/scalable_iph/scalable_iph_delegate.h"
#include "components/keyed_service/core/keyed_service.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace ash {
namespace test {

class MockScalableIphDelegate : public scalable_iph::ScalableIphDelegate {
 public:
  MockScalableIphDelegate();
  ~MockScalableIphDelegate() override;

  MOCK_METHOD(void,
              ShowBubble,
              (const scalable_iph::ScalableIphDelegate::BubbleParams& params,
               std::unique_ptr<scalable_iph::IphSession> iph_session),
              (override));
  MOCK_METHOD(
      void,
      ShowNotification,
      (const scalable_iph::ScalableIphDelegate::NotificationParams& params,
       std::unique_ptr<scalable_iph::IphSession> iph_session),
      (override));
  MOCK_METHOD(void, AddObserver, (Observer * observer), (override));
  MOCK_METHOD(void, RemoveObserver, (Observer * observer), (override));
  MOCK_METHOD(bool, IsOnline, (), (override));

  // Use a delegate object for observer related behaviors, i.e. Events from Ash.
  // A delegate object can be a fake object. We will want a fake behavior for
  // the most of cases, i.e. Unlike `ShowBubble` etc, we won't test `IsOnline`
  // called but we simulate/fake events/states. For now, we simply use a real
  // object as `ScalableIphDelegateImpl` works as a fake easily for now and it
  // can increases test coverage easily.
  void DelegateObserverWith(
      std::unique_ptr<scalable_iph::ScalableIphDelegate> delegate);

 private:
  std::unique_ptr<scalable_iph::ScalableIphDelegate> delegate_;
};

}  // namespace test
}  // namespace ash

#endif  // CHROME_BROWSER_ASH_SCALABLE_IPH_MOCK_SCALABLE_IPH_DELEGATE_H_
