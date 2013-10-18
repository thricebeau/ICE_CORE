#include <SoftwareSerial.h>

int RX = 10;
int TX = 11;
SoftwareSerial btSerial(RX,TX);

char mode = '0';
unsigned long cycle = 100;  // Cycle in ms
unsigned long timer;        // Current cycle time
unsigned long now;          // Current time

void setup(){
  Serial.begin(9600);
  btSerial.begin(9600);
  Serial.println("Polling at " + cycle);
} // Setup


void loop(){
  
  // Grab bt serial feed and change mode
  if(btSerial.read() == '$'){
    delay(1);
    char inByte = btSerial.read();
    mode = inByte;
    Serial.print("Mode Changed to: ");
    Serial.println(mode);
  }
  
  
  now = millis();
  if(now-timer > cycle){  // Check and increment cycle timer
    timer += cycle;
    
    switch(mode){
      case '0':  // Off mode
        btSerial.println("$0*");
        break;
      
      case '1':  // Ramp mode
        btSerial.print("$");
        btSerial.print(millis()%1000);
        btSerial.println("*");
        break;
      
      case '2':  // On mode
        btSerial.println("$1000*");
        break;
      
      case '3':  // Cool mode
        btSerial.print("$");
        btSerial.print(1000.0-(millis()%1000));
        btSerial.println("*");
        break;
    
    } // Switch mode
  } // If at cycle point
  
  
  
} // Loop
