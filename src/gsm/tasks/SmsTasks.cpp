//
// Created by svtem on 17.07.2019.
//

#include "gsm/tasks/SmsTasks.h"

using namespace GsmTasks;


bool ChangeSmsFormat::getCommand(char *command) {
    return generateCommand(command, COMMAND_SMS_READ, format);
}

bool ChangeSmsFormat::process(const char *responseLine) {
    if (status != ACCEPTED) return true;
    if (parseStatusCode(responseLine) == OK) {
        status = COMPLETED;
        return true;
    }
    return false;
}

bool GetSms::getCommand(char *command) {
    return generateCommand(command, COMMAND_SMS_READ, index);
}

//TODO Currently works with SMS-DELIVER only
bool GetSms::process(const char *responseLine) {
    if (status != ACCEPTED && status != PROCESSING) return true;

    if (startsWithTag(responseLine, TAG_SMS_READ)) {
        status = PROCESSING;

        char *startIndex;
        char *endIndex;
        int length;

        //<stat>
        startIndex = strchr(responseLine, '"') + 1;
        endIndex = strchr(startIndex, '"') - 1;
        length = endIndex - startIndex + 1;

        if (isdigit(*startIndex) == 0) {
            if (strncmp(startIndex, "0", 1) == 0) {
                sms.unread = true;
            }
        } else {
            if (strncmp(startIndex, "REC UNREAD", length) == 0) {
                sms.unread = true;
            }
        }

        //<oa>
        startIndex = strchr(endIndex, ',') + 1;
        endIndex = strchr(startIndex, ',') - 1;
        length = endIndex - startIndex + 1;

        strncpy(sms.number, startIndex, length);
        sms.number[length] = '\0';

        //<alpha> - skip
        startIndex = strchr(endIndex, ',') + 1;
        endIndex = strchr(startIndex, ',') - 1;

        //<scts>
        startIndex = strchr(endIndex, ',') + 1;
        endIndex = strchr(startIndex, ',') - 1;
        length = endIndex - startIndex + 1;

        strncpy(sms.date, startIndex, length);
        sms.date[length] = '\0';
    } else if (status == PROCESSING) {
        if (isStatusCode(responseLine)) {
            if (parseStatusCode(responseLine) == OK) {
                status = COMPLETED;
            }
            return true;
        }
        strcpy(sms.message, responseLine);
    }

    return false;
}
