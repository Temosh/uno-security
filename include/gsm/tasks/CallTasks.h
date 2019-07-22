//
// Created by svtem on 17.07.2019.
//

#ifndef UNO_SECURITY_CALLTASKS_H
#define UNO_SECURITY_CALLTASKS_H

#include "gsm/Sim800Commands.h"
#include "gsm/GsmListeners.h"
#include "AbstractGsmTasks.h"

using namespace Sim800Commands;


namespace GsmTasks {

    class CallTask : public AbstractGsmTask {
    private:
        IGsmCallListener *callListener{};
        char number[GSM_NUMBER_LENGTH + 1]{};

    public:
        CallTask() { status = ABORTED; };
        CallTask(IGsmCallListener *callListener, const char *number);

        bool accept() override;
        bool abort() override;
        bool process(const char *responseLine) override;

        bool getCommand(char *command) override;

        void answerCall();
        void cancelCall(TaskStatus statusCode);
    };

    class AnswerCallTask : public AbstractGsmTask {
    private:
        CallTask &callTask;
    public:
        explicit AnswerCallTask(CallTask &callTask) : callTask(callTask) {};

        bool getCommand(char *command) override { return generateCommand_P(command, COMMAND_ANSWER); }
        bool process(const char *responseLine) override;
    };

    class CancelCallTask : public AbstractGsmTask {
    private:
        CallTask &callTask;
    public:
        explicit CancelCallTask(CallTask &callTask) : callTask(callTask) {};

        bool getCommand(char *command) override { return generateCommand_P(command, COMMAND_DISCONNECT); }
        bool process(const char *responseLine) override;
    };

}


#endif //UNO_SECURITY_CALLTASKS_H
