//
// Created by svtem on 13.07.2019.
//

#ifndef GsmTasks_H_
#define GsmTasks_H_

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "../GsmTypes.h"
#include "../GsmListeners.h"

#define GSM_TASK_COMMAND_LENGTH 30


namespace GsmTasks {

    class AbstractGsmTask {
    protected:
        TaskStatus status = NOT_STARTED;

        virtual bool generateCommand(char *command, const char *commandTemplate, ...) {
            va_list args;
            va_start(args, commandTemplate);
            int commandLength = vsnprintf(command, GSM_TASK_COMMAND_LENGTH + 1, commandTemplate, args); //TODO check if there is terminator character
            va_end(args);

            if (commandLength < 0 || commandLength > GSM_TASK_COMMAND_LENGTH) {
                command[0] = '\0';
                status = BAD_COMMAND;
                return false;
            }

            return true;
        }

        static bool inline startsWithTag(const char *responseLine, const char *tag) {
            return strncmp(responseLine, tag, strlen(tag)) == 0;
        }

    public:
        virtual ~AbstractGsmTask() = default;

        virtual bool getCommand(char *command) = 0;

        virtual bool accept() {
            if (status == NOT_STARTED) {
                status = ACCEPTED;
                return true;
            }
            return false;
        };

        virtual bool abort() {
            if (status == NOT_STARTED || status == ACCEPTED || status == PROCESSING) {
                status = ABORTED;
                return true;
            }
            return false;
        }

        virtual bool process(const char *responseLine) = 0;

        TaskStatus getStatus() {
            return status;
        };
    };

    template<typename DATA>
    class AbstractGetGsmTask : public AbstractGsmTask {
        virtual DATA getResultData() = 0;
    };

}

#endif //GsmTasks_H_
