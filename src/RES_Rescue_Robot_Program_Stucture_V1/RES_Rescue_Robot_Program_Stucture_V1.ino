/**********************************************************************;
* Project           : RES_Class, Rescure robot basic program structure
* 
* Program name      : RES_Rescue_Robot_Program_Structure.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 04/17/2020
*
* Purpose           : Program stucture of a Simple Rescue Robot 
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
#include "RES_Rescue_Robot.h"

/*
 *  Constructing MPU-6050
 */
MPU6050 mpu (Wire);

#define PIN_IR_LEFT                 33                   
#define PIN_IR_RIGHT                35            
#define PIN_ENCODER_LEFT_CLK        19                  
#define PIN_ENCODER_LEFT_DT         18                 
#define PIN_ENCODER_RIGHT_CLK       17             
#define PIN_ENCODER_RIGHT_DT        16 
//#define PIN_IR_LEFT                 33          // need to be changed = normal digital pin        
//#define PIN_IR_RIGHT                35          // need to be changed = normal digital pin  
//#define PIN_ENCODER_LEFT_CLK        19          // need to be changed = interrupt        
//#define PIN_ENCODER_LEFT_DT         18          // need to be changed = interrupt       
//#define PIN_ENCODER_RIGHT_CLK       3           // need to be changed = interrupt  
//#define PIN_ENCODER_RIGHT_DT        2           // need to be changed = interrupt
#define PIN_SONAR_PING              45
#define PIN_SONAR_ECHO              47
#define PIN_SERVO                   5
#define PIN_MOTOR_RIGHT_ENA         6
#define PIN_MOTOR_RIGHT_IN1         43
#define PIN_MOTOR_RIGHT_IN2         41
#define PIN_MOTOR_LEFT_ENB          7
#define PIN_MOTOR_LEFT_IN3          39
#define PIN_MOTOR_LEFT_IN4          37

#define LEFT  0
#define RIGHT 1
#define PREV  0
#define NOW   1

#define CONTROL_MODE_MANUAL    0
#define CONTROL_MODE_LINE_FOLLOW    1

#define INTERVAL_SENSING_SONAR                  30000         // 0.03
#define INTERVAL_SENSING_IR                     1000          // 0.001
#define INTERVAL_SENSING_SERIAL_MESSAGE         1000          // 0.001
#define INTERVAL_THINKING_LOCALIZATION          50000         // 0.05 sec
#define INTERVAL_THINKING_SERVO_POSITION        50000         // 0.05 sec
#define INTERVAL_THINKING_REPORT                50000         // 0.05
#define INTERVAL_THINKING_MESSAGE_CHECK         1000          // 0.001
#define INTERVAL_ACTING_MOTOR                   1000          // 0.001
#define INTERVAL_ACTING_SERVO                   1000          // 0.001

#define WAITTIME_MOVE_SONAR                     20000         // 0.02 wait for servo move time before sonar reading

#define MIN_SONAR_ANGLE      30
#define MID_SONAR_ANGLE      90
#define MAX_SONAR_ANGLE      150
#define MAX_SONAR_VALUE       300      // e.g. 600 mm is max range 
#define STRIDE_SONAR_ANGLE     1

#define JOY_DEADZONE           10

#define RES_PROTOCOL_MAX_BUF_SIZE   64

Servo myServo;  // create servo object to control a servo

long counterLeftWheel[2] = {0,0}; long counterRightWheel[2] = {0,0};

int motorSpeed[2] = {0,0};   // current left/right motor speed {range -255 ~ + 255}
int controlMode = 0;         // manual control = 0, line following = 1,

char msgBuf[RES_PROTOCOL_MAX_BUF_SIZE];
int msgBufPnt = 0;
int joystick[3] = {0,0,0};    // joystick status

int statusIRLeft[2] = {0,0}; int statusIRRight[2] = {0,0};
 
