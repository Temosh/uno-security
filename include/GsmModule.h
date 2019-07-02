#ifndef GsmModule_H_
#define GsmModule_H_

#include <WString.h>
#include <SoftwareSerial.h>

const String TEST = "ATD+380959310248;\r";

enum GsmStatusCode
{
    OK,
    CONNECT,
    RING,
    NO_CARRIER,
    ERROR,
    NO_DIALTONE,
    BUSY,
    NO_ANSWER,
    PROCEEDING,
    UNDEFINED
};

class IGsmPhoneListener
{
public:
    // virtual ~IGsmPhoneListener();
    virtual void onPhoneCall(String number) = 0;
    virtual void onPhoneEvent(GsmStatusCode code) = 0;
};

class IGsmSmsListener
{
public:
    // virtual ~IGsmSmsListener();
    virtual void incomingSms(String number, String sms) = 0;
};

class GsmModule
{
public:
    GsmModule(uint8_t receivePin, uint8_t transmitPin, long speed, bool traceGsmMessages = true);
    ~GsmModule();
    void check();
    void addPhoneListener(IGsmPhoneListener *phoneListener);
    void addSmsListener(IGsmSmsListener *smsListener);
    void sendCommand(String command);
    void call(String);
    void cancelCall();
    void answerCall();

private:
    void parseGsmMessage(String &gmsMessage);
    bool isStatusCode(String response);
    GsmStatusCode parseStatusCode(String response);
    void parseAnswer(String response);
    void processSms();

    SoftwareSerial gsm;
    bool traceGsmMessages;

    IGsmPhoneListener *phoneListener;
    IGsmSmsListener *smsListener;
};

#endif