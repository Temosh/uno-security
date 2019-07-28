#ifndef GsmModule_H_
#define GsmModule_H_

#include <WString.h>
#include <SoftwareSerial.h>
#include <gsm/handlers/IGsmPhoneHandler.h>
#include "gsm/handlers/PhoneHandler.h"
#include "gsm/GsmTypes.h"
#include "gsm/tasks/AbstractGsmTasks.h"
#include "gsm/tasks/CallTasks.h"
#include "gsm/GsmListeners.h"

#define GSM_BUFFER_SIZE 350


class GsmModule
{
public:
    GsmModule(uint8_t receivePin, uint8_t transmitPin, long speed, bool traceGsmMessages = true);
    ~GsmModule();
    void init();
    void onTick();

//    IGsmEventHandler *registerEventHandler(IGsmEventListener *eventListener);
    IGsmPhoneHandler *registerPhoneHandler(IGsmPhoneListener *phoneListener);
//    IGsmSmsHandler *registerSmsHandler(IGsmSmsListener *smsListener);

    bool sendCommand(char *command);

private:
    bool processOutgoingCommands();
    bool checkEcho(char *gsmOutputLine);
    bool processGsmResponses(char *gsmOutputLine);

    size_t readGsmOutputLine(char *gsmOutputLine);
    void executeGsmCommand(const char *command);

    SoftwareSerial gsm;
    bool traceGsmMessages;

    PhoneHandler phoneHandler{};
//    SmsHandler smsHandler{};
//    EventHandler eventHandler{};

    //TODO Hardcode
    GsmTasks::AbstractGsmTask *gsmHandlers[1] = {&phoneHandler};
    int gsmHandlerCount = 1;

    GsmTasks::AbstractGsmTask *activeTask{};
    char activeCommand[GSM_TASK_COMMAND_LENGTH]{};
    unsigned long activeTaskLastActionTime = -1;

    char buffer[GSM_BUFFER_SIZE]{};
};

#endif