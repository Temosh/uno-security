//
// Created by svtem on 17.07.2019.
//

#ifndef UNO_SECURITY_CALLTASKS_H
#define UNO_SECURITY_CALLTASKS_H

#include "gsm/Sim800Commands.h"
#include "gsm/GsmListeners.h"
#include "AbstractGsmTasks.h"
#include "GsmChainTask.h"

#define GSM_RING_TIMEOUT 4000

using namespace Sim800Commands;


namespace GsmTasks {

    class CallTask : public AbstractGsmTask {
    private:
        const char *number{};
    public:
        CallTask() { status = FAILED; };
        explicit CallTask(const char *number);

        bool getNextCommand(char *command) override;
        unsigned long getTimeout() override;
        bool process(const char *responseLine) override;
    };

    class AnswerCallTask : public AbstractGsmTask {
    public:
        bool getNextCommand(char *command) override;
        bool process(const char *responseLine) override;
    };

    class CancelCallTask : public AbstractGsmTask {
    public:
        bool getNextCommand(char *command) override;
        bool process(const char *responseLine) override;
    };

    class CallHandlerTask : public AbstractGsmTask {
    public:
        CallHandlerTask() { status = ON_HOLD; };

        bool getNextCommand(char *command) override { return false; }
        unsigned long getTimeout() override { return GSM_TASK_NO_TIMEOUT; }
        bool accept() override { return false; }
        bool abort() override { return false; }

        bool process(const char *responseLine) override;
    };

    class RingHandlerTask : public AbstractGsmTask {
    public:
        RingHandlerTask() { status = ON_HOLD; };

        bool getNextCommand(char *command) override { return false; }
        unsigned long getTimeout() override { return GSM_RING_TIMEOUT; }
        bool accept() override { return false; }
        bool abort() override { return false; }

        bool process(const char *responseLine) override;
    };

    class NoOpTask : public AbstractGsmTask {
    public:
        NoOpTask() { status = ON_HOLD; };

        bool getNextCommand(char *command) override { return false; }
        bool accept() override { return false; }
        bool abort() override { return false; }
        bool process(const char *responseLine) override { return false; }
    };

}


#endif //UNO_SECURITY_CALLTASKS_H
