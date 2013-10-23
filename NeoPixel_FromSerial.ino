  /* 
   This sketch is and adaptation of "LED String Control from Serial"  - adpated for Neopixel. 
   Most commands are the same, the main difference is the setup part and the libraries called.
   THe idea is to define a way of dealing with Serial comms, 
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
  
  // Adafruit neopixel stuff
  #include <Adafruit_NeoPixel.h>
  const int nLEDs = 8; // Number of RGB LEDs in strand:
  int dataPin  = 2; //green wire
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(nLEDs, dataPin, NEO_GRB + NEO_KHZ800);
  
  // Basic stuff
  const int ArduinoLedPin =  13;      // the number of the LED pin
  boolean Diagnostic = 1;
  int i;
  
  // Serial comms stuff
  byte inByte = 0; //inByte is constantly read and evaluated for value '255' If its not 255, it is discarded and the next byte is read
  int BytesInBuffer = 0;
  int Mode = 0;
  
  // Full System Descriptions
  byte STATE0[nLEDs*3];
  byte STATE1[nLEDs*3];
  byte STATE2[nLEDs*3];
  byte STATE3[nLEDs*3];
  byte STATE4[nLEDs*3];
  
  // Bytes for other modes
  byte RedByte = 0;
  byte GreenByte = 0;
  byte BlueByte = 0;
  int Chasecount = 0;
  int TimerByte = 0;
  
  
  void setup() {
    pinMode(ArduinoLedPin, OUTPUT);
    Serial.begin(9600); 
  
    // LED stuff
    strip.begin(); // Start up the LED strip
    strip.show();  // Update the strip, to start they are all 'off'
  
    //WakeUp
    Wakeup(); 
  
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
        case 10: //Read Bytes into array STATE0
  
          Serial.readBytesUntil(254, (char *)STATE0, nLEDs*3);
          for (i=0; i < nLEDs; i++) {
            strip.setPixelColor(i, strip.Color((STATE0[i*3])/2,(STATE0[i*3+1])/2,(STATE0[i*3+2])/2));
            strip.show();
          }
          break;
        case 11: //Read Bytes into array STATE1
  
          Serial.readBytesUntil(254, (char *)STATE1, nLEDs*3);
          for (i=0; i < nLEDs; i++) {
            strip.setPixelColor(i, strip.Color((STATE1[i*3])/2,(STATE1[i*3+1])/2,(STATE1[i*3+2])/2));
            strip.show();
          }
          break;
  
        case 20: //Update LEDs with ARRAY0
          for (i=0; i < nLEDs; i++) {
            strip.setPixelColor(i, strip.Color((STATE0[i*3])/2,(STATE0[i*3+1])/2,(STATE0[i*3+2])/2));
            strip.show();
          }
          break;
  
        case 21://Update LEDs with ARRAY1
          for (i=0; i < nLEDs; i++) {
            strip.setPixelColor(i, strip.Color((STATE1[i*3])/2,(STATE1[i*3+1])/2,(STATE1[i*3+2])/2));
            strip.show();
          }
          break;
         case 0: //All LEDS OFF
          {for(int i=0; i<nLEDs; i++) {
        strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
      }
      strip.show();              // Refresh LED states
      break;
          
          }
          
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
  
  void Wakeup()
  {
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
  }
  

