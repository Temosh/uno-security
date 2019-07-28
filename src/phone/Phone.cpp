#include "phone/Phone.h"

void Phone::init() {
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

void Phone::setPhoneHandler(IGsmPhoneHandler *handler) {
    phoneHandler = handler;
}

IGsmPhoneHandler *Phone::getPhoneHandler() {
    return phoneHandler;
}
