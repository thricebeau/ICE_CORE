/*--Integrated Control Environment (ICE)-----------------------------------------------
This backbone system provides infrastructure for ICE systems including
LCD panel interface with menu system and function calls for user input,
Relay output controls, timing functions, and User Feedback.

Menu Setup:
	Monitor
	
	Config
		for: Relays[i]
			on/off
			temp
			time 0000 0000
			light
	Status


LT-ICE01-AVC	Integrated Control Environment
Copyright 2012 Lazarus Technologies LLC
Constructed by Beau-James Erion 20120528
All Rights Reserved
------------------------------------------------------------------------------------------------------*/

//	Library Inclusions
#include <LiquidCrystal.h>
#include <LCDKeypad.h>
#include <MenuBackend.h>
#include <EEPROM.h>
#include <string.h>
#include <ctype.h>

//	Define the Hard Pin Settings
#define anl1	8		// Analog 1 - Analog pin 6: Light Sensor
#define anl2	9		// Analog 2 - Analog pin 7: Temp Sensor
#define anl3	10	// Analog 3 - Analog pin 0: pot input 1
#define anl4	11	// Analog 4 - Analog pin 8: pot input 2
#define rel1	31	// Relay 1
#define rel2	32	// Relay 2
#define rel3	33	// Relay 3
#define rel4	34	// Relay 4
#define led1	35	// Led indicator 1
#define led2	36	// Led indicator 2
#define led3	37	// Led indicator 3
#define led4	38	// Led indicator 4

//	Setup Global Variables
int runMode[4];										// Operation Mode for each Relay
int Relays[4] = ( rel1, rel2, rel3, rel4 );		// Relay Status Trigger
int Analog[3] = ( anl1, anl2, anl3, anl4 );	// Analog array for I/O
int LED[4] = ( led1, led2, led3, led4 );		// LED status Indicator

int tempLow[4];							// Temp switches at low
int tempEnd[4];							// Temp switches at high

int timeLow[4];							// Time switch on
int timeEnd[4];							// Time switch off

int valueLight = analogRead(anl1);	// Light reading
int valueTemp = analogRead(anl2);	// Temp reading

int pot1 = analogRead(anl3);		// Pot 1 input
int pot2 = analogRead(anl4);		// Pot 2 input

LCDKeypad lcd;						// Inialize the LCD Keypad


void setup() {		// Display the Splash Screen; Configure pins
	lcd.begin(16, 2); lcd.clear();
	lcd.print("  (LT) Lazarus  ");
	lcd.setCursor(0,1);
	lcd.print("  Technologies  ");
	delay(1000); lcd.clear(); lcd.setCursor(0,0);
	lcd.print("   ICE System   ");
	lcd.setCursor(0,1);
	lcd.print("  Loader  v1.0  ");
	delay(1000); lcd.clear(); lcd.setCursor(0,0);
	lcd.print("Display Test:");
	
	//	Setup the Pin Modes
	for (int i=0; i<4; i++) { pinMode (Relays[i], OUTPUT); }		// Configure Relays as Out
	for (int i=0; i<3; i++) { pinMode (Analog[i], INPUT ); }			// Configure Analog as In
}

void loop() {		// Main function operation

	for (int i=1; i<5; i++) {		// Check operation mode for each relay and process
		switch (runMode[i]){
			case 1:					// On/Off Mode
				
				break;
				
			case 2:					// Temp Mode
				if (valueTemp > tempEnd[i]) { digitalWrite(Relays[i], HIGH; }
				else { Relays[i] = LOW; digitalWrite( Relays[i + 30], LOW ); }		// ! need to switch relay off at low
				break;
				
			case 3:					// Time Mode
				// Requires clock module
				break;
				
			case 4:					// Light Mode
				if (valueLight < 1000) { Relays[i] = LOW; }		// Read light sensor
				else  { Relays[i] = HIGH; }
				break;
				
			default:
				break;
		}
	}

	
	for (int i=0; i<4; i++) {		// Adjust Relay LED status
		if (Relays[i] == LOW) { digitalWrite(led[i], LOW; }
		else { digitalWrite(led[i], HIGH); }
	}
	
	

	//	Adjust the switching speed for the relays to slow down
	TCCR0B = TCCR0B & 0b11111101;			// Pins 5+6 61Hz 
	TCCR1B = TCCR1B & 0b11111101;			// Pins 9+10 30Hz
	TCCR2B = TCCR2B & 0b11111111;			// Pins 11+3 30Hz (stuck at 490Hz)
}

/*--Function Calls-----------------------------------------------------------*/


int waitButton() {						// Wait for button Press
  int buttonPressed;
  buttonPressed = lcd.button();
  delay(50);
  waitReleaseButton;
  return buttonPressed;
}
void waitReleaseButton() {				// Wait for button Release
  while(lcd.button() != KEYPAD_NONE) {}
  delay(50);
}

void toggleRelay(int i) {				// Toggle Relay
	if (Relays[i] == LOW) {
		Relays[i] = HIGH;
		digitalWrite( 30 + i, HIGH );
	}
	else {
		Relays[i] = LOW;
		digitalWrite( 30+i, LOW );
	}
}