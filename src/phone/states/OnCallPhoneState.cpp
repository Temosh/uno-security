#include <Keys.h>
#include <phone/states/ReadyPhoneState.h>
#include "phone/states/OnCallPhoneState.h"


OnCallPhoneState::OnCallPhoneState(PhoneContext &phoneContext) : PhoneState(phoneContext) {}

void OnCallPhoneState::init() {

}

void OnCallPhoneState::onKeyEvent(KeypadEvent key) {
    if (key == KEY_B) {
        phoneContext.getGsmModule().cancelCall();
//        phoneContext.changeState(new ReadyPhoneState(phoneContext)); //TODO Check me!!!
    }
}

void OnCallPhoneState::onDialing(const char *number) {
    phoneContext.getLcd().clear();
    phoneContext.getLcd().print("Dialing...");
    phoneContext.getLcd().setCursor(0, 1);
    phoneContext.getLcd().print(number);
}

void OnCallPhoneState::onCallStart(const char *number) {
    phoneContext.getLcd().clear();
    phoneContext.getLcd().print("On call...");
    phoneContext.getLcd().setCursor(0, 1);
    phoneContext.getLcd().print(number);
}

void OnCallPhoneState::onCallEnd(const char *number) {
    phoneContext.changeState(new ReadyPhoneState(phoneContext));
}

void OnCallPhoneState::onPhoneCall(const char *number) {

}

void OnCallPhoneState::onMissedPhoneCall(const char *number) {

}
