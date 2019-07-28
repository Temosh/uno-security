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


GsmModule::GsmModule(uint8_t receivePin, uint8_t transmitPin, long speed, bool traceGsmMessages) : gsm(
        SoftwareSerial(receivePin, transmitPin)) {
    this->traceGsmMessages = traceGsmMessages;
    gsm.begin(speed);
}

GsmModule::~GsmModule() {
    gsm.end();
}

void GsmModule::init() {
    if (strlen_P(COMMAND_AT) > GSM_TASK_COMMAND_LENGTH) {
        //TODO Add error log
        return;
    }
    strcpy_P(buffer, COMMAND_AT);
    executeGsmCommand(buffer);
}

void GsmModule::onTick() {
    if (activeTask) {
        if (activeTask->getStatus() == ON_HOLD ||
            activeTask->getStatus() == COMPLETED ||
            activeTask->getStatus() == FAILED ||
            activeTask->getStatus() == ABORTED) {
            activeTask = nullptr;
        }
    }

    char *outputLine = buffer;
    size_t outputLineLength = readGsmOutputLine(outputLine);

    if (outputLineLength > 0) {
        if (checkEcho(outputLine)) return;
        if (processGsmResponses(outputLine)) return;
    } else {
        if (processOutgoingCommands()) return;
    }

    if (activeTask && activeTask->getTimeout() > 0 && millis() - activeTaskLastActionTime > activeTask->getTimeout()) {
        activeTask->abort();
    }
}

bool GsmModule::processOutgoingCommands() {
    if (strlen(activeCommand) > 0) return false;

    if (!activeTask) {
        for (int i = 0; i < gsmHandlerCount; ++i) {
            if (gsmHandlers[i]->hasNextCommand()) {
                activeTask = gsmHandlers[i];
                break;
            }
        }
    }

    if (!activeTask || !activeTask->hasNextCommand()) return false;

    activeTask->getNextCommand(buffer);
    if (strlen(buffer) < 1 || strlen(buffer) > GSM_TASK_COMMAND_LENGTH) {
        activeTask->abort();
        return false;
    }

    strcpy(activeCommand, buffer);
    executeGsmCommand(activeCommand);
    activeTaskLastActionTime = millis();
    return true;
}

bool GsmModule::checkEcho(char *gsmOutputLine) {
    if (strlen(activeCommand) > 0) {
        if (strcmp(activeCommand, gsmOutputLine) == 0) {
            activeTask->accept();
            activeCommand[0] = '\0';
            activeTaskLastActionTime = millis();
            return true;
        }
    }
    return false;
}

bool GsmModule::processGsmResponses(char *gsmOutputLine) {
    if (activeTask) {
        if (activeTask->process(gsmOutputLine)) {
            activeTaskLastActionTime = millis();
            return true;
        }
    } else {
        for (int i = 0; i < gsmHandlerCount; ++i) {
            if (gsmHandlers[i]->process(gsmOutputLine)) {
                if (gsmHandlers[i]->getStatus() == PROCESSING) {
                    activeTask = gsmHandlers[i];
                    activeTaskLastActionTime = millis();
                }
                return true;
            }
        }
    }
    return false;
}


IGsmPhoneHandler *GsmModule::registerPhoneHandler(IGsmPhoneListener *phoneListener) {
    phoneHandler.setPhoneListener(phoneListener);
    return &phoneHandler;
}

size_t GsmModule::readGsmOutputLine(char *gsmOutputLine) {
    size_t lineLength = 0;

    if (gsm.available()) {
        lineLength = gsm.readBytesUntil('\r', gsmOutputLine, GSM_BUFFER_SIZE - 1);
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

bool GsmModule::sendCommand(char *command) {
    if (activeTask) return false;
    trim(command);
    if (strlen(command) > 0) {
        executeGsmCommand(command);
        return true;
    }
    return false;
}

void GsmModule::executeGsmCommand(const char *command) {
    if (traceGsmMessages) {
        Serial.print(">");
        Serial.println(command);
    }
    gsm.print(command);
    gsm.print(CR);
}
