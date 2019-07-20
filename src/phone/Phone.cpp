#include "phone/Phone.h"

Phone::Phone(GsmModule &gsm, LiquidCrystal_I2C &lcd) : gsm(gsm), lcd(lcd) {
    currentState->init();
}

void Phone::onKeyEvent(KeypadEvent key) {
    currentState->onKeyEvent(key);
}

void Phone::onPhoneCall(const char *number) {
    strcpy(phoneNumber, number);
    currentState->onPhoneCall(number);
}

void Phone::onMissedPhoneCall(const char *number) {
    currentState->onMissedPhoneCall(number);
}

GsmModule &Phone::getGsmModule() {
    return gsm;
}

LiquidCrystal_I2C &Phone::getLcd() {
    return lcd;
}

void Phone::changeState(PhoneState *newState) {
    delete currentState; //TODO DO NOT DELETE PREVIOUS STATE HERE, it is still in process!!!
    currentState = newState;
    currentState->init();
}

char *Phone::getNumber() {
    return phoneNumber;
}

void Phone::setNumber(const char *number) {
    strcpy(phoneNumber, number);
}
