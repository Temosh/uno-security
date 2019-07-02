#ifndef Phone_H_
#define Phone_H_

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "GsmModule.h"

typedef unsigned int uint;
typedef unsigned long ulong;

enum PhoneState
{
    READY,
    DIALING,
    CALLING,
    ON_CALL,
    INCOMING_CALL
};

class Phone : public IGsmPhoneListener
// class Phone
{
public:
    Phone(const GsmModule &gsm, const LiquidCrystal_I2C &lcd);
    ~Phone();
    void onKeyEvent(KeypadEvent key);
    void onPhoneCall(String number) override;
    void onPhoneEvent(GsmStatusCode code) override;

private:
    void changePhoneState(PhoneState newState);

    GsmModule gsm;
    LiquidCrystal_I2C lcd;

    String phoneNumber;
    PhoneState phoneState;
};

#endif