#include <Keys.h>
#include <states/OnCallPhoneState.h>
#include <states/ReadyPhoneState.h>
#include "states/IncomingCallPhoneState.h"

IncomingCallPhoneState::IncomingCallPhoneState(PhoneContext &phoneContext) : PhoneState(phoneContext) {}

void IncomingCallPhoneState::init() {
    phoneContext.getLcd().clear();
    phoneContext.getLcd().print("Incoming call...");
    phoneContext.getLcd().setCursor(0, 1);
    phoneContext.getLcd().print(phoneContext.getNumber());
}

void IncomingCallPhoneState::onKeyEvent(KeypadEvent key) {
    if (key == KEY_A) {
        phoneContext.getGsmModule().answerCall();
        phoneContext.changeState(new OnCallPhoneState(phoneContext));
    } else if (key == KEY_B) {
        phoneContext.getGsmModule().cancelCall();
        phoneContext.changeState(new ReadyPhoneState(phoneContext));
    }
}

void IncomingCallPhoneState::onPhoneCall() {

}

void IncomingCallPhoneState::onPhoneEvent(GsmStatusCode code) {
    if (code != OK && code != RING) {
        phoneContext.changeState(new ReadyPhoneState(phoneContext));
    }
}
