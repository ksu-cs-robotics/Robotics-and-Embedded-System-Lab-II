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
#include <SoftwareSerial.h>
#include "Protocol.h"
#include "Config.h"



unsigned long cTime = 0;     // current time of each iteration 
unsigned long jTime = 0;     // joystic control message interval
unsigned long dTime = 0;     // OLED display update interval
unsigned long mTime = 0;     // motor control update interval
unsigned long pTime = 0;     // Pose update interval
unsigned long rTime = 0;     // report interval

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SoftwareSerial mySerial(A2,A3);

void setup() {
  // Serial Port Setup
  Serial.begin(115200);   //Serial3.begin(115200);
  mySerial.begin(115200);
  Serial.println("Robot Initialization Started");
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
  updatePose();
  updateDisplay();
  updateMotorSpeed();
  sendReport();

}

void initVariable(){
  pinMode(PIN_MOTOR_PWM_RIGHT, OUTPUT); pinMode(PIN_MOTOR_IN0_RIGHT, OUTPUT); pinMode(PIN_MOTOR_IN1_RIGHT, OUTPUT); 
  pinMode(PIN_MOTOR_PWM_LEFT, OUTPUT); pinMode(PIN_MOTOR_IN0_LEFT, OUTPUT); pinMode(PIN_MOTOR_IN1_LEFT, OUTPUT); 
  for(int i = 0; i < SCREEN_WIDTH; i++){
    for(int j = 0; j < SCREEN_HEIGHT; j++){
      ogmap[i][j] = false;
    }  
  }
  
}

void checkMessage(){
  checkSerialMessage(&Serial, msgBuffer_Serial0, &msgBufferPointer_Serial0);  // Mega (Controller) <--> PC
//  checkSerialMessage(&Serial3, msgBufferPointer_Serial3, &msgBufferPointer_Serial3); // Mega (Controller) <--> Bluetooth <--> Nano (Robot)
  checkSerialMessage(&mySerial, msgBuffer_mySerial, &msgBufferPointer_mySerial); // Mega (Controller) <--> Bluetooth <--> Nano (Robot)
}

void updatePose(){
  if (cTime > pTime) {  // Update Pose
    pTime = cTime + INTERVAL_POSE; 
  }
}

void sendReport(){
  if (cTime > rTime) {  // send message to controller
    rTime = cTime + INTERVAL_REPORT; 
    String pose_data[3] = {String(pose[0]), String(pose[1]), String(pose[2])};    
    String cmd_vel_data[3] = {String(cmd_vel[0]), String(cmd_vel[1])};    
//    sendCommand(&Serial,cmd_vel_data , 2, "c");       
//    sendCommand(&mySerial, cmd_vel_data, 2, "c");       
//    sendCommand(&Serial,pose_data , 3, "p");       
    sendCommand(&mySerial, pose_data, 3, "p");       

  }
}


void updateJoy(){
  if (cTime > jTime) {  // Joystick control time
    jTime = cTime + INTERVAL_JOYSTICK; 
    cmd_vel[0] = joystick[1]; 
    cmd_vel[1] = joystick[0];

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


void updateMotorSpeed(){
  if ( cTime > mTime) {
    mTime = cTime +  INTERVAL_MOTOR_SPEED;  

    if (cmd_vel[0] > 0){ //linear 
       motor_speed[LEFT] = cmd_vel[0] - cmd_vel[1];
       motor_speed[RIGHT] = cmd_vel[0]+ cmd_vel[1];               
    }else{
       motor_speed[LEFT] = cmd_vel[0] + cmd_vel[1];
       motor_speed[RIGHT] = cmd_vel[0]- cmd_vel[1];                     
    }

    if (motor_speed[RIGHT] > 255) { 
      motor_speed[RIGHT] = 255;
    }else if (motor_speed[RIGHT] < -255) {
      motor_speed[RIGHT] = -255;
    }
    if (motor_speed[LEFT] > 255) { 
      motor_speed[LEFT] = 255;
    }else if (motor_speed[LEFT] < -255) {
      motor_speed[LEFT] = -255;
    }

    analogWrite( PIN_MOTOR_PWM_RIGHT, abs( motor_speed[RIGHT]));
    analogWrite( PIN_MOTOR_PWM_LEFT, abs(motor_speed[LEFT]));
    
    if (motor_speed[RIGHT] > 0){  //linear
      digitalWrite( PIN_MOTOR_IN0_RIGHT, LOW); digitalWrite( PIN_MOTOR_IN1_RIGHT, HIGH);
    }else{
      digitalWrite( PIN_MOTOR_IN0_RIGHT, HIGH); digitalWrite( PIN_MOTOR_IN1_RIGHT, LOW);
    }
    if (motor_speed[LEFT] > 0 ){
      digitalWrite( PIN_MOTOR_IN0_LEFT, LOW); digitalWrite( PIN_MOTOR_IN1_LEFT, HIGH);    
    }else{
      digitalWrite( PIN_MOTOR_IN0_LEFT, HIGH); digitalWrite( PIN_MOTOR_IN1_LEFT, LOW);      
    }    
  }
}
