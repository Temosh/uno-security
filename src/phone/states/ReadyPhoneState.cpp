#include "phone/states/ReadyPhoneState.h"
#include "phone/states/IncomingCallPhoneState.h"
#include "phone/states/DialingPhoneState.h"
#include "Keys.h"


ReadyPhoneState::ReadyPhoneState(PhoneContext &phoneContext) : PhoneState(phoneContext) {}

void ReadyPhoneState::init() {
    phoneContext.getLcd().clear();
    phoneContext.getLcd().print(F("Press A for dial"));
}

void ReadyPhoneState::onKeyEvent(KeypadEvent key) {
    if (key == KEY_A) {
        phoneContext.changeState(new DialingPhoneState(phoneContext));
    }
}

void ReadyPhoneState::onPhoneCall(const char *number) {
    phoneContext.setNumber(number);
    phoneContext.changeState(new IncomingCallPhoneState(phoneContext));
}

void ReadyPhoneState::onMissedPhoneCall(const char *number) {

}
