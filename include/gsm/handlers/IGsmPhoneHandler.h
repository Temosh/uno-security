//
// Created by svtem on 28.07.2019.
//

#ifndef UNO_SECURITY_IGSMPHONEHANDLER_H
#define UNO_SECURITY_IGSMPHONEHANDLER_H

#include "gsm/GsmListeners.h"


class IGsmPhoneHandler {
public:
    virtual bool call(const char *number, IGsmCallListener *callListener) = 0;
    virtual bool answerCall(IGsmCallListener *callListener) = 0;
    virtual bool cancelCall() = 0;
};

#endif //UNO_SECURITY_IGSMPHONEHANDLER_H
