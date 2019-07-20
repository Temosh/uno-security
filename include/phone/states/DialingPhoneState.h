#ifndef DealingPhoneState_H_
#define DealingPhoneState_H_

#include "phone/PhoneState.h"

class DialingPhoneState : public PhoneState {
public:
    explicit DialingPhoneState(PhoneContext &phoneContext);

    void init() override;

    void onKeyEvent(KeypadEvent key) override;
    void onPhoneCall(const char *number) override;
    void onMissedPhoneCall(const char *number) override;

private:
    char number[GSM_NUMBER_LENGTH + 1]{};
};


#endif //DealingPhoneState_H_
