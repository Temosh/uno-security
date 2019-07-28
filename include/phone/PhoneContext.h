#ifndef PhoneContext_H_
#define PhoneContext_H_

#include "gsm/handlers/IGsmPhoneHandler.h"


class PhoneState;

class PhoneContext {
public:
    virtual void changeState(PhoneState *newState) = 0;

    virtual IGsmPhoneHandler *getPhoneHandler() = 0;
    virtual LiquidCrystal_I2C &getLcd() = 0;

    virtual char *getNumber() = 0;
    virtual void setNumber(const char *number) = 0;
};

#endif //PhoneContext_H_
