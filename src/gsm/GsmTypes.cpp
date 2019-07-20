//
// Created by svtem on 17.07.2019.
//

#include "gsm/GsmTypes.h"

GsmStatusCode parseStatusCode(const char *codeString) {
    if (codeString[0] == '+') {
        if (strncmp(codeString, TAG_ERROR_CME, TAG_ERROR_LENGTH) == 0)
            return CME_ERROR;
        if (strncmp(codeString, TAG_ERROR_CMS, TAG_ERROR_LENGTH) == 0)
            return CMS_ERROR;
        return UNDEFINED;
    }
    if (codeString[0] == 'A')
        return UNDEFINED;
    if (strcmp(codeString, CODE_OK) == 0)
        return OK;
    if (strcmp(codeString, CODE_CONNECT) == 0)
        return CONNECT;
    if (strcmp(codeString, CODE_RING) == 0)
        return RING;
    if (strcmp(codeString, CODE_NO_CARRIER) == 0)
        return NO_CARRIER;
    if (strcmp(codeString, CODE_ERROR) == 0)
        return ERROR;
    if (strcmp(codeString, CODE_NO_DIALTONE) == 0)
        return NO_DIALTONE;
    if (strcmp(codeString, CODE_BUSY) == 0)
        return BUSY;
    if (strcmp(codeString, CODE_NO_ANSWER) == 0)
        return NO_ANSWER;
    if (strcmp(codeString, CODE_PROCEEDING) == 0)
        return PROCEEDING;

    return UNDEFINED;
}

bool isSuccessCode(GsmStatusCode statusCode) {
    return statusCode != UNDEFINED && statusCode != ERROR && statusCode != CME_ERROR && statusCode != CMS_ERROR;
}

bool isErrorCode(GsmStatusCode statusCode) {
    return statusCode != UNDEFINED && !isSuccessCode(statusCode);
}
