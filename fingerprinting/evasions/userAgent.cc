
#include "fingerprinting/evasions/evasion.h"



class UserAgentEvasion : public IEvasion{

    js = R"(
        var userAgent = navigator.userAgent;
        Object.defineProperty(navigator, 'userAgent', {
            get: function () {
                return userAgent;
            }
        });
    )";



    GetPatchedUA(){
        return "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko)";
    }

}