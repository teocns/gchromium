

Below are the traces for the most common platform [client] hints and where they get invoked from



# content::GetCpuBitness()


user_agent.cc 
Only invoked by `embedder_support::GetUserAgentMetadata``
Returns string `64`



# content::GetCpuArchitecture


user_agent.cc 
Only invoked by `embedder_support::GetUserAgentMetadata``
Returns string `arm`


# version_info::GetVersion()

Invoked by:
- GetMajorVersionNumberAsInt
- downgrade::DowngradeManager::PrepareUserDataDirectoryForCurrentVersion(base::FilePath const&)
- extensions::ChromeExtensionsBrowserClient::DidVersionUpdate(content::BrowserContext*)

We should not modify this function as it is used by other internal utilities.


Returns:
- string `117.0.5855.3`

# version_info::GetMajorVersionNumberAsInt

Only invoked by GetMajorVersionNumber

Returns 