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
#include <Servo.h>
#include <PID_v1.h>
#include <Arduino.h>
#include <TinyMPU6050.h>
#include "SimpleDeadReckoning.h"
#include "Config.h"

/*
 *  Constructing MPU-6050
 */

long counterLeftWheel[2] = {0,0}; long counterRightWheel[2] = {0,0};

int motorSpeed[2] = {0,0};   // current left/right motor speed {range -255 ~ + 255}
float cmd_vel[2] = {0.0, 0.0};    // LINEAR 0, ANGULAR 1
float odom_pose[3] = {0.0, 0.0, 0.0};
float odom_vel[2] = {0.0, 0.0};
int controlMode = 0;         // manual control = 0, line following = 1,

int motorTestCount = 0;

int statusIRLeft[2] = {0,0}; int statusIRRight[2] = {0,0};
 
long cTime = 0;     // current time of each iteration 
long lTime = 0;     // localization interval
long mTime = 0;     // motor interval
long sTime = 0;     // sonar interval
long iTime = 0;     // IMU interval
long dTime = 0;     // debug print interval

int sonarAngle = MID_SONAR_ANGLE;

int prevLeftCLK, prevLeftDT, nowLeftCLK, nowLeftDT = 0;
int prevRightCLK, prevRightDT, nowRightCLK, nowRightDT = 0;
long lCounter, rCounter = 0;
float cTheta, xLocation, yLocation = 0;

float thetaOffset = 0.0;

MPU6050 mpu (Wire);
Servo myServo;  // create servo object to control a servo
SimpleDeadReckoning mySDR( 122.0, 3.39, 15.5, 0);   // encoder values per one rotation,  wheel radius, distance between two wheels, unit (cm)

void setup() {
  // Serial Port Setup
  Serial.begin(115200); //  Serial3.begin(115200);
  Serial.println("Initialization Started");

//  // IMU init
  mpu.Initialize();
//  Serial.println("Starting calibration...");
//  mpu.Calibrate();
  thetaOffset= mpu.GetAngZ();

  mySDR.setXLocation(500.0);
  mySDR.setYLocation(400.0);
  
//  Serial.println("Calibration complete!");
//  Serial.println("Offsets:");
//  Serial.print("GyroX Offset = ");
//  Serial.println(mpu.GetGyroXOffset());
//  Serial.print("GyroY Offset = ");
//  Serial.println(mpu.GetGyroYOffset());
//  Serial.print("GyroZ Offset = ");
//  Serial.println(mpu.GetGyroZOffset());
//

  // Motor Driver Setting
  pinMode(PIN_MOTOR_LEFT_ENB, OUTPUT);  pinMode(PIN_MOTOR_LEFT_IN3, OUTPUT); pinMode(PIN_MOTOR_LEFT_IN4, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_ENA, OUTPUT);  pinMode(PIN_MOTOR_RIGHT_IN1, OUTPUT); pinMode(PIN_MOTOR_RIGHT_IN2, OUTPUT);

  // Wheel Encoder Setting
  pinMode(PIN_ENCODER_LEFT_CLK, INPUT_PULLUP);  pinMode(PIN_ENCODER_RIGHT_CLK, INPUT_PULLUP);
  pinMode(PIN_ENCODER_LEFT_DT, INPUT_PULLUP);  pinMode(PIN_ENCODER_RIGHT_DT, INPUT_PULLUP);
//  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_LEFT_CLK), checkLeftEncoder, CHANGE);  
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_LEFT_DT), checkLeftEncoder, CHANGE);  
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_RIGHT_CLK), checkRightEncoder, CHANGE);  
//  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_RIGHT_DT), checkRightEncoder, CHANGE);  
  // Line Sensor Setting
  pinMode(PIN_IR_LEFT, INPUT_PULLUP);  pinMode(PIN_IR_RIGHT, INPUT_PULLUP);
  // Sonar Setting
  pinMode(PIN_SONAR_PING, OUTPUT);     pinMode(PIN_SONAR_ECHO, INPUT);
//  myServo.attach(PIN_SERVO);  myServo.write(MID_SONAR_ANGLE);// attaches the servo on PIN_SERVO to the servo object and set middle position


  
  // 
}



