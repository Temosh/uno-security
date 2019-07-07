#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Logger.h>

#include "UnoSecurity.h"
#include "GsmModule.h"
#include "Phone.h"
#include "Keys.h"

const char *LOG_TAG = "UnoSecurity";

const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLUMNS = 4;

char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLUMNS] = {
	{KEY_1, KEY_2, KEY_3, KEY_A},
	{KEY_4, KEY_5, KEY_6, KEY_B},
	{KEY_7, KEY_8, KEY_9, KEY_C},
	{KEY_ASTERISK, KEY_0, KEY_SHARP, KEY_D},
};

byte KEYPAD_ROW_PINS[KEYPAD_ROWS] = {11, 10, 9, 8};
byte KEYPAD_COLUMN_PINS[KEYPAD_COLUMNS] = {7, 6, 5, 4};

Keypad keypad(makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PINS, KEYPAD_COLUMN_PINS, KEYPAD_ROWS, KEYPAD_COLUMNS);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
GsmModule gsmModule(2, 3, 4800);
Phone *phone;

void setup()
{
	Serial.begin(9600);
	Logger::setLogLevel(Logger::NOTICE);
	Logger::notice(LOG_TAG, "Testing GSM SIM800L...");

	lcd.init();
	lcd.backlight();

	keypad.addEventListener(keypadEvent);

	phone = new Phone(gsmModule, lcd);
	gsmModule.addPhoneListener(phone);
}

void loop()
{
	distributeTickEvent();

	if (Serial.available())
	{
		String command = Serial.readString();
		Serial.println(">" + command);
		gsmModule.sendCommand(command);
	}
}

void distributeTickEvent()
{
	keypad.getKeys();
	gsmModule.check();
}

void keypadEvent(KeypadEvent key)
{
	if (keypad.isPressed(key))
	{
		phone->onKeyEvent(key);
	}
}