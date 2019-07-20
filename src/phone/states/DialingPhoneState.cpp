#include <phone/states/ReadyPhoneState.h>
#include <phone/states/IncomingCallPhoneState.h>
#include <phone/states/OnCallPhoneState.h>
#include "Keys.h"
#include "phone/states/DialingPhoneState.h"


DialingPhoneState::DialingPhoneState(PhoneContext &phoneContext) : PhoneState(phoneContext) {}

void DialingPhoneState::init() {
    strcpy(number, "+");
    phoneContext.getLcd().clear();
    phoneContext.getLcd().print(number);
}

void DialingPhoneState::onKeyEvent(KeypadEvent key) {
    switch (key) {
        case KEY_A:
            {
                auto *onCallPhoneState = new OnCallPhoneState(phoneContext);
                if (phoneContext.getGsmModule().call(number, onCallPhoneState)) {
                    phoneContext.setNumber(number);
                    phoneContext.changeState(onCallPhoneState);
                }
            }
            break;
        case KEY_B:
            phoneContext.changeState(new ReadyPhoneState(phoneContext));
            break;
        case KEY_C:
            strcpy(number, "+"); //TODO TEMP!!!
            phoneContext.getLcd().setCursor(0, 0);
            phoneContext.getLcd().print(number);
            break;
        case KEY_D:
        case KEY_ASTERISK:
        case KEY_SHARP:
            break;
        default:
            size_t length = strlen(number);
            if (length == GSM_NUMBER_LENGTH) {
                break;
            }
            number[length] = key;
            number[length + 1] = '\0';
            phoneContext.getLcd().setCursor(0, 0);
            phoneContext.getLcd().print(number);
    }
}

void DialingPhoneState::onPhoneCall(const char *number) {
    phoneContext.setNumber(number);
    phoneContext.changeState(new IncomingCallPhoneState(phoneContext));
}

void DialingPhoneState::onMissedPhoneCall(const char *number) {

}