int cnt = 0;
void loop() {
  cTime = millis ();
  recieved_py();
  
  mpu.Execute();
  cTheta = mpu.GetAngZ()- thetaOffset;
  mySDR.updateLocation(lCounter, rCounter, cTheta);
  Serial.print("path ");
  Serial.print(mySDR.getXLocation());
  Serial.print(" ");
  Serial.println(mySDR.getYLocation());
  delay(100);
  
  //motor checking
//  analogWrite(PIN_MOTOR_RIGHT_ENA, 255);   digitalWrite( PIN_MOTOR_RIGHT_IN1, HIGH);    digitalWrite( PIN_MOTOR_RIGHT_IN2 , LOW);
//  analogWrite(PIN_MOTOR_LEFT_ENB, 255);   digitalWrite( PIN_MOTOR_LEFT_IN3, HIGH);    digitalWrite( PIN_MOTOR_LEFT_IN4 , LOW);
//
//  //Servo checking
//  myServo.write(100);


//  //IR Line sensor checking
//  Serial.print( digitalRead(PIN_IR_LEFT)); Serial.print(" "); Serial.print(digitalRead(PIN_IR_RIGHT)); Serial.print("][");
//  //Encoder checking
//  Serial.print( lCounter); Serial.print(" = ");
//  Serial.print( digitalRead(PIN_ENCODER_LEFT_CLK)); Serial.print(" "); Serial.print(digitalRead(PIN_ENCODER_LEFT_DT));Serial.print("][");
//  Serial.print( rCounter); Serial.print(" = ");
//  Serial.print( digitalRead(PIN_ENCODER_RIGHT_CLK)); Serial.print(" "); Serial.print(digitalRead(PIN_ENCODER_RIGHT_DT));Serial.print("][");


//  //Sonar Checking
//  digitalWrite(PIN_SONAR_PING, LOW);
//  delayMicroseconds(2);
//  digitalWrite(PIN_SONAR_PING, HIGH);
//  delayMicroseconds(5);
//  digitalWrite(PIN_SONAR_PING, LOW);
//  Serial.print(pulseIn(PIN_SONAR_ECHO, HIGH,5000 ) / 29 / 2 ); Serial.print("]");
// 
 
//  //IMU checking
//  Serial.print("[");
//  mpu.Execute();
//  Serial.print(mpu.GetAngX());
//  Serial.print("  ");
//  Serial.print(mpu.GetAngY());
//  Serial.print("  ");
//  Serial.print(mpu.GetAngZ());
//  Serial.println("]");


//  //Bluetooth Serial checking
//  while (Serial.available() > 0){
//    char a = Serial.read();
//    Serial3.write((char)a);  
//  }
//  while (Serial3.available() > 0){
//    char b = Serial3.read();
//    Serial.write((char)b);  
//  }
//  
//  delay(30);
//  
    String odom_temp[8] = {String(odom_pose[0]), String(odom_pose[1]), String(odom_pose[2]), String(odom_vel[0]), String(odom_vel[2])};
    send_py(odom_temp, 5, ODOM_MSG);
    
}




void checkLeftEncoder(){
  prevLeftCLK = nowLeftCLK;
  prevLeftDT = nowLeftDT;
  nowLeftCLK = digitalRead(PIN_ENCODER_LEFT_CLK);
  nowLeftDT = digitalRead(PIN_ENCODER_LEFT_DT);
  if ((prevLeftCLK == 0) && (prevLeftDT == 0)){
      if ((nowLeftCLK == 0) && (nowLeftDT == 1)){
        lCounter++;
      }else if ((nowLeftCLK == 1) && (nowLeftDT == 0)){
        lCounter--;
      }
  }else if ((prevLeftCLK == 0) && (prevLeftDT == 1)){
      if ((nowLeftCLK == 0) && (nowLeftDT == 0)){
        lCounter--;
      }else if ((nowLeftCLK == 1) && (nowLeftDT == 1)){
        lCounter++;
      }  
  }else if ((prevLeftCLK == 1) && (prevLeftDT == 0)){
      if ((nowLeftCLK == 0) && (nowLeftDT == 0)){
        lCounter++;
      }else if ((nowLeftCLK == 1) && (nowLeftDT == 1)){
        lCounter--;
      }
  }else if ((prevLeftCLK == 1) && (prevLeftDT == 1)){
      if ((nowLeftCLK == 0) && (nowLeftDT == 1)){
        lCounter--;
      }else if ((nowLeftCLK == 1) && (nowLeftDT == 0)){
        lCounter++;
      }
   }
}



