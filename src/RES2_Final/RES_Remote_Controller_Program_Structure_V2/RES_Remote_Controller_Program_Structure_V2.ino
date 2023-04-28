/**********************************************************************;
* Project           : RES_Class, Basic Function Check, Sensor, Motion,Comm 
*
* Program name      : RES_Basic_Function_Check.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 04/28/2023
*
* Purpose           : Inititalization of a Simple Rescue Robot 
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/
/*********************************************************************
*   Instructional Note:  
*           Arduino Leonardo has 4 interrupt pins ( 0, 1, 2, 3  13 )
*           Arduino Mega has 6 interrupt pins ( 2, 3, 18, 19, 20, 21)
*                      
***********************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Protocol.h"
#include "Config.h"



int joystick[3] = {0,0,0};    // joystick status
int joySWFlag = 0;            // push button flag
unsigned long cTime = 0;     // current time of each iteration 
unsigned long jTime = 0;     // joystic test interval
unsigned long mTime = 0;     // message check interval
unsigned long lTime = 0;     // LED update interval
unsigned long dTime = 0;     // OLED display update interval

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  // Serial Port Setup
  Serial.begin(115200);   Serial3.begin(115200);
  Serial.println("Remote Controller Initialization Started");

  pinMode(PIN_JOY_X, INPUT); pinMode(PIN_JOY_Y, INPUT);
  pinMode(PIN_JOY_SW, INPUT);   digitalWrite(PIN_JOY_SW, HIGH);                  // or pinMode(PIN_JOY_SW, INPUT_PULLUP); 
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(1000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
}



void loop() {
  cTime = micros();     // get current time for this iteration
  if (cTime > jTime) {  // Joystick control time
    jTime = cTime + INTERVAL_JOYSTICK; 
    joystick[0] = map(analogRead(PIN_JOY_X), 0, 1023, -255, 255);
    joystick[1] = map(analogRead(PIN_JOY_Y), 0, 1023, -255, 255);
    int tmpJoySW = digitalRead(PIN_JOY_SW);

    /////////////////////////////////////////////////////////////////////////////
    //Using joySWFlag, you should make toggle button and update value on joystick[2] 
    //////////////////////////////////////////////////////////////////////////
        
    Serial.print("Joy "); Serial.print(joystick[0]); Serial.print(" "); Serial.print(joystick[1]); Serial.print(" "); Serial.println(joystick[2]);     
    Serial3.print("Joy "); Serial3.print(joystick[0]); Serial3.print(" "); Serial3.print(joystick[1]); Serial3.print(" "); Serial3.println(joystick[2]);     
  }

  checkSerialMessage(&Serial, msgBufferPointer_Serial0, &msgBufferPointer_Serial0);
  
  if (cTime > dTime) {  // Joystick control time
    dTime = micros() + INTERVAL_DISCOVERY_UPDATE;
    displayTest();
  }
}


void displayTest(){
//  // Clear the buffer
//  display.clearDisplay();
//  // Draw a single pixel in white
//  display.drawPixel(10, 10, SSD1306_WHITE);
//  delay(1000); // Pause for 2 seconds
  
  display.clearDisplay();
  display.setTextSize(1); // Draw 1X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  String tmpStr = (String)joystick[0];
  display.println(tmpStr);
  display.display();      // Show initial text  
}
