#ifndef ReadyPhoneState_H_
#define ReadyPhoneState_H_

#include "PhoneState.h"
#include "PhoneContext.h"

class ReadyPhoneState : public PhoneState {
public:
    ReadyPhoneState(PhoneContext &phoneContext);

    void init() override;

    void onKeyEvent(KeypadEvent key) override;
    void onPhoneCall() override;
    void onPhoneEvent(GsmStatusCode code) override;
};


#endif //ReadyPhoneState_H_
