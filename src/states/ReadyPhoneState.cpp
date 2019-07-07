#include "states/ReadyPhoneState.h"
#include "states/IncomingCallPhoneState.h"
#include "states/DialingPhoneState.h"
#include "Keys.h"

ReadyPhoneState::ReadyPhoneState(PhoneContext &phoneContext) : PhoneState(phoneContext) {}

void ReadyPhoneState::init() {
    phoneContext.getLcd().clear();
    phoneContext.getLcd().print("Press A for dial");
}

void ReadyPhoneState::onKeyEvent(KeypadEvent key) {
    if (key == KEY_A) {
        phoneContext.changeState(new DialingPhoneState(phoneContext));
    }
}

void ReadyPhoneState::onPhoneCall() {
    phoneContext.changeState(new IncomingCallPhoneState(phoneContext));
}

void ReadyPhoneState::onPhoneEvent(GsmStatusCode code) {
    if (code == RING) {
        phoneContext.setNumber("-Unknown-");
        phoneContext.changeState(new IncomingCallPhoneState(phoneContext));
    }
}
