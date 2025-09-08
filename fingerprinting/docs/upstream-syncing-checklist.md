# Upstream syncing checklist



This is a TODO checklist that must be completed before syncing with upstream.


1. `third_party/blink/common/user_agent/user_agent_metadata.cc`  <> `fingerprinting/utils/user-agent.h`

    * `kPickleVersion` to correspond with `kVersion` of `user_agent_metadata.cc`
    * **Pickling** order and semantics to equal for `GetUserAgentMetadata`.


2. `components/embedder_support/user_agent_utils.h` <> `fingerprinting/utils/user-agent.h`
    * `kMobileFormFactor` to correspond
