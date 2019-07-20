//
// Created by svtem on 17.07.2019.
//

#include "gsm/tasks/CallTasks.h"

using namespace GsmTasks;


CallTask::CallTask(IGsmCallListener *callListener, const char *number) {
    status = NOT_STARTED;
    this->callListener = callListener;

    size_t length = strlen(number);
    if (length > GSM_NUMBER_LENGTH) {
        status = BAD_COMMAND;
        return;
    }
    strcpy(this->number, number);
    this->number[length] = '\0';
}

bool CallTask::getCommand(char *command) {
    return generateCommand(command, COMMAND_DIAL, number);
}

bool CallTask::accept() {
    if (!AbstractGsmTask::accept()) {
        return false;
    }
    callListener->onDialing(number);
    return true;
}

bool CallTask::abort() {
    cancelCall(FAILED);
    return AbstractGsmTask::abort();
}

bool CallTask::process(const char *responseLine) {
    GsmStatusCode gsmStatusCode = parseStatusCode(responseLine);

    if (gsmStatusCode == UNDEFINED) {
        if (isErrorCode(gsmStatusCode)) {
            cancelCall(FAILED);
            return true;
        }
    }
    if (status == ACCEPTED) {
        if (gsmStatusCode == OK) {
            answerCall();
            return false;
        } else if (gsmStatusCode != UNDEFINED) {
            cancelCall(FAILED);
            return true;
        }
    } else if (status == PROCESSING) {
        if (isSuccessCode(gsmStatusCode)) {
            cancelCall(COMPLETED);
            return true;
        } else {
            cancelCall(FAILED);
            return false;
        }
    }

    return false;
}

void CallTask::answerCall() {
    status = PROCESSING;
    if (callListener) {
        callListener->onCallStart(number);
    }
}

void CallTask::cancelCall(TaskStatus statusCode) {
    this->status = statusCode;
    if (callListener) {
        callListener->onCallEnd(number);
    }
}

bool AnswerCallTask::process(const char *responseLine) {
    if (status == ACCEPTED) {
        if (parseStatusCode(responseLine) == OK) {
            status = COMPLETED;
            callTask.answerCall();
        } else {
            status = FAILED;
            callTask.cancelCall(FAILED);
        }
    }
    return true;
}

bool CancelCallTask::process(const char *responseLine) {
    if (status == ACCEPTED) {
        GsmStatusCode gsmStatusCode = parseStatusCode(responseLine);
        if (gsmStatusCode == OK) {
            status = COMPLETED;
            callTask.cancelCall(COMPLETED);
            return true;
        } else if (isErrorCode(gsmStatusCode)) {
            status = FAILED;
            callTask.cancelCall(FAILED);
            return true;
        }
    }
    return false;
}