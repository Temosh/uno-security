#ifndef PhoneState_H_
#define PhoneState_H_

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "gsm/GsmModule.h"
#include "PhoneContext.h"

class PhoneState {
public:
    PhoneState(PhoneContext &phoneContext) : phoneContext(phoneContext) {};
    virtual ~PhoneState() = default;

    virtual void init() = 0;

    virtual void onKeyEvent(KeypadEvent key) = 0;
    virtual void onPhoneCall(const char *number) = 0;
    virtual void onMissedPhoneCall(const char *number) = 0;

protected:
    PhoneContext &phoneContext;
};


#endif //PhoneState_H_
