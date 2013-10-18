#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <PID_v1.h>
#include <MAX31855.h>
#include <WString.h>

// Setup PID
double tTemp=50, cTemp=50, output=50;
double kp = 2, ki = 0.5, kd = 2;
PID myPID(&cTemp, &output, &tTemp,kp,ki,kd, DIRECT);


SoftwareSerial btSerial(2,11);
LiquidCrystal lcd(A1, A0, 4, 7, 8, 9);
const byte RelayPin = 5;
const byte SSRPin = 6;


// Setup Thermocouple
const byte thermocoupleCS = 10;
const byte thermocoupleSO = 12;
const byte thermocoupleCLK = 13;
MAX31855 thermocouple(thermocoupleCLK, thermocoupleCS, thermocoupleSO);

// Operational Setup
int mode;					// operational mode
int curve;					// curve aggression
double rampPoint=80.0;		// percent of setpoint to activate PID

unsigned long cycleGate;	// ms of gate width
unsigned long cycleStart;	// time the current gate it at

unsigned long coolCycle;	// Hours of cool cycle
unsigned long coolStart;	// Time the cool cycle started


// Timers
unsigned long now;			// Current time
unsigned long pidTime;		// Timer for the PID
unsigned long lcdTime;		// Timer for LCD Refresh
unsigned long cTime;		// Current time
unsigned long pTime;		// Previous time
unsigned long reportTime;	// ms increment for sending bt update

// variables







void setup(){
  Serial.begin(9600);      // Start default comm
  btSerial.begin(9600);    // Start Bluetooth comm
  lcd.begin(8,2);          // Start lcd screen
  lcd.setCursor(0,0);
  lcd.print("START");
  
  
  cycleGate = 5000;				// 5 second cycles
  cycleStart = millis();	// start the first window

}


void loop(){
	
	

	if(btSerial.read() == '$'){				// Check for token
		int inByte = btSerial.read();		// Grab mode character
		
		// Read serial information depending on the mode
		switch(inByte){
		  
			case '0':		// Mode 0
				break;
				
			case '1':		// Mode 1
				
				
				break;
			
			case '2':		// Mode 2
				break;
			
			case '3':		// Mode 3
				break;
			
			default:
				btSerial.print("$ERROR*");
				break;
		} //Switch
	} // Serial read

	
	String word = "123";
	int number = word.toInt();
	
	String Weight = "1.23";
	char buf[Weight.length()];
	Weight.toCharArray(buf,Weight.length());
	float WVal=atof(buf);
	
	
	now = millis();	// update the present
	if(now-cycleStart > cycleGate){		// If at the next cycle
		cycleStart+=cycleGate;			// Shift the cycle start
		myPID.Compute();				// compute the PID for next cycle
	}

	// PID processing Block
	switch(mode){
		
		case 0:			// Mode 0: OFF
			break;
		
		case 1:			// Mode 1: RAMP
			switch(curve){
				case 0:			// Aggressive curve
					digitalWrite(RelayPin,HIGH);
					break;
					
				case 1:			// Conservative curve
					
					if(cTemp > tTemp*rampPoint){		// If we're over the rampPoint%, act as PID
						if(output < now-cycleStart) digitalWrite(RelayPin,HIGH);
						else digitalWrite(RelayPin,LOW);
						
					} else digitalWrite(RelayPin,HIGH);	// Otherwise, ramp full on
					break;
			}
			if(tTemp-cTemp < 30) mode=2;	// If we're within 30 degrees of target, switch to hold mode
			break;
		
		
		case 2:			// Mode 2: HOLD
			// Power on until we hit the output cycle point, then turn off until the next cycle
			if(output < now-cycleStart) digitalWrite(RelayPin,HIGH);
			else digitalWrite(RelayPin,LOW);
			break;
		
		
		case 3:			// Mode 3: COOL
			// TODO: Insert some jazz relating the cooling time domain with some sine function
			break;
		
		
		default: // Something went wrong with the mode, report it or something
			break;
		
	} // PID Mode Switch
	
	
	
	// Reporting block
	


  
}