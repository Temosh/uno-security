#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "UnoSecurity.h"
#include "gsm/GsmModule.h"
#include "phone/Phone.h"
#include "Keys.h"
#include "Utils.h"

const char *LOG_TAG = "UnoSecurity";

const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLUMNS = 4;

char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLUMNS] = {
        {KEY_1,        KEY_2, KEY_3,     KEY_A},
        {KEY_4,        KEY_5, KEY_6,     KEY_B},
        {KEY_7,        KEY_8, KEY_9,     KEY_C},
        {KEY_ASTERISK, KEY_0, KEY_SHARP, KEY_D},
};

byte KEYPAD_ROW_PINS[KEYPAD_ROWS] = {11, 10, 9, 8};
byte KEYPAD_COLUMN_PINS[KEYPAD_COLUMNS] = {7, 6, 5, 4};

Keypad keypad(makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PINS, KEYPAD_COLUMN_PINS, KEYPAD_ROWS, KEYPAD_COLUMNS);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
GsmModule gsmModule(2, 3, 4800);
Phone phone(gsmModule, lcd);

void setup() {
    Serial.begin(9600);
    Serial.println(F("Testing GSM SIM800L..."));

    lcd.init();
    lcd.backlight();

    keypad.addEventListener(keypadEvent);
    gsmModule.addPhoneListener(&phone);
    gsmModule.init();
    phone.init();
}

void loop() {
    distributeTickEvent();

    if (Serial.available()) {
        char command[GSM_TASK_COMMAND_LENGTH + 1]{};
        size_t length = Serial.readBytesUntil('\n', command, GSM_TASK_COMMAND_LENGTH);
        command[length] = '\0';
        trim(command);
        if (strlen(command) > 0) {
            gsmModule.sendCommand(command);
        }
    }
}

void distributeTickEvent() {
    keypad.getKeys();
    gsmModule.onTick();
}

void keypadEvent(KeypadEvent key) {
    if (keypad.isPressed(key)) {
        phone.onKeyEvent(key);
    }
}