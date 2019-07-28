//
// Created by svtem on 14.07.2019.
//

#ifndef GsmTypes_H_
#define GsmTypes_H_

#include "GsmTypes.h"

#define GSM_NUMBER_LENGTH 16        //15 digits and '+' sign
#define GSM_DATE_LENGTH 20          //19/06/23,22:44:39+08
#define GSM_SMS_MESSAGE_LENGTH 127


enum GsmStatusCode {
    OK,
    CONNECT,
    RING,
    NO_CARRIER,
    ERROR,
    NO_DIALTONE,
    BUSY,
    NO_ANSWER,
    PROCEEDING,
    CME_ERROR,
    CMS_ERROR,
    UNDEFINED
};

enum TaskStatus {
    READY,
    ON_HOLD,
    EXECUTING,
    ACCEPTED,
    ABORTED,
    PROCESSING,
    COMPLETED,
    FAILED
};

struct Sms {
    char number[GSM_NUMBER_LENGTH + 1];
    char date[GSM_DATE_LENGTH + 1];
    bool unread;
    char message[GSM_SMS_MESSAGE_LENGTH + 1];
};

GsmStatusCode parseStatusCode(const char *codeString);

inline bool isStatusCode(const char *gsmOutputLine) {
    return parseStatusCode(gsmOutputLine) != UNDEFINED;
};

inline bool isStatusCode(GsmStatusCode statusCode) {
    return statusCode != UNDEFINED;
};

bool isSuccessCode(GsmStatusCode statusCode);
bool isErrorCode(GsmStatusCode statusCode);

#endif //GsmTypes_H_
