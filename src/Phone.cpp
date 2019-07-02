#include "Phone.h"

Phone::Phone(const GsmModule &gsm, const LiquidCrystal_I2C &lcd) : gsm(gsm), lcd(lcd)
{
    changePhoneState(READY);
}

Phone::~Phone()
{
}

void Phone::onKeyEvent(KeypadEvent key)
{
    switch (phoneState)
    {
    case READY:
        if (key == 'A')
        {
            changePhoneState(DIALING);
        }
        break;
    case DIALING:
        switch (key)
        {
        case 'A':
            gsm.call(phoneNumber);
            changePhoneState(CALLING);
            break;
        case 'B':
            changePhoneState(READY);
            break;
        case 'C':
            phoneNumber = "+380959310248"; //TODO TEMP!!!
            lcd.setCursor(0, 0);
            lcd.print(phoneNumber);
            break;
        case 'D':
        case '*':
        case '#':
            break;
        default:
            phoneNumber += key;
            lcd.setCursor(0, 0);
            lcd.print(phoneNumber);
        }
        break;
    case CALLING: //fall thru
    case ON_CALL:
        if (key == 'B')
        {
            gsm.cancelCall();
            changePhoneState(READY);
        }
        break;
    case INCOMING_CALL:
        if (key == 'A')
        {
            gsm.answerCall();
            changePhoneState(ON_CALL);
        }
        else if (key == 'B')
        {
            gsm.cancelCall();
            changePhoneState(READY);
        }
        break;
    }
}

void Phone::changePhoneState(PhoneState newState)
{
    switch (newState)
    {
    case READY:
        phoneNumber = "+";
        lcd.clear();
        lcd.print("Press A for dial");
        break;
    case DIALING:
        lcd.clear();
        lcd.print(phoneNumber);
        break;
    case CALLING:
        lcd.clear();
        lcd.print("Calling...");
        lcd.setCursor(0, 1);
        lcd.print(phoneNumber);
        break;
    case ON_CALL:
        lcd.clear();
        lcd.print("On call...");
        lcd.setCursor(0, 1);
        lcd.print(phoneNumber);
        break;
    case INCOMING_CALL:
        lcd.clear();
        lcd.print("Incoming call...");
        lcd.setCursor(0, 1);
        lcd.print(phoneNumber);
        break;
    }

    phoneState = newState;
}

void Phone::onPhoneCall(String number)
{
    phoneNumber = number;
    changePhoneState(INCOMING_CALL);
}

void Phone::onPhoneEvent(GsmStatusCode code)
{
    if (code == RING)
    {
        if (phoneState != INCOMING_CALL)
        {
            onPhoneCall("-Unknown-");
        }
    }
    else if (code == OK && phoneState == CALLING)
    {
        changePhoneState(ON_CALL);
    }
    else if (code != OK && phoneState != READY)
    {
        changePhoneState(READY);
    }
}