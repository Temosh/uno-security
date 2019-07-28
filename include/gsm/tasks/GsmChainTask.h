//
// Created by svtem on 23.07.2019.
//

#ifndef UNO_SECURITY_GSMCHAINTASK_H
#define UNO_SECURITY_GSMCHAINTASK_H

#include "AbstractGsmTasks.h"


namespace GsmTasks {

    class GsmChainTask : public AbstractGsmTask {
    private:
        AbstractGsmTask *chain = nullptr;
        int chainSize = 0;
        int currentTaskIndex = 0;

    public:
        GsmChainTask(AbstractGsmTask *chain, int chainSize) : chain(chain), chainSize(chainSize) {};

        bool hasNextCommand() override {
            if (chainSize > 0 && chain != nullptr) {
                return chain[currentTaskIndex].hasNextCommand();
            }
            return false;
        }

        bool getNextCommand(char *command) override {
            if (chainSize > 0 && chain != nullptr) {
                return chain[currentTaskIndex].getNextCommand(command);
            }
            return false;
        }

        bool accept() override {
            if (chainSize > 0 && chain != nullptr) {
                return chain[currentTaskIndex].accept();
            }
            return false;
        }

        bool abort() override {
            if (chainSize > 0 && chain != nullptr) {
                return chain[currentTaskIndex].abort();
            }
            return false;
        }

        bool process(const char *responseLine) override {
            if (chainSize > 0 && chain != nullptr) {
                bool processed = chain[currentTaskIndex].process(responseLine);
                if (processed) {
                    switch (chain[currentTaskIndex].getStatus()) {
                        case READY:
                            //Task should not have such status after being processed
                            return false;
                        case ON_HOLD:
                            //Fall thru
                        case EXECUTING:
                            //Fall thru
                        case ACCEPTED:
                            //Fall thru
                        case PROCESSING:
                            status = chain[currentTaskIndex].getStatus();
                            return true;
                        case COMPLETED:
                            currentTaskIndex++;
                            if (currentTaskIndex < chainSize) {
                                return true;
                            }
                            //Fall thru
                        case FAILED:
                            //Fall thru
                        case ABORTED:
                            status = chain[currentTaskIndex].getStatus();
                            currentTaskIndex = 0;
                            return true;
                    }
                }
            }
            return false;
        }
    };
    
}

#endif //UNO_SECURITY_GSMCHAINTASK_H
