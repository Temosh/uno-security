#include <Keys.h>
#include <states/ReadyPhoneState.h>
#include "states/OnCallPhoneState.h"

OnCallPhoneState::OnCallPhoneState(PhoneContext &phoneContext) : PhoneState(phoneContext) {}

void OnCallPhoneState::init() {
    phoneContext.getLcd().clear();
    phoneContext.getLcd().print("On call...");
    phoneContext.getLcd().setCursor(0, 1);
    phoneContext.getLcd().print(phoneContext.getNumber());
}

void OnCallPhoneState::onKeyEvent(KeypadEvent key) {
    if (key == KEY_B) {
        phoneContext.getGsmModule().cancelCall();
        phoneContext.changeState(new ReadyPhoneState(phoneContext));
    }
}

void OnCallPhoneState::onPhoneCall() {

}

void OnCallPhoneState::onPhoneEvent(GsmStatusCode code) {
    if (code != OK && code != RING) {
        phoneContext.changeState(new ReadyPhoneState(phoneContext));
    }
}
