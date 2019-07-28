#ifndef Phone_H_
#define Phone_H_

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include "gsm/GsmModule.h"
#include "PhoneContext.h"
#include "PhoneState.h"
#include "Keys.h"
#include "phone/states/ReadyPhoneState.h"
#include "gsm/handlers/IGsmPhoneHandler.h"

typedef unsigned int uint;
typedef unsigned long ulong;

class Phone : public IGsmPhoneListener, public PhoneContext
{
public:
    explicit Phone(LiquidCrystal_I2C &lcd) : lcd(lcd) {};
    ~Phone() override = default;

    void init();

    void onKeyEvent(KeypadEvent key);
    void onPhoneCall(const char *number) override;
    void onMissedPhoneCall(const char *number) override;

    void changeState(PhoneState *newState) override;

    IGsmPhoneHandler *getPhoneHandler() override;
    LiquidCrystal_I2C &getLcd() override;

    char *getNumber() override;
    void setNumber(const char *number) override;

    void setPhoneHandler(IGsmPhoneHandler *handler);

private:
    IGsmPhoneHandler *phoneHandler{};
    LiquidCrystal_I2C &lcd;

    PhoneState *currentState = new ReadyPhoneState(*this);
    char phoneNumber[GSM_NUMBER_LENGTH + 1]{};
};

#endif