void checkRightEncoder(){
  prevRightCLK = nowRightCLK;
  prevRightDT = nowRightDT;
  nowRightCLK = digitalRead(PIN_ENCODER_RIGHT_CLK);
  nowRightDT = digitalRead(PIN_ENCODER_RIGHT_DT);
  if ((prevRightCLK == 0) && (prevRightDT == 0)){
      if ((nowRightCLK == 0) && (nowRightDT == 1)){
        rCounter++;
      }else if ((nowRightCLK == 1) && (nowRightDT == 0)){
        rCounter--;
      }
  }else if ((prevRightCLK == 0) && (prevRightDT == 1)){
      if ((nowRightCLK == 0) && (nowRightDT == 0)){
        rCounter--;
      }else if ((nowRightCLK == 1) && (nowRightDT == 1)){
        rCounter++;
      }  
  }else if ((prevRightCLK == 1) && (prevRightDT == 0)){
      if ((nowRightCLK == 0) && (nowRightDT == 0)){
        rCounter++;
      }else if ((nowRightCLK == 1) && (nowRightDT == 1)){
        rCounter--;
      }
  }else if ((prevRightCLK == 1) && (prevRightDT == 1)){
      if ((nowRightCLK == 0) && (nowRightDT == 1)){
        rCounter--;
      }else if ((nowRightCLK == 1) && (nowRightDT == 0)){
        rCounter++;
      }
   }
}



void recieved_py(){
  if (Serial.available() > 0){
    char tmpChar = Serial.read();
    if ((msgBufferPointer == 0) && (tmpChar == '<')){
      msgBuffer[msgBufferPointer] = tmpChar; 
      msgBufferPointer++;
    }
    else if (msgBufferPointer == 1){
      if (tmpChar == '!'){ 
        msgBuffer[msgBufferPointer] = tmpChar; 
        msgBufferPointer++; 
      }
      else{
        if (tmpChar == '<'){
          msgBuffer[0] = tmpChar;
          msgBufferPointer = 1;
        }else{
          msgBufferPointer = 0;          
        }
      }
    }

    else if (msgBufferPointer >= 2){
      if (tmpChar == '<'){ 
        msgBuffer[0] = tmpChar;
        msgBufferPointer = 1;
      }
      else if (tmpChar == '>'){
        msgBuffer[msgBufferPointer] = tmpChar;
        msgBufferPointer = 0;
        evaluateCommand();
      }
      else{
        msgBuffer[msgBufferPointer] = tmpChar;
        msgBufferPointer++;
      }
    }
  }
}

void evaluateCommand(){
  char* command = strtok(msgBuffer, ",");
  int temp = 0;
  String type = "none";
  while (command != 0){
    if (temp == 1){
      if (String(command) == TORQUE_MSG){
        type = TORQUE_MSG;
      }
      else if (String(command) == CMD_MSG){
        type = CMD_MSG;
      }
    }

    if (type == TORQUE_MSG){
      if (temp == 3){
        if (String(command) == "true"){
          Serial.println("Torque-on function will be implemented");
        }
        else if (String(command) == "false"){
          Serial.println("Torque-off function will be implemented");
        }
      }
    }
    else if (type == CMD_MSG){
      if (temp == 3){
        Serial.println(String(String(command).toFloat()));
        cmd_vel[LINEAR] = String(command).toFloat();
      }
      else if (temp == 4){
        Serial.println(String(String(command).toFloat()));
        cmd_vel[ANGULAR] = String(command).toFloat();
      }
    }
    temp += 1;
    command = strtok(0, ",");
  }
}

void send_py(String data[], int size, String type){
  String data_out;
  data_out = "<!," + type + "," + String(size) + ",";
  for(int i = 0; i < size; i++){
    data_out += data[i];
    data_out += ",";
  }
  data_out += "#>";

  Serial.print(data_out);
  Serial.print("\n");
}
