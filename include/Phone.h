#ifndef Phone_H_
#define Phone_H_

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "GsmModule.h"
#include "PhoneContext.h"
#include "PhoneState.h"
#include "Keys.h"
#include "states/ReadyPhoneState.h"

typedef unsigned int uint;
typedef unsigned long ulong;

class Phone : public IGsmPhoneListener, public PhoneContext
{
public:
    Phone(GsmModule &gsm, LiquidCrystal_I2C &lcd);
    ~Phone() = default;

    void onKeyEvent(KeypadEvent key);
    void onPhoneCall(String number) override;
    void onPhoneEvent(GsmStatusCode code) override;

    void changeState(PhoneState *newState) override;

    GsmModule &getGsmModule() override;
    LiquidCrystal_I2C &getLcd() override;

    String getNumber() override;
    void setNumber(const String &number) override;

private:
    GsmModule &gsm;
    LiquidCrystal_I2C &lcd;

    PhoneState *currentState = new ReadyPhoneState(*this);
    String phoneNumber;
};

#endif