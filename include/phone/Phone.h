#ifndef Phone_H_
#define Phone_H_

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "gsm/GsmModule.h"
#include "PhoneContext.h"
#include "PhoneState.h"
#include "Keys.h"
#include "phone/states/ReadyPhoneState.h"

typedef unsigned int uint;
typedef unsigned long ulong;

class Phone : public IGsmPhoneListener, public PhoneContext
{
public:
    Phone(GsmModule &gsm, LiquidCrystal_I2C &lcd);
    ~Phone() override = default;

    void onKeyEvent(KeypadEvent key);
    void onPhoneCall(const char *number) override;
    void onMissedPhoneCall(const char *number) override;

    void changeState(PhoneState *newState) override;

    GsmModule &getGsmModule() override;
    LiquidCrystal_I2C &getLcd() override;

    char *getNumber() override;
    void setNumber(const char *number) override;

private:
    GsmModule &gsm;
    LiquidCrystal_I2C &lcd;

    PhoneState *currentState = new ReadyPhoneState(*this);
    char phoneNumber[GSM_NUMBER_LENGTH + 1];
};

#endif