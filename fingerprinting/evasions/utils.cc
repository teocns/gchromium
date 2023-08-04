#include <string>
#include <regex>
#include <map>
#include <stdexcept>
#include "third_party/abseil-cpp/absl/types/optional.h"



namespace base{


    
std::string chromeVersion(const std::string& userAgent) {
    std::smatch matches;
    std::regex pattern ("[Chrome|CriOS]/(.*?) ");
    if (std::regex_search(userAgent, matches, pattern)) {
        return matches[1].str();
    }
    return "";
}

std::string chromeMajorVersion(const std::string& userAgent) {
    std::smatch matches;
    std::regex pattern ("[Chrome|CriOS]/(.*?)\\.");
    if (std::regex_search(userAgent, matches, pattern)) {
        return matches[1].str();
    }
}

std::string osType(const std::string& userAgent) {
    std::string result;
    std::map<std::string, std::string> OSArray = {
        {"Windows", "((win32)|(win64)|(windows))"},
        {"macOS", "((macintosh)|(mac68k)|(macppc)|(macintosh))"},
        {"Linux", "linux"},
        {"iPhone", "iphone"},
        {"iPod", "ipod"},
        {"iPad", "ipad"},
        {"Android", "android"}
    };

    std::string lowerUserAgent(userAgent);
    std::transform(userAgent.begin(), userAgent.end(), lowerUserAgent.begin(), ::tolower);

    for (auto &os : OSArray) {
        if (std::regex_search(lowerUserAgent, std::regex(os.second))) {
            result = os.first;
        }
    }

    return result;
}




}
