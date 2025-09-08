cfdo %s/fingerprinting\/core/components\/fingerprinting\/common/g




# Changes from /core/ to /common/

common/cache.h|18 col 11| #include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
common/device_descriptor/fingerprint_impl.cc|5 col 11| #include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
common/device_descriptor/fingerprint_impl.h|7 col 11| #include "fingerprinting/core/device_descriptor/fingerprint.h"
common/device_descriptor/fingerprint_impl.h|8 col 11| #include "fingerprinting/core/device_descriptor/mixins/user-agent.h"
common/device_descriptor/fingerprint_impl.h|9 col 11| #include "fingerprinting/core/device_descriptor/mixins/webgl.h"
common/manager.h|16 col 11| #include "fingerprinting/core/cache.h"
common/manager.h|17 col 11| #include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
common/device_descriptor/mixins/user-agent.h|5 col 11| #include "fingerprinting/core/device_descriptor/fingerprint.h"
common/device_descriptor/mixins/webgl.h|5 col 11| #include "fingerprinting/core/device_descriptor/fingerprint.h"
common/manager.cc|1 col 11| #include "fingerprinting/core/manager.h"
common/manager.cc|11 col 11| #include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
BUILD.gn|40 col 12| #       "//fingerprinting/core:device_descriptor",
public/cpp/common.h|5 col 11| #include "fingerprinting/core/manager.h"
public/cpp/manager.h|14 col 11| #include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
public/cpp/manager.h|15 col 11| #include "fingerprinting/core/manager.h"
public/cpp/manager.cc|12 col 11| #include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
public/cpp/BUILD.gn|21 col 8| "//fingerprinting/core",
public/cpp/mixins/user-agent.mojom.h|4 col 11| #include "fingerprinting/core/manager.h"
demo/v8.cc|9 col 11| #include "fingerprinting/core/device_descriptor/fingerprint_impl.h"
demo/v8.cc|10 col 11| #include "fingerprinting/core/evasions/pack.h"
demo/json.cc|13 col 11| #include "fingerprinting/core/device_descriptor/fingerprint_impl.h"

