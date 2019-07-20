#ifndef ReadyPhoneState_H_
#define ReadyPhoneState_H_

#include "phone/PhoneState.h"
#include "phone/PhoneContext.h"

class ReadyPhoneState : public PhoneState {
public:
    explicit ReadyPhoneState(PhoneContext &phoneContext);

    void init() override;

    void onKeyEvent(KeypadEvent key) override;
    void onPhoneCall(const char *number) override;
    void onMissedPhoneCall(const char *number) override;
};


#endif //ReadyPhoneState_H_
