#include <Keys.h>
#include <phone/states/OnCallPhoneState.h>
#include <phone/states/ReadyPhoneState.h>
#include "phone/states/IncomingCallPhoneState.h"


IncomingCallPhoneState::IncomingCallPhoneState(PhoneContext &phoneContext) : PhoneState(phoneContext) {}

void IncomingCallPhoneState::init() {
    phoneContext.getLcd().clear();
    phoneContext.getLcd().print("Incoming call...");
    phoneContext.getLcd().setCursor(0, 1);
    phoneContext.getLcd().print(phoneContext.getNumber());
}

void IncomingCallPhoneState::onKeyEvent(KeypadEvent key) {
    if (key == KEY_A) {
        auto onCallPhoneState = new OnCallPhoneState(phoneContext);
        if (phoneContext.getGsmModule().answerCall(phoneContext.getNumber(), onCallPhoneState)) {
            phoneContext.changeState(onCallPhoneState);
        }
    } else if (key == KEY_B) {
        phoneContext.getGsmModule().cancelCall();
        phoneContext.changeState(new ReadyPhoneState(phoneContext));
    }
}

void IncomingCallPhoneState::onPhoneCall(const char *number) {

}

void IncomingCallPhoneState::onMissedPhoneCall(const char *number) {
    phoneContext.changeState(new ReadyPhoneState(phoneContext));
}
