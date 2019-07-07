#ifndef IncomingCallPhoneState_H_
#define IncomingCallPhoneState_H_

#include "PhoneState.h"

class IncomingCallPhoneState : public PhoneState {
public:
    IncomingCallPhoneState(PhoneContext &phoneContext);

    void init() override;

    void onKeyEvent(KeypadEvent key) override;
    void onPhoneCall() override;
    void onPhoneEvent(GsmStatusCode code) override;

private:
    String number;
};


#endif //IncomingCallPhoneState_H_