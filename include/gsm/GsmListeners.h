//
// Created by svtem on 16.07.2019.
//

#ifndef GsmListeners_H_
#define GsmListeners_H_

#include "GsmTypes.h"


class IGsmPhoneListener {
public:
    virtual ~IGsmPhoneListener() = default;;
    virtual void onPhoneCall(const char *number) = 0;
    virtual void onMissedPhoneCall(const char *number) = 0;
};

class IGsmSmsListener {
public:
    virtual ~IGsmSmsListener() = default;;
    virtual void onIncomingSms(String number, String sms) = 0;
};

class IGsmStatusListener {
public:
    virtual ~IGsmStatusListener() = default;;
    virtual void onEvent(GsmStatusCode statusCode) = 0;
};

class IGsmCallListener {
public:
    virtual ~IGsmCallListener() = default;
    virtual void onDialing(const char *number) = 0;
    virtual void onCallStart(const char *number) = 0;
    virtual void onCallEnd(const char *number) = 0;
};

#endif //GsmListeners_H_
