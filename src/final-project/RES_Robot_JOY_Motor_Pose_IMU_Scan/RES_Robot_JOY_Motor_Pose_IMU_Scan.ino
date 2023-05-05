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
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MPU6050_light.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <VL53L0X.h>
#include "Protocol.h"
#include "Config.h"
#include "SimpleDeadReckoning.h"


unsigned long cTime = 0;     // current time of each iteration 
unsigned long jTime = 0;     // joystic control message interval
unsigned long dTime = 0;     // OLED display update interval
unsigned long mTime = 0;     // motor control update interval
unsigned long pTime = 0;     // Pose update interval
unsigned long rTime = 0;     // report interval

MPU6050 mpu (Wire);
VL53L0X sRidar;

//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SoftwareSerial mySerial(A2,A3);
Servo myServo;  // create servo object to control a servo
SimpleDeadReckoning mySDR( 310.0, 1.8, 8.8, 0);   // encoder values per one rotation,  wheel radius, distance between two wheels, unit (cm)

void setup() {
  // Serial Port Setup
  Serial.begin(115200);   //Serial3.begin(115200);
  mySerial.begin(57600);
  Serial.println("Robot Initialization Started");
  initVariable();
  
//  initScan();
  initIMU();
//  initOLED();

}



void loop() {
  cTime = micros();     // get current time for this iteration
  mpu.update();
  updateJoy();
  checkMessage();
  updatePose();
//  updateDisplay();
//  updateMotorSpeed();
  sendReport();

}

void initVariable(){
  pinMode(PIN_MOTOR_PWM_RIGHT, OUTPUT); pinMode(PIN_MOTOR_IN0_RIGHT, OUTPUT); pinMode(PIN_MOTOR_IN1_RIGHT, OUTPUT); 
  pinMode(PIN_MOTOR_PWM_LEFT, OUTPUT); pinMode(PIN_MOTOR_IN0_LEFT, OUTPUT); pinMode(PIN_MOTOR_IN1_LEFT, OUTPUT); 
  pinMode(PIN_WHEEL_ENCODER_CLK_LEFT, INPUT); pinMode(PIN_WHEEL_ENCODER_DT_LEFT, INPUT);
  pinMode(PIN_WHEEL_ENCODER_CLK_RIGHT, INPUT); pinMode(PIN_WHEEL_ENCODER_DT_RIGHT, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_WHEEL_ENCODER_CLK_LEFT), updateEncoder_left, CHANGE);  
  attachInterrupt(digitalPinToInterrupt(PIN_WHEEL_ENCODER_CLK_RIGHT), updateEncoder_right, CHANGE);  
  thetaOffset= 0.0; 
  //thetaOffset= mpu.getAngleZ();
  mySDR.setXLocation(64.0);
  mySDR.setYLocation(16.0);

  myServo.attach(PIN_SERVO);  myServo.write(MID_RIDAR_ANGLE);// attaches the servo on PIN_SERVO to the servo object and set middle position

 
  for(int i = 0; i < SCREEN_WIDTH; i++){
    for(int j = 0; j < SCREEN_HEIGHT; j++){
      ogmap[i][j] = false;
    }  
  }
  
}

void initScan(){
  sRidar.setTimeout(500);
  if (!sRidar.init()){
       Serial.println("Failed to detect and initialize sensor!");
  }else{
    sRidar.startContinuous();
  }
}

void initIMU(){
  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");
  
  thetaOffset= mpu.getAngleZ();
}

//void initOLED(){
//  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
//  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
//    Serial.println(F("SSD1306 allocation failed"));
////    for(;;); // Don't proceed, loop forever
//  }else{
//    display.display();
//  }
//  delay(1000); // Pause for 1 seconds
//
//  // Clear the buffer
//  display.clearDisplay();  
//}

void checkMessage(){
  checkSerialMessage(&Serial, msgBuffer_Serial0, &msgBufferPointer_Serial0);  // Mega (Controller) <--> PC
//  checkSerialMessage(&Serial3, msgBufferPointer_Serial3, &msgBufferPointer_Serial3); // Mega (Controller) <--> Bluetooth <--> Nano (Robot)
  checkSerialMessage(&mySerial, msgBuffer_mySerial, &msgBufferPointer_mySerial); // Mega (Controller) <--> Bluetooth <--> Nano (Robot)
}

int angle, heading = 0;
void updatePose(){
  if (cTime > pTime) {  // Update Pose
    pTime = cTime + INTERVAL_POSE; 
    mySDR.updateLocation(counter_left, counter_right, mpu.getAngleZ()); // imu based theta
 //   mySDR.updateLocation(counter_left, counter_right);    // odometry based theta 
    pose[0]= mySDR.getXLocation();
    pose[1]= mySDR.getYLocation();
    pose[2]= mySDR.getTheta();
//    Serial.print(counter_left); Serial.print(":"); 
//    Serial.print(counter_right); Serial.print(":"); 
//    Serial.print(mpu.getAngleZ()); Serial.print("="); 
//    Serial.print(pose[0]); Serial.print(":"); 
//    Serial.print(pose[1]); Serial.print(":"); 
//    Serial.println(pose[2]); 
    if (heading == 0){    // plus direction
      if (angle++ > 180){
        heading = 1 ;
        angle = 180;   
//        myServo.write(angle);
      }
    }else{                // minus direction
      if (angle-- < 0){
        heading = 0 ;
        angle = 0;   
//        myServo.write(angle);
      }      
    }
    myServo.write(angle);
//    Serial.println(angle);
  }
}

void sendReport(){
  if (cTime > rTime) {  // send message to controller
    rTime = cTime + INTERVAL_REPORT; 
    String pose_data[3] = {String(pose[0]), String(pose[1]), String(pose[2])};    
    String cmd_vel_data[3] = {String(cmd_vel[0]), String(cmd_vel[1])};    
//    sendCommand(&Serial,cmd_vel_data , 2, "c");       
    sendCommand(&mySerial, cmd_vel_data, 2, "c");       
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

    if (motor_speed[RIGHT] > MAX_MOTOR_SPEED) { 
      motor_speed[RIGHT] = MAX_MOTOR_SPEED;
    }else if (motor_speed[RIGHT] < -(MAX_MOTOR_SPEED)) {
      motor_speed[RIGHT] = -(MAX_MOTOR_SPEED);
    }
    if (motor_speed[LEFT] > MAX_MOTOR_SPEED) { 
      motor_speed[LEFT] = MAX_MOTOR_SPEED;
    }else if (motor_speed[LEFT] < -(MAX_MOTOR_SPEED)) {
      motor_speed[LEFT] = -(MAX_MOTOR_SPEED);
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
