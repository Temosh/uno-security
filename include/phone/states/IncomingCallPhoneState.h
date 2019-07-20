#ifndef IncomingCallPhoneState_H_
#define IncomingCallPhoneState_H_

#include "phone/PhoneState.h"

class IncomingCallPhoneState : public PhoneState {
public:
    explicit IncomingCallPhoneState(PhoneContext &phoneContext);

    void init() override;

    void onKeyEvent(KeypadEvent key) override;
    void onPhoneCall(const char *number) override;
    void onMissedPhoneCall(const char *number) override;

private:
    String number;
};


#endif //IncomingCallPhoneState_H_