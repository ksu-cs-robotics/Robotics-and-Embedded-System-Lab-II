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
*           Arduino Nano has 2 external interrupt pins (2, 3   )
*           Arduino Leonardo has 4 external interrupt pins ( 0, 1, 2, 3  7 )
*           Arduino Mega has 6 iexternal nterrupt pins ( 2, 3, 18, 19, 20, 21)
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
unsigned long jTime = 0;     // joystic control message interval
unsigned long dTime = 0;     // OLED display update interval

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  // Serial Port Setup
  Serial.begin(115200);   Serial3.begin(115200);
  Serial.println("Remote Controller Initialization Started");
  initConfig();

 
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
  updateJoy();
  checkMessage();
  updateDisplay();

}

void initConfig(){
  pinMode(PIN_JOY_X, INPUT); pinMode(PIN_JOY_Y, INPUT);
  pinMode(PIN_JOY_SW_1, INPUT_PULLUP); pinMode(PIN_JOY_SW_2, INPUT_PULLUP); pinMode(PIN_JOY_SW_3, INPUT_PULLUP);
  pinMode(PIN_JOY_SW_UP, INPUT_PULLUP); pinMode(PIN_JOY_SW_LEFT, INPUT_PULLUP); pinMode(PIN_JOY_SW_RIGHT, INPUT_PULLUP);  pinMode(PIN_JOY_SW_DOWN, INPUT_PULLUP);
  for(int i = 0; i < SCREEN_WIDTH; i++){
    for(int j = 0; j < SCREEN_HEIGHT; j++){
      ogmap[i][j] = false;
    }  
  } 
}

void checkMessage(){
  checkSerialMessage(&Serial, msgBufferPointer_Serial0, &msgBufferPointer_Serial0);  // Mega (Controller) <--> PC
  checkSerialMessage(&Serial3, msgBufferPointer_Serial3, &msgBufferPointer_Serial3); // Mega (Controller) <--> Bluetooth <--> Nano (Robot)
}

void updateJoy(){
  if (cTime > jTime) {  // Joystick control time
    jTime = cTime + INTERVAL_JOYSTICK; 
    joystick[0] = map(analogRead(PIN_JOY_X), 0, 1023, -255, 255);
    joystick[1] = map(analogRead(PIN_JOY_Y), 0, 1023, -255, 255);
    String joy[2] = {String(joystick[0]), String(joystick[1])};

//    Button Function Checking 
//    Serial.print(digitalRead(PIN_JOY_SW_1)); Serial.print(",");
//    Serial.print(digitalRead(PIN_JOY_SW_2)); Serial.print(",");
//    Serial.print(digitalRead(PIN_JOY_SW_3)); Serial.print(",");
//    Serial.print(digitalRead(PIN_JOY_SW_UP)); Serial.print(",");
//    Serial.print(digitalRead(PIN_JOY_SW_LEFT)); Serial.print(",");
//    Serial.print(digitalRead(PIN_JOY_SW_RIGHT)); Serial.print(",");
//    Serial.println(digitalRead(PIN_JOY_SW_DOWN));    

//   sendCommand(&Serial, joy, 2, "c");       
    sendCommand(&Serial3, joy, 2, "c");       
  }  
}

void updateDisplay(){
  if ( cTime > dTime) {
    dTime = micros() +  INTERVAL_DISCOVERY_UPDATE;    
    display.clearDisplay();       //reflesh oled screan
    // Draw a single pixel in white
//    display.drawPixel((int)pose[0], (int)pose[1], SSD1306_WHITE);

    display.setTextSize(1); // Draw 1X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Joystick= "); display.print(joystick[0]); display.print(" : ");       
    display.println(joystick[1]);    // Show the display buffer on the screen. You MUST call display() after
    display.println("----------------"); 
    display.print("MODE BTN = "); 
    display.print(digitalRead(PIN_JOY_SW_1)); display.print(" : ");
    display.print(digitalRead(PIN_JOY_SW_2)); display.print(" : ");
    display.println(digitalRead(PIN_JOY_SW_3)); 
    display.print("CNT = "); 
    display.print(digitalRead(PIN_JOY_SW_UP)); display.print(" : ");
    display.print(digitalRead(PIN_JOY_SW_LEFT)); display.print(" : ");
    display.print(digitalRead(PIN_JOY_SW_RIGHT)); display.print(" : ");
    display.println(digitalRead(PIN_JOY_SW_DOWN)); 
    
    // drawing commands to make them visible on screen!
    display.display();   
  }
}
