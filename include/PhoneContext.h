#ifndef PhoneContext_H_
#define PhoneContext_H_

//enum PhoneStates
//{
//    READY,
//    DIALING,
//    CALLING,
//    ON_CALL,
//    INCOMING_CALL
//};

class PhoneState;

class PhoneContext {
public:
    virtual void changeState(PhoneState *newState) = 0;

    virtual GsmModule &getGsmModule() = 0;
    virtual LiquidCrystal_I2C &getLcd() = 0;

    virtual String getNumber() = 0;
    virtual void setNumber(const String &number) = 0;
};

#endif //PhoneContext_H_
