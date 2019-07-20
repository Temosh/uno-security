#ifndef Sim800Commands_H_
#define Sim800Commands_H_

#include <WString.h>
#include <SoftwareSerial.h>

namespace Sim800Commands
{
const char EMPTY[] = "\0";
const char COLON[] = ":";
const char SEMICOLON[] = ";";
const char EQUALS[] = "=";

const char CR[] = "\r";
const char LN[] = "\n";
const char CRLN[] = "\r\n";

#define TAG_ERROR_CME               "+CME ERROR"
#define TAG_ERROR_CMS               "+CMS ERROR"
#define TAG_ERROR_LENGTH            10

#define COMMAND_AT =                "AT"
#define COMMAND_DIAL                "ATD%s;"
#define COMMAND_ANSWER              "ATA"

#define COMMAND_DISCONNECT          "ATH"
#define TAG_SMS_CHANGE_FORMAT       "+CMGF"
#define TAG_SMS_READ                "+CMGR"
#define COMMAND_SMS_CHANGE_FORMAT   "AT+CMGF=%d"
#define COMMAND_SMS_READ            "AT+CMGR=%d"

const char CODE_OK[] = "OK";
const char CODE_CONNECT[] = "CONNECT";
const char CODE_RING[] = "RING";
const char CODE_NO_CARRIER[] = "NO CARRIER";
const char CODE_ERROR[] = "ERROR";
const char CODE_NO_DIALTONE[] = "NO DIALTONE";
const char CODE_BUSY[] = "BUSY";
const char CODE_NO_ANSWER[] = "NO ANSWER";
const char CODE_PROCEEDING[] = "PROCEEDING";

#define SMS_FORMAT_PDU 0;
#define SMS_FORMAT_TEXT 1;

} // namespace Sim800Commands

#endif