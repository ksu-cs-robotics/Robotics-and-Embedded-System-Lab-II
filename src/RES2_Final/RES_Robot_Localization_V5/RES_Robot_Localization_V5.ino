/**********************************************************************;
* Project           : RES_Class, Basic Function Check, Sensor, Motion,Comm 
*
* Program name      : RES_Basic_Function_Check.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 04/05/2020
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
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Arduino.h>
#include <Wire.h>
#include <MPU6050_light.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SimpleDeadReckoning.h"
#include "Protocol.h"
#include "Config.h"


/*
 *  Constructing MPU-6050
 */

long counterLeftWheel[2] = {0,0}; long counterRightWheel[2] = {0,0};



 
long cTime = 0;     // current time of each iteration 
long lTime = 0;     // localization interval
long mTime = 0;     // motor interval
long oTime = 0;     // oled interval
long rTime = 0;     // ridar interval
long iTime = 0;     // IMU interval
long dTime = 0;     // debug print interval

int ridarAngle = MID_RIDAR_ANGLE;
int ridarDirection = 0;
float ridarValue[MAX_RIDAR_ANGLE - MIN_RIDAR_ANGLE];

int prevLeftCLK, prevLeftDT, nowLeftCLK, nowLeftDT = 0;
int prevRightCLK, prevRightDT, nowRightCLK, nowRightDT = 0;

int encoderPin_X_left,  encoderPin_Y_left ,  encoderPin_X_right, encoderPin_Y_right  = 0;
long counter_right, counter_left   =0;

long lCounter, rCounter = 0;
float cTheta, xLocation, yLocation = 0;

float thetaOffset = 0.0;

MPU6050 mpu (Wire);
Servo myServo;  // create servo object to control a servo
SimpleDeadReckoning mySDR( 310.0, 1.8, 8.8, 0);   // encoder values per one rotation,  wheel radius, distance between two wheels, unit (cm)
SoftwareSerial mySerial(A2,A3);  //SoftwareSerial(rxPin[Bluetooth-TX], txPin[Bluetooth-RX])
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void initVariable();

void setup() {
  // Serial Port Setup
  Serial.begin(115200);
  Serial.println("Robot-USB");
  mySerial.begin(115200);
  mySerial.println("Robot-BL");

  initVariable(); 
  
//  // IMU init

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  

  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
  
  thetaOffset= mpu.getAngleZ();

  mySDR.setXLocation(64.0);
  mySDR.setYLocation(16.0);

  // Motor Driver Setting
  pinMode(PIN_MOTOR_PWM_RIGHT,OUTPUT);pinMode(PIN_MOTOR_IN0_RIGHT,OUTPUT);pinMode(PIN_MOTOR_IN1_RIGHT,OUTPUT);
  pinMode(PIN_MOTOR_PWM_LEFT,OUTPUT);pinMode(PIN_MOTOR_IN0_LEFT,OUTPUT);pinMode(PIN_MOTOR_IN1_LEFT,OUTPUT);
  
  // Wheel Encoder Setting
  pinMode(PIN_WHEEL_ENCODER_CLK_LEFT, INPUT); pinMode(PIN_WHEEL_ENCODER_DT_LEFT, INPUT);
  pinMode(PIN_WHEEL_ENCODER_CLK_RIGHT, INPUT); pinMode(PIN_WHEEL_ENCODER_DT_RIGHT, INPUT);
 
  attachInterrupt(digitalPinToInterrupt(PIN_WHEEL_ENCODER_CLK_LEFT), updateEncoder_left, CHANGE);  
  attachInterrupt(digitalPinToInterrupt(PIN_WHEEL_ENCODER_CLK_RIGHT), updateEncoder_right, CHANGE);  

  myServo.attach(PIN_SERVO);  myServo.write(MID_RIDAR_ANGLE);// attaches the servo on PIN_SERVO to the servo object and set middle position
}



int cnt = 0;
void loop() {
  cTime = millis ();
  checkSerialMessage();
  
  mpu.update();  // IMU update
  cTheta = mpu.getAngleZ()- thetaOffset;
  mySDR.updateLocation(counter_left, counter_right, cTheta);

    String odom_temp[8] = {String(odom_pose[0]), String(odom_pose[1]), String(odom_pose[2]), String(odom_vel[0]), String(odom_vel[2])};
//    send_py(odom_temp, 5, ODOM_MSG);

  checkDebug();
  updateDisplay();
//  updateRidar();
//  updateMotorSpeed();
}





void updateEncoder_left(){
  encoderPin_X_left = digitalRead(PIN_WHEEL_ENCODER_CLK_LEFT);
  encoderPin_Y_left = digitalRead(PIN_WHEEL_ENCODER_DT_LEFT);

  if (encoderPin_X_left == 0){  
    if (encoderPin_Y_left == 0){
      counter_left++;
    }else{
      counter_left--;
    }
  }else{
    if (encoderPin_Y_left == 0){
      counter_left--;
    }else{
      counter_left++;
    }    
  }
}

void updateEncoder_right(){
  encoderPin_X_right = digitalRead(PIN_WHEEL_ENCODER_CLK_RIGHT);
  encoderPin_Y_right = digitalRead(PIN_WHEEL_ENCODER_DT_RIGHT);

  if (encoderPin_X_right == 0){  
    if (encoderPin_Y_right == 0){
      counter_right--;
    }else{
      counter_right++;
    }
  }else{
    if (encoderPin_Y_right == 0){
      counter_right++;
    }else{
      counter_right--;
    }    
  }
}






