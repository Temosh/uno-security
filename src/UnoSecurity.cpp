#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Logger.h>

#include "UnoSecurity.h"
#include "GsmModule.h"
#include "Phone.h"

const char *LOG_TAG = "UnoSecurity";

const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLUMNS = 4;

char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLUMNS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'},
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