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



unsigned long cTime = 0;     // current time of each iteration 
unsigned long jTime = 0;     // joystic control message interval
unsigned long dTime = 0;     // OLED display update interval
unsigned long mTime = 0;     // OLED display update interval

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  // Serial Port Setup
  Serial.begin(115200);   Serial1.begin(57600);
  Serial.println("Remote Controller Initialization Started");
  initVariable();

 
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
//  updateMotorSpeed();

}

void initVariable(){
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
  checkSerialMessage(&Serial, msgBuffer_Serial0, &msgBufferPointer_Serial0);  // Mega (Controller) <--> PC
  checkSerialMessage(&Serial1, msgBuffer_Serial1, &msgBufferPointer_Serial1); // Mega (Controller) <--> Bluetooth <--> Nano (Robot)
}

void updateJoy(){
  if (cTime > jTime) {  // Joystick control time
    jTime = cTime + INTERVAL_JOYSTICK; 
    joystick[0] = map(analogRead(PIN_JOY_X), 0, 1023, -(MAX_MOTOR_SPEED), MAX_MOTOR_SPEED);
    joystick[1] = map(analogRead(PIN_JOY_Y), 0, 1023, -(MAX_MOTOR_SPEED), MAX_MOTOR_SPEED);
    String joy[2] = {String(joystick[0]), String(joystick[1])};
//    cmd_vel[0] = joystick[1]; 
//    cmd_vel[1] = joystick[0];


//    sendCommand(&Serial, joy, 2, "j");       
    sendCommand(&Serial1, joy, 2, "j");       
  }  
}

void updateDisplay(){
  if ( cTime > dTime) {
    dTime = cTime +  INTERVAL_DISCOVERY_UPDATE;    
    // Draw a single pixel in white
//    display.drawPixel((int)pose[0], (int)pose[1], SSD1306_WHITE);
    display.clearDisplay();
    display.setTextSize(1); // Draw 1X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print(joystick[0]); display.print(" : ");   
    display.println(joystick[1]);    
    display.print(cmd_vel[0]); display.print(" : ");   
    display.println(cmd_vel[1]);    
    display.print(motor_speed[0]); display.print(" : ");   
    display.println(motor_speed[1]);    
    display.print(pose[0]); display.print(" : ");   
    display.print(pose[1]); display.print(" : ");   
    display.println(pose[2]);    
    
    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();   
  }
}