unsigned long cTime = 0;     // current time of each iteration 
unsigned long sSensingTime = 0;     // sonar value 
unsigned long iSensingTime = 0;     // IR-Line sensor value 
unsigned long mSensingTime = 0;     // Serial Message reading 
unsigned long lThinkingTime = 0;     // localization checking time
unsigned long sThinkingTime = 0;     // servo position checking time
unsigned long rThinkingTime = 0;     // reporting time
unsigned long mThinkingTime = 0;     // message (command) checking time
unsigned long mActingTime = 0;     // motor control time
unsigned long sActingTime = 0;     // servo control time

int sonarAngle = MID_SONAR_ANGLE;
int sonar[2][180];
int sonarScanDirection = 0;

int prevLeftCLK, prevLeftDT, nowLeftCLK, nowLeftDT = 0;
int prevRightCLK, prevRightDT, nowRightCLK, nowRightDT = 0;
long lCounter, rCounter = 0;

float cTheta, xLocation, yLocation = 0;
int xStartingOffset, yStartingOffset = 0;

SimpleDeadReckoning mySDR( 122.0, 3.39, 15.5, 0);   // encoder values per one rotation,  wheel radius, distance between two wheels, unit (cm)
float thetaOffset = 0.0;

void setup() {
  // Serial Port Setup
  Serial.begin(115200);   Serial3.begin(115200);
  Serial.println("Initialization Started");

  // Initialization
  mpu.Initialize();   // IMU init
  Serial.println("Starting calibration...");
  mpu.Calibrate();
  thetaOffset= mpu.GetAngZ();
  mySDR.setXLocation(400.0);        // starting location X in a path display 
  mySDR.setYLocation(400.0);        // starting location y in a path display


  // Motor Driver Setting
  pinMode(PIN_MOTOR_LEFT_ENB, OUTPUT);  pinMode(PIN_MOTOR_LEFT_IN3, OUTPUT); pinMode(PIN_MOTOR_LEFT_IN4, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_ENA, OUTPUT);  pinMode(PIN_MOTOR_RIGHT_IN1, OUTPUT); pinMode(PIN_MOTOR_RIGHT_IN2, OUTPUT);

  // Wheel Encoder Setting
  pinMode(PIN_ENCODER_LEFT_CLK, INPUT_PULLUP);  pinMode(PIN_ENCODER_RIGHT_CLK, INPUT_PULLUP);
  pinMode(PIN_ENCODER_LEFT_DT, INPUT_PULLUP);  pinMode(PIN_ENCODER_RIGHT_DT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_LEFT_CLK), checkLeftEncoder, CHANGE);  
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_LEFT_DT), checkLeftEncoder, CHANGE);  
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_RIGHT_CLK), checkRightEncoder, CHANGE);  
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_RIGHT_DT), checkRightEncoder, CHANGE);  
  // Line Sensor Setting
  pinMode(PIN_IR_LEFT, INPUT_PULLUP);  pinMode(PIN_IR_RIGHT, INPUT_PULLUP);
  // Sonar Setting
  pinMode(PIN_SONAR_PING, OUTPUT);     pinMode(PIN_SONAR_ECHO, INPUT);
  // Servo Setting for Sonar
  myServo.attach(PIN_SERVO);  myServo.write(MID_SONAR_ANGLE);// attaches the servo on PIN_SERVO to the servo object and set middle position

}



void loop() {
  cTime = micros();

  sensing();

  thinking();

  acting();

}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

void sensing(){
  if (cTime > sSensingTime){
     sensingSonarValue();
     sSensingTime = cTime + INTERVAL_SENSING_SONAR; // next sonar reading event
  }
  if (cTime > iSensingTime){
     sensingIRValue();
     iSensingTime = cTime + INTERVAL_SENSING_IR; // next IR sensor reading event     
  }
  if (cTime > mSensingTime){
     sensingCheckSerialMessge();
     mSensingTime = cTime + INTERVAL_SENSING_SERIAL_MESSAGE; // next Serial Message Reading event         
  }
}

