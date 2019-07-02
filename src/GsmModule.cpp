#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "GsmModule.h"
#include "Sim800Commands.h"

using namespace Sim800Commands;

GsmModule::GsmModule(uint8_t receivePin, uint8_t transmitPin, long speed, bool traceGsmMessages = true)
{
    gsm = new SoftwareSerial(receivePin, transmitPin);
    this->traceGsmMessages = traceGsmMessages;

    gsm->begin(speed);
    // sendCommand(strcat(COMMAND_AT, CR)); //TODO
}

GsmModule::~GsmModule()
{
    delete gsm;
}

void GsmModule::check()
{
    if (gsm->available())
    {
        String gmsMessage = gsm->readString();
        if (traceGsmMessages)
        {
            Serial.println(">>-----"); //TODO For testing only!
            Serial.print(gmsMessage);
            Serial.println("----->>"); //TODO For testing only!
        }
        parseGsmMessage(gmsMessage);
    }
}

void GsmModule::addSmsListener(IGsmSmsListener *smsListener)
{
    this->smsListener = smsListener;
}

void GsmModule::addPhoneListener(IGsmPhoneListener *phoneListener)
{
    this->phoneListener = phoneListener;
}

void GsmModule::sendCommand(String command)
{
    //TODO Add logging
    gsm->print(command);
}

void GsmModule::call(String number)
{
    String command = COMMAND_DIAL + number + SEMICOLON + CR;
    sendCommand(command);
}

void GsmModule::cancelCall()
{
    String command = COMMAND_DISCONNECT + CR;
    sendCommand(command);
}

void GsmModule::answerCall()
{
    String command = COMMAND_ANSWER + CR;
    sendCommand(command);
}

void GsmModule::parseGsmMessage(String &gmsMessage)
{
    if (gmsMessage.charAt(0) == '\0') {
        gmsMessage = gmsMessage.substring(1, gmsMessage.length());
    }

    String response = gmsMessage.substring(gmsMessage.indexOf(CRLN) + 2, gmsMessage.lastIndexOf(CRLN));
    Serial.println("Response: '" + response + "'");

    if (isStatusCode(response))
    {
        GsmStatusCode code = parseStatusCode(response);
        if (code != UNDEFINED && phoneListener)
            phoneListener->onPhoneEvent(code);
    }
    else
    {
        parseAnswer(response);
    }
}

bool GsmModule::isStatusCode(String response)
{
    return response.charAt(0) != '+';
}

GsmStatusCode GsmModule::parseStatusCode(String response)
{
    if (response == CODE_OK)
        return OK;
    if (response == CODE_CONNECT)
        return CONNECT;
    if (response == CODE_RING)
        return RING;
    if (response == CODE_NO_CARRIER)
        return NO_CARRIER;
    if (response == CODE_ERROR)
        return ERROR;
    if (response == CODE_NO_DIALTONE)
        return NO_DIALTONE;
    if (response == CODE_BUSY)
        return BUSY;
    if (response == CODE_NO_ANSWER)
        return NO_ANSWER;
    if (response == CODE_PROCEEDING)
        return PROCEEDING;

    Serial.println("Unknown code: '" + response + "'");
    return UNDEFINED;
}

void GsmModule::parseAnswer(String response)
{
}