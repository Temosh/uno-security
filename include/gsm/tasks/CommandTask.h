//
// Created by svtem on 17.07.2019.
//

#ifndef UNO_SECURITY_COMMANDTASK_H
#define UNO_SECURITY_COMMANDTASK_H

#include "AbstractGsmTasks.h"
#include "gsm/GsmTypes.h"


namespace GsmTasks {
    class CommandTask : public AbstractGsmTask {
    private:
        const char *taskCommand;
    public:
        explicit CommandTask(const char *command) : taskCommand(command) {};
        bool getCommand(char *command) override { return generateCommand(command, taskCommand); }
        bool process(const char *responseLine) override { return isStatusCode(responseLine); }
    };
}


#endif //UNO_SECURITY_COMMANDTASK_H