void thinking(){
  if (cTime > lThinkingTime){
     thinkingRobotPosition();  // robot location caculation
     lThinkingTime = cTime + INTERVAL_THINKING_LOCALIZATION; // next thinking event
  }
  if (cTime > sThinkingTime){
     thinkingSonarPosition();  // sonar servo potion update
     sThinkingTime = cTime + INTERVAL_THINKING_SERVO_POSITION; // next thinking event
//     sSensingTime = sThinkingTime + WAITTIME_MOVE_SONAR;       // need some wait before reading new sonar reading
  }
  
  if (cTime > rThinkingTime){
     thinkingReportSonarData();
     thinkingReportIRData();
     thinkingReportIMUData();
     thinkingReportRobotPosition();
     rThinkingTime = cTime + INTERVAL_THINKING_REPORT; // next reporting event
  }    
  if (cTime > mThinkingTime){
     thinkingCheckMission();
     mThinkingTime = cTime + INTERVAL_THINKING_MESSAGE_CHECK; // next Command Message event
  }  
}

void acting(){
  if (cTime > mActingTime){        
      if(motorSpeed[0] > 0){
        analogWrite(PIN_MOTOR_RIGHT_ENA, abs(motorSpeed[0])); digitalWrite( PIN_MOTOR_RIGHT_IN1, HIGH);    digitalWrite( PIN_MOTOR_RIGHT_IN2 , LOW);
      }else if(motorSpeed[0] < 0){
        analogWrite(PIN_MOTOR_RIGHT_ENA, abs(motorSpeed[0])); digitalWrite( PIN_MOTOR_RIGHT_IN1, LOW);    digitalWrite( PIN_MOTOR_RIGHT_IN2 , HIGH);
      }else{
        analogWrite(PIN_MOTOR_RIGHT_ENA, 0); digitalWrite( PIN_MOTOR_RIGHT_IN1, LOW);    digitalWrite( PIN_MOTOR_RIGHT_IN2 , LOW);
      }
      
      if(motorSpeed[1] > 0){
        analogWrite(PIN_MOTOR_LEFT_ENB, abs(motorSpeed[1]));   digitalWrite( PIN_MOTOR_LEFT_IN3, HIGH);    digitalWrite( PIN_MOTOR_LEFT_IN4 , LOW);
      }else if(motorSpeed[1] < 0){
        analogWrite(PIN_MOTOR_LEFT_ENB, abs(motorSpeed[1]));   digitalWrite( PIN_MOTOR_LEFT_IN3, LOW);    digitalWrite( PIN_MOTOR_LEFT_IN4 , HIGH);
      }else{
        analogWrite(PIN_MOTOR_LEFT_ENB, 0);   digitalWrite( PIN_MOTOR_LEFT_IN3, LOW);    digitalWrite( PIN_MOTOR_LEFT_IN4 , LOW);
      }
      mActingTime = cTime + INTERVAL_ACTING_MOTOR;
  }
  if (cTime > sActingTime){
     myServo.write(sonarAngle);
     sActingTime = cTime + INTERVAL_ACTING_SERVO;
  }  
}


//////////////////////////////////////////////////////////////////////////////////////
/////////   Thinking
//////////////////////////////////////////////////////////////////////////////////////
void thinkingRobotPosition(){
  mpu.Execute();
  cTheta = mpu.GetAngZ() - thetaOffset;
  mySDR.updateLocation(lCounter, rCounter, cTheta);
}

void thinkingSonarPosition(){
  // Servo motor position
  if (sonarScanDirection == 0){ // increase scan angle
    sonarAngle+= STRIDE_SONAR_ANGLE;
    if (sonarAngle >= MAX_SONAR_ANGLE) sonarScanDirection = 1;     
  }else{
    sonarAngle-= STRIDE_SONAR_ANGLE;
    if (sonarAngle <= MIN_SONAR_ANGLE) sonarScanDirection = 0;         
  }
}

void thinkingReportSonarData(){
  Serial3.print("sonar ");
  Serial3.print(sonarAngle);
  Serial3.print(" ");
  Serial3.print(sonar[NOW][sonarAngle]); 
  Serial3.print(" ");
  Serial3.println(sonar[PREV][sonarAngle]);     
}

