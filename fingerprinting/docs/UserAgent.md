# User Agent Client Hints [UACH]


2. `components/embedder_support/user_agent_utils.cc`
1. `components/version_info/version_info.cc`





A few stack traces to retrieve the user-agent:


```
3   libchrome_dll.dylib                 0x0000000109c30f04 version_info::GetVersion() + 76
4   libchrome_dll.dylib                 0x0000000109c30e30 version_info::GetMajorVersionNumberAsInt() + 28
5   libchrome_dll.dylib                 0x0000000109c30dcc version_info::GetMajorVersionNumber() + 104
6   libchrome_dll.dylib                 0x000000010abb7124 embedder_support::GetUserAgentMetadata(PrefService const*) + 444
7   libchrome_dll.dylib                 0x000000010970b144 ChromeContentBrowserClient::GetUserAgentMetadata() + 64
8   libcontent.dylib                    0x000000010f2ad0e4 content::RenderProcessHostImpl::Init() + 752



3   libchrome_dll.dylib                 0x0000000109c30bb0 version_info::GetProductNameAndVersionForReducedUserAgent()
5   libchrome_dll.dylib                 0x000000010abb5420 embedder_support::GetUserAgentInternal(embedder_support::ForceMajorVersionToMinorPosition




1   libbase.dylib                       0x0000000102eaa4ec base::debug::StackTrace::StackTrace() + 24
2   libchrome_dll.dylib                 0x0000000109c30ab4 version_info::logTrace(std::__Cr::basic_string<char, std::__Cr::char_traits<char>, std::__Cr::allocator<char>>) + 44
3   libchrome_dll.dylib                 0x0000000109c30f04 version_info::GetVersion() + 76
4   libchrome_dll.dylib                 0x000000010a7715cc extensions::ChromeExtensionsBrowserClient::DidVersionUpdate(content::BrowserContext*) + 624
5   libchrome_dll.dylib                 0x0000000108fd6290 extensions::RuntimeAPI::RuntimeAPI(content::BrowserContext*) + 524
6   libkeyed_service_core.dylib         0x0000000105295900 KeyedServiceFactory::GetServiceForContext(void*, bool) + 244
```



- `blink::UserAgentMetadata GetUserAgentMetadata` return data


[29845:259:0824/215236.310718:ERROR:user_agent_utils.cc(594)] User Agent Metadata:
  Full Version: 117.0.5855.3
  Platform: macOS
  Architecture: arm
  Model:
  Mobile: false
  Form Factor:
  Platform Version: 13.3.1
  Bitness: 64
  WoW64: false






These are the callee methods that I expect to invkoe the user agnet utility methods. Is there a global interface I can share between all?

SystemNetworkContextManager::ConfigureDefaultNetworkContextParams(network::mojom::NetworkContextParams*)

media_router::GetCastMessageHandler()

printing::InitializePrintingForWebContents(content::WebContents*)

content::(anonymous namespace)::UpdateNavigationRequestClientUaHeadersImpl


content::RenderProcessHostImpl::Init()
