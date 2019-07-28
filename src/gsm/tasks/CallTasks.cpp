//
// Created by svtem on 17.07.2019.
//

#include "gsm/tasks/CallTasks.h"

using namespace GsmTasks;


CallTask::CallTask(const char *number) {
    size_t length = strlen(number);
    if (length > GSM_NUMBER_LENGTH) {
        status = FAILED;
        return;
    }
    this->number = number;
}

bool CallTask::getNextCommand(char *command) {
    if (status == READY) {
        status = EXECUTING;
        return generateCommand_P(command, COMMAND_DIAL, number);
    }
    return false;
}

unsigned long CallTask::getTimeout() {
    if (status == ACCEPTED) {
        return GSM_TASK_NO_TIMEOUT;
    } else {
        return AbstractGsmTask::getTimeout();
    }
}

bool CallTask::process(const char *responseLine) {
    GsmStatusCode gsmStatusCode = parseStatusCode(responseLine);

    if (status == ACCEPTED) {
        if (gsmStatusCode == OK) {
            status = COMPLETED;
            return true;
        } else if (gsmStatusCode != UNDEFINED) {
            status = FAILED;
            return true;
        }
    }

    return false;
}

bool AnswerCallTask::process(const char *responseLine) {
    if (status == ACCEPTED) {
        GsmStatusCode gsmStatusCode = parseStatusCode(responseLine);
        if (gsmStatusCode == OK) {
            status = COMPLETED;
            return true;
        } else if (isErrorCode(gsmStatusCode)) {
            status = FAILED;
            return true;
        }
    }
    return false;
}

bool AnswerCallTask::getNextCommand(char *command) {
    status = EXECUTING;
    return generateCommand_P(command, COMMAND_ANSWER);
}

bool CancelCallTask::process(const char *responseLine) {
    if (status == ACCEPTED) {
        GsmStatusCode gsmStatusCode = parseStatusCode(responseLine);
        if (gsmStatusCode == OK) {
            status = COMPLETED;
            return true;
        } else if (isErrorCode(gsmStatusCode)) {
            status = FAILED;
            return true;
        }
    }
    return false;
}

bool CancelCallTask::getNextCommand(char *command) {
    status = EXECUTING;
    return generateCommand_P(command, COMMAND_DISCONNECT);
}

bool CallHandlerTask::process(const char *responseLine) {
    return parseStatusCode(responseLine) != RING;
}

bool RingHandlerTask::process(const char *responseLine) {
    return parseStatusCode(responseLine) == RING; //TODO Need to catch NO CARRIER
}
