//
// Created by svtem on 13.07.2019.
//

#ifndef GsmTasks_H_
#define GsmTasks_H_

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "../GsmTypes.h"
#include "../GsmListeners.h"

#define GSM_TASK_COMMAND_LENGTH 30
#define GSM_DEFAULT_TASK_TIMEOUT 5000
#define GSM_TASK_NO_TIMEOUT -1


namespace GsmTasks {

    class AbstractGsmTask {
    protected:
        TaskStatus status = READY;

        bool generateCommand(char *command, const char *commandTemplate, ...) {
            va_list args;
            va_start(args, commandTemplate);
            int commandLength = vsnprintf(command, GSM_TASK_COMMAND_LENGTH + 1, commandTemplate, args); //TODO check if there is terminator character
            va_end(args);
            return validateCommand(command, commandLength);
        }

        bool generateCommand_P(char *command, PGM_P commandTemplate, ...) {
            va_list args;
            va_start(args, commandTemplate);
            int commandLength = vsnprintf_P(command, GSM_TASK_COMMAND_LENGTH + 1, commandTemplate, args); //TODO check if there is terminator character
            va_end(args);
            return validateCommand(command, commandLength);
        }

        bool validateCommand(char *command, const int commandLength) {
            if (commandLength < 0 || commandLength > GSM_TASK_COMMAND_LENGTH) {
                command[0] = '\0';
                status = FAILED;
                return false;
            }

            return true;
        }

        static bool startsWithTag(const char *responseLine, const char *tag) {
            return strncmp(responseLine, tag, strlen(tag)) == 0;
        }

        static bool startsWithTag_P(const char *responseLine, PGM_P tag) {
            return strncmp_P(responseLine, tag, strlen_P(tag)) == 0;
        }

    public:
        virtual ~AbstractGsmTask() = default;

        virtual bool hasNextCommand() { return status == READY; };
        virtual bool getNextCommand(char *command) = 0;

        virtual unsigned long getTimeout() { return GSM_DEFAULT_TASK_TIMEOUT; };

        virtual bool accept() {
            if (status == EXECUTING) {
                status = ACCEPTED;
                return true;
            }
            return false;
        };

        virtual bool abort() {
            if (status != COMPLETED && status != FAILED) {
                status = ABORTED;
                return true;
            }
            return false;
        }

        virtual bool process(const char *responseLine) = 0;

        virtual TaskStatus getStatus() {
            return status;
        };
    };

    template<typename DATA>
    class AbstractGsmGetTask : public AbstractGsmTask {
    public:
        virtual DATA getResultData() = 0;
    };

}

#endif //GsmTasks_H_
