#ifndef OnCallPhoneState_H_
#define OnCallPhoneState_H_

#include "PhoneState.h"

class OnCallPhoneState : public PhoneState {
public:
    OnCallPhoneState(PhoneContext &phoneContext);

    void init() override;

    void onKeyEvent(KeypadEvent key) override;
    void onPhoneCall() override;
    void onPhoneEvent(GsmStatusCode code) override;
};


#endif //OnCallPhoneState_H_
