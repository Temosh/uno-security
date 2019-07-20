#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include <gsm/tasks/CallTasks.h>
#include <gsm/tasks/CommandTask.h>
#include "gsm/GsmModule.h"
#include "gsm/tasks/CommandTask.h"
#include "gsm/tasks/CallTasks.h"
#include "gsm/tasks/SmsTasks.h"
#include "Utils.h"

using namespace Sim800Commands;
using namespace GsmTasks;

GsmModule::GsmModule(uint8_t receivePin, uint8_t transmitPin, long speed, bool traceGsmMessages) : gsm(new SoftwareSerial(receivePin, transmitPin)) {
    this->traceGsmMessages = traceGsmMessages;
    gsm->begin(speed);
}

GsmModule::~GsmModule() {
    gsm->end();
    delete gsm;
}

void GsmModule::onTick() {
    while (gsm->available()) {
        char *outputLine = buffer;
        size_t outputLineLength = readGsmOutputLine(outputLine);
        if (outputLineLength == 0) continue;

        processUnsolicitedGsmOutput(outputLine);
    }

    if (ringing && (millis() - gsmRingTime > GSM_RING_TIMEOUT)) {
        ringing = false;
        phoneListener->onMissedPhoneCall("-Unknown-");
    }
}

size_t GsmModule::readGsmOutputLine(char *gsmOutputLine) {
    size_t lineLength = 0;

    if (gsm->available()) {
        lineLength = gsm->readBytesUntil('\r', gsmOutputLine, GSM_BUFFER_SIZE - 1);
        gsmOutputLine[lineLength] = '\0';

        if (lineLength == 0) return lineLength;

        trim(gsmOutputLine);
        lineLength = strlen(gsmOutputLine);

        if (lineLength == 0) return lineLength;

        if (traceGsmMessages) {
            Serial.println(gsmOutputLine);
        }
    }

    return lineLength;
}

void GsmModule::processUnsolicitedGsmOutput(char *gsmOutputLine) {
    auto statusCode = parseStatusCode(gsmOutputLine);
    if (isStatusCode(statusCode)) {
        if (callTask.getStatus() == ACCEPTED || callTask.getStatus() == PROCESSING) {
            callTask.process(gsmOutputLine);
            return;
        } else if (statusCode == RING) {
            gsmRingTime = millis();
            if (!ringing) {
                ringing = true;
                phoneListener->onPhoneCall("-Unknown-");
            }
            return;
        } else if (ringing && statusCode != OK ) {
            ringing = false;
            phoneListener->onMissedPhoneCall("-Unknown-");
            return;
        }
    }

    if (isUnsolicitedResultCode(gsmOutputLine)) {
        return;
    }
}

bool GsmModule::sendCommand(char *command) {
    trim(command);
    auto commandTask = CommandTask(command);
    return processGsmTask(commandTask);
}

bool GsmModule::processGsmTask(AbstractGsmTask &task, bool wait) {
    if (callTask.getStatus() == PROCESSING) return false; //Check if call is active
    if (task.getStatus() != NOT_STARTED) return false;

    gsmTaskStartTime = millis();

    if (executeGsmTask(task)) {
        task.accept();
    } else {
        task.abort();
        return false;
    }

    if (wait) {
        return waitForResponse(task);
    } else {
        return true;
    }
}

bool GsmModule::executeGsmTask(GsmTasks::AbstractGsmTask &task) {
    task.getCommand(buffer);

    if (traceGsmMessages) {
        Serial.print(">");
        Serial.println(buffer);
    }

    gsm->print(buffer);
    gsm->print(CR);

    return waitForEcho(buffer);
}

bool GsmModule::waitForEcho(const char *command) {
    while (millis() - gsmTaskStartTime < GSM_TASK_TIMEOUT) {
        char *outputLine = buffer;
        size_t outputLineLength = readGsmOutputLine(outputLine);
        if (outputLineLength == 0) continue;

        if (strcmp(outputLine, command) == 0) return true;

        processUnsolicitedGsmOutput(outputLine);
    }
    return false;
}

bool GsmModule::waitForResponse(GsmTasks::AbstractGsmTask &task) {
    while (millis() - gsmTaskStartTime < GSM_TASK_TIMEOUT) {
        char *outputLine = buffer;
        size_t outputLineLength = readGsmOutputLine(outputLine);
        if (outputLineLength == 0) continue;

        if (task.process(outputLine)) {
            return true;
        }

        if (task.getStatus() != PROCESSING) {
            processUnsolicitedGsmOutput(outputLine);
        }
    }

    task.abort();
    return false;
}

bool GsmModule::isUnsolicitedResultCode(const char *gsmOutputLine) {
    return gsmOutputLine[0] == '+';
}

bool GsmModule::call(const char *number, IGsmCallListener *callListener) {
    if (callTask.getStatus() == ACCEPTED || callTask.getStatus() == PROCESSING) return false;
    if (ringing) return false;

    callTask = CallTask(callListener, number);
    if (callTask.getStatus() == NOT_STARTED) {
        if (processGsmTask(callTask, false)) {
            return true;
        } else {
            cancelCall();
            return false;
        }
    } else {
        return false;
    }
}

bool GsmModule::answerCall(const char *number, IGsmCallListener *callListener) {
    if (callTask.getStatus() == ACCEPTED || callTask.getStatus() == PROCESSING) return false;
    if (!ringing) return false;

    callTask = CallTask(callListener, number);

    if (callTask.getStatus() != NOT_STARTED) {
        return false;
    }

    auto task = AnswerCallTask(callTask);
    if (processGsmTask(task)) {
        ringing = false;
        return true;
    } else {
        return false;
    }
}

bool GsmModule::cancelCall() {
    ringing = false;
    auto task = CancelCallTask(callTask);

    gsmTaskStartTime = millis();

    if (executeGsmTask(task)) {
        task.accept();
    } else {
        task.abort();
        return false;
    }

    return waitForResponse(task);
}

bool GsmModule::sendSms(const char *number, const char *message) {
    return false;
}

Sms *GsmModule::readAllSms() {
    return nullptr;
}

Sms *GsmModule::readNewSms() {
    return nullptr;
}

Sms GsmModule::readSms(int index) {
    auto task = GsmTasks::GetSms(index);
    processGsmTask(task);
    if (task.getStatus() == COMPLETED) {
        return task.getResultData();
    }

    return Sms{};
}