void thinkingReportIRData(){
 ////////////////////////////////////////////////////////////////////////////////////
  //// complete robot IR sensor report 
  ////          -- hint.1  Serial3.print()  // Serial3 is connected to bluetooth
  ////          -- hint.2  check message structure in RES_Remote_Controller_Program_Structure_V1.ino 
  ////          
  ////////////////////////////////////////////////////////////////////////////////////   
}

void thinkingReportIMUData(){
 ////////////////////////////////////////////////////////////////////////////////////
  //// complete robot orientation report 
  ////          -- hint.1  Serial3.print()  // Serial3 is connected to bluetooth
  ////          -- hint.2  check message structure in sonar_imu_v1.py 
  ////          -- hint.3  Which axis would be the orientation of the robot
  ////////////////////////////////////////////////////////////////////////////////////   
}


void thinkingReportRobotPosition(){
  ////////////////////////////////////////////////////////////////////////////////////
  //// complete robot position report 
  ////          -- hint.1  Serial3.print()  // Serial3 is connected to bluetooth
  ////          -- hint.2  check message structure in path_v1.py 
  ////
  ////////////////////////////////////////////////////////////////////////////////////    
}

void thinkingCheckMission(){
  if (controlMode != joystick[2] )  controlMode =  joystick[2]; // update controlMode from joy sw input    

  if (controlMode == CONTROL_MODE_MANUAL){
  ////////////////////////////////////////////////////////////////////////////////////
  ////
  //// manual control function
  ////          -- control by joystick
  ////
  ////////////////////////////////////////////////////////////////////////////////////     
  }else if (controlMode == CONTROL_MODE_LINE_FOLLOW){
  ////////////////////////////////////////////////////////////////////////////////////
  ////
  //// line follow control function
  ////          -- switched by joystick sw value
  ////
  ////////////////////////////////////////////////////////////////////////////////////     

  }
}

//////////////////////////////////////////////////////////////////////////////////////
////                Sensing check sensors
//////////////////////////////////////////////////////////////////////////////////////
void sensingIRValue(){
  statusIRLeft[PREV] = statusIRLeft[NOW];
  statusIRRight[PREV] = statusIRRight[NOW];
  statusIRLeft[NOW] = digitalRead(PIN_IR_LEFT);
  statusIRRight[NOW] = digitalRead(PIN_IR_RIGHT);
}
void sensingSonarValue(){
    ////////////////////////////////////////////////////////////////////////////////////
    ////
    //// complete sonar reading fuction 
    ////          -- hint.1 you may refer the RES_Drawing_IMU_Sonar
    ////          -- hint.2 you need to change sonarAngle for scanning 
    ////
    ////////////////////////////////////////////////////////////////////////////////////    
  sonar[PREV][sonarAngle] = sonar[NOW][sonarAngle];
  sonar[NOW][sonarAngle] = "sonar reading vaule";
}

//////////////////////////////////////////////////////////////////////////////////////
//////////  Sensing Message Checking 
//////////////////////////////////////////////////////////////////////////////////////
void sensingCheckSerialMessge(){
  while (Serial3.available() > 0){
    ////////////////////////////////////////////////////////////////////////////////////
    ////
    //// complete serial message fuction 
    ////          -- hint. you may refer the RES_Remote_Controller_Program_Structure
    ////
    ////////////////////////////////////////////////////////////////////////////////////     
    
  }
}

bool checkFrame(){
 char *p = msgBuf;
 String str;
 int cnt =0;
    ////////////////////////////////////////////////////////////////////////////////////
    ////
    //// complete message interpret fuction 
    ////          -- hint. you may refer the RES_Remote_Controller_Program_Structure
    ////
    ////////////////////////////////////////////////////////////////////////////////////     

}


//////////////////////////////////////////////////////////////////////////////////////
//////////      Sensing with interrupts
//////////////////////////////////////////////////////////////////////////////////////

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




    
