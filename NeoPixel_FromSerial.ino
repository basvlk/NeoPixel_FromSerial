/* 
 This sketch is to define a way of dealing with Serial comms, 
 - Startbyte is 255. All data that does not start with 255 is discarded.
 - Data is read until the buffersize is reached, the serial times out (1 sec) or the stopbyte is encountered
 - Stopbyte is 254. 
 - Data is read into an array of Bytes.
 
 - a few blinks to show the program has started (at the end of 'setup')
 - "[" (ASCII 91) FROM Arduino is the character for debugging information. Any comments or debugging information start with '[' 
 - "]" (ASCII 93) FROM Arduino is the 'bytes in buffer' ie. when zero, it means the Arduino is ready for new commands
 - Feedback to the main system can be switched on and off with a constant: Diagnostic = 0 or 1
 
 NEXT STEPS: use Serial.Find instead of evaluation serial.read for '255' every cycle
 2nd byte is a MODE byte which triggers cases
 load 5 different arrays with loading-modes, and trigger them quickly witch trigger modes
 */

//LED stuff
#include "LPD8806.h"
#include "SPI.h"

const  int nLEDs = 24; // Number of RGB LEDs in strand:
int dataPin  = 2; //green wire
int clockPin = 3; //yellow wire
int i;

LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// Basic stuff
const int ArduinoLedPin =  13;      // the number of the LED pin
boolean Diagnostic = 1;

// Serial comms stuff
byte inByte = 0; //inByte is constantly read and evaluated for value '255' If its not 255, it is discarded and the next byte is read
int BytesInBuffer = 0;
int Mode = 0;

byte STATE0[nLEDs*3];
byte STATE1[nLEDs*3];
byte STATE2[nLEDs*3];
byte STATE3[nLEDs*3];
byte STATE4[nLEDs*3];


byte RED[nLEDs*3] = {
  255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, 255, 0, 0, };
byte BLUE[nLEDs*3] = {
  0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,0, 0, 127,};



void setup() {
  pinMode(ArduinoLedPin, OUTPUT);
  Serial.begin(9600); 

  // LED stuff
  strip.begin(); // Start up the LED strip
  strip.show();  // Update the strip, to start they are all 'off'

  //WakeUp
  Serial.println("Hello.");
  // 4 blinks to show Arduino is starting
  digitalWrite(ArduinoLedPin, HIGH);
  strip.setPixelColor(0, strip.Color(20,0,0));
  strip.show();
  delay(200);
  digitalWrite(ArduinoLedPin, LOW);
  strip.setPixelColor(1, strip.Color(0,20,0));
  strip.show();
  delay(100);  
  digitalWrite(ArduinoLedPin, HIGH);
  strip.setPixelColor(2, strip.Color(0,0,20));
  strip.show();
  delay(200);
  digitalWrite(ArduinoLedPin, LOW);
  strip.setPixelColor(3, strip.Color(0,20,20));
  strip.show();
  delay(100);
  digitalWrite(ArduinoLedPin, HIGH);
  strip.setPixelColor(0, strip.Color(0,0,0));
  strip.show();
  delay(200);
  digitalWrite(ArduinoLedPin, LOW);
  strip.setPixelColor(1, strip.Color(0,0,0));
  strip.show();
  delay(100);
  digitalWrite(ArduinoLedPin, HIGH);
  strip.setPixelColor(2, strip.Color(0,0,0));
  strip.setPixelColor(3, strip.Color(0,0,0));
  strip.show();
  delay(500);
  digitalWrite(ArduinoLedPin, LOW);  

  if (Diagnostic) {
    Serial.println("[ Diagnostic ON"); 
  } 
  else {
    Serial.println("[ Diagnostic OFF"); 
  };
}


///////////////////////////////////////////////////////////

void loop() 
{
  /// Reading incoming serial data into an Array starts here

  BytesInBuffer =  Serial.available(); 
  if (Diagnostic) {
    Serial.print("[ Bytes in Buffer: "); 
    Serial.println(BytesInBuffer); 
  }

  Serial.print("]");
  Serial.println(BytesInBuffer);

  if (BytesInBuffer > 0) 
  {
    if (Diagnostic) {
      Serial.print("[ Bytes in Buffer: "); 
      Serial.println(BytesInBuffer); 
    }
    inByte = Serial.read();

    if (inByte == 255)
    {
      if (Diagnostic) {
        while (!Serial.available()) {
        }; 
        Mode = Serial.read();
        Serial.print("Mode=");
        Serial.println(Mode);
      }
      switch (Mode) {
      case 0:

        Serial.readBytesUntil(254, (char *)STATE0, nLEDs*3);
        for (i=0; i < nLEDs; i++) {
          strip.setPixelColor(i, strip.Color((STATE0[i*3])/2,(STATE0[i*3+1])/2,(STATE0[i*3+2])/2));
          strip.show();
        }
        break;
      case 1:

        Serial.readBytesUntil(254, (char *)STATE1, nLEDs*3);
        for (i=0; i < nLEDs; i++) {
          strip.setPixelColor(i, strip.Color((STATE1[i*3])/2,(STATE1[i*3+1])/2,(STATE1[i*3+2])/2));
          strip.show();
        }
        break;

      case 10:
        for (i=0; i < nLEDs; i++) {
          strip.setPixelColor(i, strip.Color((STATE0[i*3])/2,(STATE0[i*3+1])/2,(STATE0[i*3+2])/2));
          strip.show();
        }
        break;

      case 11:
        for (i=0; i < nLEDs; i++) {
          strip.setPixelColor(i, strip.Color((STATE1[i*3])/2,(STATE1[i*3+1])/2,(STATE1[i*3+2])/2));
          strip.show();
        }
        break;
      }
    }
  }

  ///// print array to serial port for verification

  /*
  Serial.print("A");
   for (int i=0; i< nLEDs*3 ; i++)
   {
   
   Serial.print(" ");
   Serial.print(STATE0[i], DEC);
   Serial.print(" ");
   }    
   Serial.println("");
   
   Serial.print("B");
   for (int i=0; i< nLEDs*3 ; i++)
   {
   
   Serial.print(" ");
   Serial.print(STATE1[i], DEC);
   Serial.print(" ");
   }    
   Serial.println("");
   */
}

