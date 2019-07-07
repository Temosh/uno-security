#include "Phone.h"

Phone::Phone(GsmModule &gsm, LiquidCrystal_I2C &lcd) : gsm(gsm), lcd(lcd) {
    currentState->init();
}

void Phone::onKeyEvent(KeypadEvent key) {
    currentState->onKeyEvent(key);
}

void Phone::onPhoneCall(String number) {
    phoneNumber = number;
    currentState->onPhoneCall();
}

void Phone::onPhoneEvent(GsmStatusCode code) {
    currentState->onPhoneEvent(code);
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

String Phone::getNumber() {
    return phoneNumber;
}

void Phone::setNumber(const String &number) {
    phoneNumber = number;
}
