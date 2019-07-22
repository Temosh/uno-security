#ifndef Sim800Commands_H_
#define Sim800Commands_H_

#include <WString.h>
#include <SoftwareSerial.h>
#include <avr/pgmspace.h>


namespace Sim800Commands
{
const char EMPTY[] PROGMEM                      = "\0";
const char COLON[] PROGMEM                      = ":";
const char SEMICOLON[] PROGMEM                  = ";";
const char EQUALS[] PROGMEM                     = "=";

const char CR                                   = '\r';
const char LN                                   = '\n';
const char CRLN[]                               = "\r\n";

const char TAG_ERROR_CME[] PROGMEM              = "+CME ERROR";
const char TAG_ERROR_CMS[] PROGMEM              = "+CMS ERROR";
#define TAG_ERROR_LENGTH                        10

const char COMMAND_AT[] PROGMEM                 = "AT";
const char COMMAND_DIAL[] PROGMEM               = "ATD%s;";
const char COMMAND_ANSWER[] PROGMEM             = "ATA";

const char COMMAND_DISCONNECT[] PROGMEM         = "ATH";
const char TAG_SMS_CHANGE_FORMAT[] PROGMEM      = "+CMGF";
const char TAG_SMS_READ[] PROGMEM               = "+CMGR";
const char COMMAND_SMS_CHANGE_FORMAT[] PROGMEM  = "AT+CMGF=%d";
const char COMMAND_SMS_READ[] PROGMEM           = "AT+CMGR=%d";

const char CODE_OK[] PROGMEM                    = "OK";
const char CODE_CONNECT[] PROGMEM               = "CONNECT";
const char CODE_RING[] PROGMEM                  = "RING";
const char CODE_NO_CARRIER[] PROGMEM            = "NO CARRIER";
const char CODE_ERROR[] PROGMEM                 = "ERROR";
const char CODE_NO_DIALTONE[] PROGMEM           = "NO DIALTONE";
const char CODE_BUSY[] PROGMEM                  = "BUSY";
const char CODE_NO_ANSWER[] PROGMEM             = "NO ANSWER";
const char CODE_PROCEEDING[] PROGMEM            = "PROCEEDING";

#define SMS_FORMAT_PDU 0;
#define SMS_FORMAT_TEXT 1;

} // namespace Sim800Commands

#endif