#include <Arduino.h>
#include <LiquidCrystal.h>

const int pinLogicMain = 8;
const int pinLogicSecondary = 9;
const int pinNextGate = 10;
const int pinEnter = 11;
const int pinLed = 12;

int stateLogicMain = 0;
int stateLogicSecondary = 0;
int stateNextGate = 0;
int stateEnter = 0;
int stateLed = 0;

int logicModeEnabled = 0;

#define GATES_SIZE 7
const char* gates[GATES_SIZE] = {"AND", "OR", "NOT", "NAND", "NOR", "XOR", "XNOR"};
int gateSelected = -1;

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
	Serial.begin(9600);

	pinMode(pinLogicMain, INPUT);
	pinMode(pinLogicSecondary, INPUT);
	pinMode(pinNextGate, INPUT);
	pinMode(pinEnter, INPUT);
	pinMode(pinLed, OUTPUT);
	
	lcd.begin(16, 2);
	lcd.print("Select gate");
}

void ledControl(int enabled)
{
	if (enabled) {
		digitalWrite(pinLed, HIGH);
	} else {
		digitalWrite(pinLed, LOW);
	}
}

void loop()
{
	stateLogicMain = digitalRead(pinLogicMain);
	stateLogicSecondary = digitalRead(pinLogicSecondary);
	stateNextGate = digitalRead(pinNextGate);
	stateEnter = digitalRead(pinEnter);
	stateLed = digitalRead(pinLed);

	if (stateNextGate == HIGH) {
		logicModeEnabled = 0;
		
		if (gateSelected + 1 < GATES_SIZE) {
			gateSelected++;
		} else {
			gateSelected = 0;
		}
		
		lcd.clear();

		char msg[16];
		sprintf(msg, "%i -> %s", gateSelected, gates[gateSelected]);
		lcd.setCursor(0, 0);
		lcd.print(msg);
		delay(500);
	}

	if (stateEnter == HIGH && logicModeEnabled == 0) {
		logicModeEnabled = 1;
	}

	if (logicModeEnabled == 1) {
		lcd.setCursor(0, 1);
		lcd.print("Enabled");
		lcd.setCursor(0, 0);

		switch (gateSelected) {
		case 0:
			ledControl(stateLogicMain && stateLogicSecondary);
			break;
		case 1:
			ledControl(stateLogicMain || stateLogicSecondary);
			break;
		case 2:
			ledControl(!stateLogicMain);
			break;
		case 3:
			ledControl(!(stateLogicMain && stateLogicSecondary));
			break;
		case 4:
			ledControl(!(stateLogicMain || stateLogicSecondary));
			break;
		case 5:
			ledControl(stateLogicMain != stateLogicSecondary);
			break;
		case 6:
			ledControl(stateLogicMain == stateLogicSecondary);
			break;
		}
	} else {
		digitalWrite(pinLed, LOW);
	}
}
