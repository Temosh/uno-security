#ifndef PhoneState_H_
#define PhoneState_H_

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "GsmModule.h"
#include "PhoneContext.h"

class PhoneState {
public:
    PhoneState(PhoneContext &phoneContext) : phoneContext(phoneContext) {};
    virtual ~PhoneState() = default;

    virtual void init() = 0;

    virtual void onKeyEvent(KeypadEvent key) = 0;
    virtual void onPhoneCall() = 0;
    virtual void onPhoneEvent(GsmStatusCode code) = 0;

protected:
    PhoneContext &phoneContext;
};


#endif //PhoneState_H_
