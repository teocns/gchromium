#ifndef FINGERPRINTING_FINGERPRINT_H_
#define FINGERPRINTING_FINGERPRINT_H_

#include "base/values.h"
#include "fingerprinting/export.h"






namespace fingerprinting{


class FINGERPRINTING_EXPORT Fingerprint{


    public:

    Fingerprint(base::Value& value){
        DCHECK(value.is_dict());
        value_ = std::move(value);
    }

    Fingerprint(base::Value&& value) {
        DCHECK(value.is_dict());
        value_ = std::move(value);
    }

    private:
        base::Value value_;
    
};


}

#endif  // FINGERPRINTING_FINGERPRINT_H_