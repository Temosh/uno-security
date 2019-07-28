//
// Created by svtem on 23.07.2019.
//

#include <HardwareSerial.h>
#include "gsm/handlers/PhoneHandler.h"


bool PhoneHandler::hasNextCommand() {
    return activeTask->hasNextCommand();
}

bool PhoneHandler::getNextCommand(char *command) {
    return activeTask->getNextCommand(command);
}

unsigned long PhoneHandler::getTimeout() {
    return activeTask->getTimeout();
}

bool PhoneHandler::accept() {
    Serial.println(F("accept..."));
    return activeTask->accept();
}

bool PhoneHandler::abort() {
    if (phoneState == IDLE) return true;
    Serial.println(F("abort..."));

    if (phoneState == CANCELING) {
        changeState(IDLE);
        return true;
    } else {
        cancelCall();
        return false;
    }
}

bool PhoneHandler::process(const char *responseLine) {
    if (activeTask->process(responseLine)) {
        Serial.println(F("process..."));
        if (activeTask->getStatus() == COMPLETED) {
            taskCompleted();
        } else if (activeTask->getStatus() == FAILED || activeTask->getStatus() == ABORTED) {
            taskFailed();
        } else {
            taskProcessed();
        }
        return true;
    }
    return false;
}

TaskStatus PhoneHandler::getStatus() {
    return phoneState == IDLE ? ON_HOLD : PROCESSING;
}

bool PhoneHandler::call(const char *number, IGsmCallListener *callListener) {
    if (phoneState != IDLE) return false;
    Serial.println(F("call..."));

    strncpy(this->number, number, GSM_NUMBER_LENGTH);
    this->number[GSM_NUMBER_LENGTH] = '\0';
    this->callListener = callListener;
    changeState(CALLING);
    return true;
}

bool PhoneHandler::answerCall(IGsmCallListener *callListener) {
    if (phoneState != RINGING) return false;
    Serial.println(F("answerCall..."));

    this->callListener = callListener;
    changeState(ANSWERING);
    return true;
}

bool PhoneHandler::cancelCall() {
    if (phoneState == IDLE) return false;
    Serial.println(F("cancelCall..."));

    changeState(CANCELING);
    return true;
}

void PhoneHandler::taskCompleted() {
    switch (phoneState) {
        case IDLE:
            break;
        case RINGING:
            break;
        case CALLING:
            changeState(ON_CALL);
            break;
        case ANSWERING:
            changeState(ON_CALL);
            break;
        case CANCELING:
            changeState(IDLE);
            break;
        case ON_CALL:
            break;
    }
}

void PhoneHandler::taskFailed() {
    switch (phoneState) {
        case IDLE:
            break;
        case RINGING:
            break;
        case CALLING:
            changeState(IDLE);
            break;
        case ANSWERING:
            changeState(CANCELING);
            break;
        case CANCELING:
            changeState(IDLE);
            break;
        case ON_CALL:
            break;
    }
}

void PhoneHandler::taskProcessed() {
    switch (phoneState) {
        case IDLE:
            changeState(RINGING);
            break;
        case ON_CALL:
            changeState(IDLE);
            break;
        default:
            break;
    }
}

void PhoneHandler::changeState(PhoneHandler::PhoneState newState) {
    if (phoneState == newState) return;

    switch (newState) {
        case IDLE:
            if (callListener) {
                callListener->onCallEnd(number);
            } else if (phoneListener) {
                phoneListener->onMissedPhoneCall(number);
            }
            number[0] = '\0';
            callListener = nullptr;
            activeTask = &ringHandlerTask;
            break;
//        case RINGING_CID:
//            activeTask = &cidTask;
//            break;
        case RINGING:
            if (phoneListener) {
                phoneListener->onPhoneCall(number);
            }
            activeTask = &ringHandlerTask;
            break;
        case CALLING:
            if (callListener) {
                callListener->onDialing(number);
            }
            callTask = CallTask(number);
            activeTask = &callTask;
            break;
        case ANSWERING:
            answerCallTask = AnswerCallTask();
            activeTask = &answerCallTask;
            break;
        case CANCELING:
            cancelCallTask = CancelCallTask();
            activeTask = &cancelCallTask;
            break;
        case ON_CALL:
            if (callListener) {
                callListener->onCallStart(number);
            }
            activeTask = &callHandlerTask;
            break;
    }

    phoneState = newState;
    Serial.print(F("NEW phone state: "));
    traceCurrentState();
    Serial.println();
}

void PhoneHandler::setPhoneListener(IGsmPhoneListener *phoneListener) {
    this->phoneListener = phoneListener;
}

void PhoneHandler::traceCurrentState() {
    switch (phoneState) {
        case IDLE:
            Serial.print(F("IDLE"));
            break;
        case RINGING:
            Serial.print(F("RINGING"));
            break;
        case CALLING:
            Serial.print(F("CALLING"));
            break;
        case ANSWERING:
            Serial.print(F("ANSWERING"));
            break;
        case CANCELING:
            Serial.print(F("CANCELING"));
            break;
        case ON_CALL:
            Serial.print(F("ON_CALL"));
            break;
    }
}
