#ifndef GsmModule_H_
#define GsmModule_H_

#include <WString.h>
#include <SoftwareSerial.h>
#include "gsm/GsmTypes.h"
#include "gsm/tasks/AbstractGsmTasks.h"
#include "gsm/tasks/CallTasks.h"
#include "gsm/GsmListeners.h"

#define GSM_RING_TIMEOUT 4000
#define GSM_TASK_TIMEOUT 5000
#define GSM_BUFFER_SIZE 256


class GsmModule
{
public:
    GsmModule(uint8_t receivePin, uint8_t transmitPin, long speed, bool traceGsmMessages = true);
    ~GsmModule();
    void onTick();

    void addPhoneListener(IGsmPhoneListener *phoneListener) { this->phoneListener = phoneListener; };
    void addSmsListener(IGsmSmsListener *smsListener) { this->smsListener = smsListener; };
    void addStatusListener(IGsmStatusListener *statusListener) { this->statusListener = statusListener; };;

    bool sendCommand(char *command);

    bool call(const char *number, IGsmCallListener *callListener);
    bool answerCall(const char *number, IGsmCallListener *callListener);
    bool cancelCall();

    bool sendSms(const char *number, const char *message);
    Sms *readAllSms();
    Sms *readNewSms();
    Sms readSms(int index);

private:
    size_t readGsmOutputLine(char *gsmOutputLine);
    void processUnsolicitedGsmOutput(char *gsmOutputLine);
    bool processGsmTask(GsmTasks::AbstractGsmTask &task, bool waitForResponse = true);
    bool executeGsmTask(GsmTasks::AbstractGsmTask &task);
    bool waitForEcho(const char *command);
    bool waitForResponse(GsmTasks::AbstractGsmTask &task);

    static bool isUnsolicitedResultCode(const char *gsmOutputLine);

    SoftwareSerial *gsm;
    bool traceGsmMessages;

    IGsmPhoneListener *phoneListener{};
    IGsmSmsListener *smsListener{};
    IGsmStatusListener *statusListener{};

    GsmTasks::CallTask callTask{};
    bool ringing = false;

    unsigned long gsmRingTime = -1;
    unsigned long gsmTaskStartTime = -1;
    char buffer[GSM_BUFFER_SIZE]{};
};

#endif