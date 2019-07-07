#include <states/ReadyPhoneState.h>
#include <states/IncomingCallPhoneState.h>
#include <states/OnCallPhoneState.h>
#include "Keys.h"
#include "states/DialingPhoneState.h"

DialingPhoneState::DialingPhoneState(PhoneContext &phoneContext) : PhoneState(phoneContext) {}

void DialingPhoneState::init() {
    number = "+";
    phoneContext.getLcd().clear();
    phoneContext.getLcd().print(number);
}

void DialingPhoneState::onKeyEvent(KeypadEvent key) {
    switch (key) {
        case KEY_A:
            phoneContext.getGsmModule().call(number);
            phoneContext.setNumber(number);
            phoneContext.changeState(new OnCallPhoneState(phoneContext));
            break;
        case KEY_B:
            phoneContext.changeState(new ReadyPhoneState(phoneContext));
            break;
        case KEY_C:
            number = "+380959310248"; //TODO TEMP!!!
            phoneContext.getLcd().setCursor(0, 0);
            phoneContext.getLcd().print(number);
            break;
        case KEY_D:
        case KEY_ASTERISK:
        case KEY_SHARP:
            break;
        default:
            number += key;
            phoneContext.getLcd().setCursor(0, 0);
            phoneContext.getLcd().print(number);
    }
}

void DialingPhoneState::onPhoneCall() {
    phoneContext.setNumber(number);
    phoneContext.changeState(new IncomingCallPhoneState(phoneContext));
}

void DialingPhoneState::onPhoneEvent(GsmStatusCode code) {
    if (code == RING) {
        phoneContext.setNumber("-Unknown-");
        phoneContext.changeState(new IncomingCallPhoneState(phoneContext));
    }
}
