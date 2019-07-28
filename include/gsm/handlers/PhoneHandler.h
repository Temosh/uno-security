//
// Created by svtem on 23.07.2019.
//

#ifndef UNO_SECURITY_PHONEHANDLER_H
#define UNO_SECURITY_PHONEHANDLER_H

#include "gsm/tasks/AbstractGsmTasks.h"
#include "gsm/tasks/CallTasks.h"
#include "IGsmPhoneHandler.h"

using namespace GsmTasks;


class PhoneHandler : public AbstractGsmTask, public IGsmPhoneHandler {
private:
    enum PhoneState {
        IDLE,
//        RINGING_CID,
        RINGING,
        CALLING,
        ANSWERING,
        CANCELING,
        ON_CALL
    };

    RingHandlerTask ringHandlerTask{};
    CallHandlerTask callHandlerTask{};

    NoOpTask noOpTask{};
    CallTask callTask{};
    AnswerCallTask answerCallTask{};
    CancelCallTask cancelCallTask{};
//    CidTask<char*> cidTask{};

    PhoneState phoneState = IDLE;
    AbstractGsmTask *activeTask = &ringHandlerTask;

    IGsmPhoneListener *phoneListener{};
    IGsmCallListener *callListener{};
    char number[GSM_NUMBER_LENGTH + 1]{}; //TODO Change to uint64_t + 'international' bool

public:
    bool hasNextCommand() override;
    bool getNextCommand(char *command) override;
    unsigned long getTimeout() override;
    bool accept() override;
    bool abort() override;
    bool process(const char *responseLine) override;
    TaskStatus getStatus() override;

    bool call(const char *number, IGsmCallListener *callListener) override;
    bool answerCall(IGsmCallListener *callListener) override;
    bool cancelCall() override;

    void setPhoneListener(IGsmPhoneListener *phoneListener);

private:
    void taskCompleted();
    void taskFailed();
    void taskProcessed();

    void changeState(PhoneState newState);

    void traceCurrentState();
};


#endif //UNO_SECURITY_PHONEHANDLER_H
