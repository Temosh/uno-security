//
// Created by svtem on 17.07.2019.
//

#ifndef UNO_SECURITY_SMSTASKS_H
#define UNO_SECURITY_SMSTASKS_H

#include "AbstractGsmTasks.h"
#include "gsm/GsmTypes.h"


namespace GsmTasks {
    class ChangeSmsFormat : public AbstractGsmTask {
    private:
        int format;
    public:
        explicit ChangeSmsFormat(int format) : format(format) {};

        bool getNextCommand(char *command) override;
        bool process(const char *responseLine) override;
    };

    class GetSms : public AbstractGsmGetTask<Sms> {
    private:
        int index;
        Sms sms{};
    public:
        explicit GetSms(int index) : index(index) {};

        bool getNextCommand(char *command) override;
        bool process(const char *responseLine) override;
        Sms getResultData() override { return sms; };
    };
}

#endif //UNO_SECURITY_SMSTASKS_H