void checkDebug(){
  if ( cTime > dTime) {
    dTime = millis() +  INTERVAL_DEBUG;  
//    Serial.print(cmd_vel[0]);
//    Serial.print(" : ");
//    Serial.print(motorSpeed[0]);
//    Serial.print(" = ");
//    Serial.print(cmd_vel[1]);
//    Serial.print(" : ");
//    Serial.println(motorSpeed[1]);
    
//    Serial.print( digitalRead(PIN_WHEEL_ENCODER_CLK_LEFT));     Serial.print(" : ");
//    Serial.print( digitalRead(PIN_WHEEL_ENCODER_DT_LEFT));     Serial.print(" | ");
//    Serial.print( digitalRead(PIN_WHEEL_ENCODER_CLK_RIGHT));     Serial.print(" : ");
//    Serial.print( digitalRead(PIN_WHEEL_ENCODER_DT_RIGHT));     Serial.print(" === ");
//    Serial.print(encoderPin_X_right);  Serial.print("  ");   Serial.print(encoderPin_Y_right);   Serial.print("  ");
//    Serial.print(counter_right);   Serial.print(" | ");   Serial.print(encoderPin_X_left);   Serial.print("  ");
//    Serial.print(encoderPin_Y_left);   Serial.print("  ");   Serial.println(counter_left);   
  }
}

void updateDisplay(){
  if ( cTime > oTime) {
    oTime = millis() +  INTERVAL_OLED;    
//    Serial.print("path ");
//    Serial.print(mySDR.getXLocation());
//    Serial.print(" ");
//    Serial.println(mySDR.getYLocation());
  // Draw a single pixel in white
   display.drawPixel((int)mySDR.getXLocation(), (int)mySDR.getYLocation(), SSD1306_WHITE);
  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
    display.display();
    

  }
}


void  updateRidar(){
  if ( cTime > rTime) {
    rTime = millis() +  INTERVAL_RIDAR;  
    if (ridarDirection == 0){
      if (ridarAngle++ > MAX_RIDAR_ANGLE){
        ridarAngle = MAX_RIDAR_ANGLE;
        ridarDirection = 1;
      }
    }else {
      if (ridarAngle-- < MIN_RIDAR_ANGLE){
        ridarAngle = MIN_RIDAR_ANGLE;
        ridarDirection = 0;
      }
    }
    myServo.write(ridarAngle);

  }

}


void updateMotorSpeed(){
  if ( cTime > mTime) {
    mTime = millis() +  INTERVAL_MOTOR;  

    if (cmd_vel[1] > 0){ //angular
      motorSpeed[LEFT] = cmd_vel[0] - cmd_vel[1];
      motorSpeed[RIGHT] = cmd_vel[0] + cmd_vel[1];          
    }else{
      motorSpeed[LEFT] = cmd_vel[0] + cmd_vel[1];
      motorSpeed[RIGHT] = cmd_vel[0] - cmd_vel[1];          
    }
    if (motorSpeed[RIGHT] > 255) { 
      motorSpeed[RIGHT] = 255;
    }else if (motorSpeed[RIGHT] < -255) {
      motorSpeed[RIGHT] = -255;
    }
    if (motorSpeed[LEFT] > 255) { 
      motorSpeed[LEFT] = 255;
    }else if (motorSpeed[LEFT] < -255) {
      motorSpeed[LEFT] = -255;
    }
    analogWrite( PIN_MOTOR_PWM_RIGHT, abs( motorSpeed[RIGHT]));
    analogWrite( PIN_MOTOR_PWM_LEFT, abs(motorSpeed[LEFT]));
 //   analogWrite( PIN_MOTOR_PWM_RIGHT, 255);
 //   digitalWrite( PIN_MOTOR_IN0_RIGHT, HIGH); digitalWrite( PIN_MOTOR_IN1_RIGHT, LOW);
//    analogWrite( PIN_MOTOR_PWM_LEFT, 255);
//    digitalWrite( PIN_MOTOR_IN0_LEFT, HIGH); digitalWrite( PIN_MOTOR_IN1_LEFT, LOW);

    Serial.print(cmd_vel[0]);
    Serial.print(" : ");
    Serial.print(cmd_vel[1]);
    Serial.print(" = ");
    Serial.print(motorSpeed[0]);
    Serial.print(" : ");
    Serial.println(motorSpeed[1]);

    
    if (motorSpeed[RIGHT] > 0){  //linear
      digitalWrite( PIN_MOTOR_IN0_RIGHT, LOW); digitalWrite( PIN_MOTOR_IN1_RIGHT, HIGH);
    }else{
      digitalWrite( PIN_MOTOR_IN0_RIGHT, HIGH); digitalWrite( PIN_MOTOR_IN1_RIGHT, LOW);
    }
    if (motorSpeed[LEFT] > 0 ){
      digitalWrite( PIN_MOTOR_IN0_LEFT, LOW); digitalWrite( PIN_MOTOR_IN1_LEFT, HIGH);    
    }else{
      digitalWrite( PIN_MOTOR_IN0_LEFT, HIGH); digitalWrite( PIN_MOTOR_IN1_LEFT, LOW);      
    }

   
//    analogWrite( PIN_MOTOR_PWM_RIGHT, 255);
//    digitalWrite( PIN_MOTOR_IN0_RIGHT, HIGH); digitalWrite( PIN_MOTOR_IN1_RIGHT, LOW);
//    analogWrite( PIN_MOTOR_PWM_LEFT, 255);
//    digitalWrite( PIN_MOTOR_IN0_LEFT, HIGH); digitalWrite( PIN_MOTOR_IN1_LEFT, LOW);
  }
}


void initVariable(){
 for (int  i = 0; i < (MAX_RIDAR_ANGLE - MIN_RIDAR_ANGLE); i++){
    ridarValue[i] = 0.0;
 }
}
