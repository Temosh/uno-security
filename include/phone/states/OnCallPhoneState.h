#ifndef OnCallPhoneState_H_
#define OnCallPhoneState_H_

#include "phone/PhoneState.h"

class OnCallPhoneState : public PhoneState, public IGsmCallListener {
public:
    explicit OnCallPhoneState(PhoneContext &phoneContext);

    void init() override;

    void onKeyEvent(KeypadEvent key) override;
    void onPhoneCall(const char *number) override;
    void onMissedPhoneCall(const char *number) override;

private:
    void onDialing(const char *number) override;
    void onCallStart(const char *number) override;
    void onCallEnd(const char *number) override;
};


#endif //OnCallPhoneState_H_
