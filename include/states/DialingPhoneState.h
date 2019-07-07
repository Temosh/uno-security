#ifndef DealingPhoneState_H_
#define DealingPhoneState_H_

#include "PhoneState.h"

class DialingPhoneState : public PhoneState {
public:
    DialingPhoneState(PhoneContext &phoneContext);

    void init() override;

    void onKeyEvent(KeypadEvent key) override;
    void onPhoneCall() override;
    void onPhoneEvent(GsmStatusCode code) override;

private:
    String number = "";
};


#endif //DealingPhoneState_H_
