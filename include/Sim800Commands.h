#ifndef Sim800Commands_H_
#define Sim800Commands_H_

#include <WString.h>
#include <SoftwareSerial.h>

namespace Sim800Commands
{
const String SEMICOLON = ";";

const String CR = "\r";
const String LN = "\n";
const String CRLN = "\r\n";

const String COMMAND_AT = "AT";
const String COMMAND_DIAL = "ATD";
const String COMMAND_DISCONNECT = "ATH";
const String COMMAND_ANSWER = "ATA";

const String CODE_OK = "OK";
const String CODE_CONNECT = "CONNECT";
const String CODE_RING = "RING";
const String CODE_NO_CARRIER = "NO CARRIER";
const String CODE_ERROR = "ERROR";
const String CODE_NO_DIALTONE = "NO DIALTONE";
const String CODE_BUSY = "BUSY";
const String CODE_NO_ANSWER = "NO ANSWER";
const String CODE_PROCEEDING = "PROCEEDING";
} // namespace Sim800Commands

#endif