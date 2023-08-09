


#include "third_party/blink/public/web/web_local_frame.h"
#include "third_party/blink/public/web/web_script_source.h"
#include "fingerprinting/evasions/evasion.h"


void Patch(blink::WebLocalFrame* frane, Evasion& evasion){
    blink::WebString js = blink::WebString::FromUTF8(evasion);
    blink::WebScriptSource source(js);
    frane->ExecuteScript(source);
}