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

/*
 *  Constructing MPU-6050
 */
MPU6050 mpu (Wire);

#define PIN_IR_LEFT                 19            //TX1 (D 19)        
#define PIN_IR_RIGHT                18            //RX1 (D 18)
#define PIN_ENCODER_LEFT_CLK        2             // or TX2 (D 17)     
#define PIN_ENCODER_LEFT_DT         53                 
#define PIN_ENCODER_RIGHT_CLK       3             // or RX2 (D 16)
#define PIN_ENCODER_RIGHT_DT        51 
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

#define INTERVAL_MOTOR_TEST       5000
#define INTERVAL_SONAR_TEST       500
#define INTERVAL_IMU_TEST         2000
#define INTERVAL_DEBUG_PRINT      2000

#define MIN_SONAR_ANGLE      30
#define MID_SONAR_ANGLE      90
#define MAX_SONAR_ANGLE      150
#define STRID_SONAR_ANGLE     5


Servo myServo;  // create servo object to control a servo

long counterLeftWheel[2] = {0,0}; long counterRightWheel[2] = {0,0};

int motorSpeed[2] = {0,0};   // current left/right motor speed {range -255 ~ + 255}
int controlMode = 0;         // manual control = 0, line following = 1,

int motorTestCount = 0;

int statusIRLeft[2] = {0,0}; int statusIRRight[2] = {0,0};
 
long cTime = 0;     // current time of each iteration 
long mTime = 0;     // motor test interval
long sTime = 0;     // sonar test interval
long iTime = 0;     // IMU test interval
long dTime = 0;     // debug print interval

int sonarAngle = MID_SONAR_ANGLE;

void setup() {
  // Serial Port Setup
  Serial.begin(115200);   Serial3.begin(115200);
  Serial.println("Initialization Started");

  // IMU init
  mpu.Initialize();
  Serial.println("Starting calibration...");
//  mpu.Calibrate();
  Serial.println("Calibration complete!");
  Serial.println("Offsets:");
  Serial.print("GyroX Offset = ");
  Serial.println(mpu.GetGyroXOffset());
  Serial.print("GyroY Offset = ");
  Serial.println(mpu.GetGyroYOffset());
  Serial.print("GyroZ Offset = ");
  Serial.println(mpu.GetGyroZOffset());

  // Motor Driver Setting
  pinMode(PIN_MOTOR_LEFT_ENB, OUTPUT);  pinMode(PIN_MOTOR_LEFT_IN3, OUTPUT); pinMode(PIN_MOTOR_LEFT_IN4, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_ENA, OUTPUT);  pinMode(PIN_MOTOR_RIGHT_IN1, OUTPUT); pinMode(PIN_MOTOR_RIGHT_IN2, OUTPUT);

  // Wheel Encoder Setting
  pinMode(PIN_ENCODER_LEFT_CLK, INPUT_PULLUP);  pinMode(PIN_ENCODER_RIGHT_CLK, INPUT_PULLUP);
  pinMode(PIN_ENCODER_LEFT_DT, INPUT_PULLUP);  pinMode(PIN_ENCODER_RIGHT_DT, INPUT_PULLUP);

  // Line Sensor Setting
  pinMode(PIN_IR_LEFT, INPUT_PULLUP);  pinMode(PIN_IR_RIGHT, INPUT_PULLUP);
  // Sonar Setting
  pinMode(PIN_SONAR_PING, OUTPUT);     pinMode(PIN_SONAR_ECHO, INPUT);
  myServo.attach(PIN_SERVO);  myServo.write(MID_SONAR_ANGLE);// attaches the servo on PIN_SERVO to the servo object and set middle position

  // 
}




void loop() {
  //motor checking
  analogWrite(PIN_MOTOR_RIGHT_ENA, 255);   digitalWrite( PIN_MOTOR_RIGHT_IN1, HIGH);    digitalWrite( PIN_MOTOR_RIGHT_IN2 , LOW);
  analogWrite(PIN_MOTOR_LEFT_ENB, 255);   digitalWrite( PIN_MOTOR_LEFT_IN3, HIGH);    digitalWrite( PIN_MOTOR_LEFT_IN4 , LOW);

  //Servo checking
  myServo.write(100);

  //IR Line sensor checking
  Serial.print( digitalRead(PIN_IR_LEFT)); Serial.print(" "); Serial.print(digitalRead(PIN_IR_RIGHT)); Serial.print("][");
  //Encoder checking
  Serial.print( digitalRead(PIN_ENCODER_LEFT_CLK)); Serial.print(" "); Serial.print(digitalRead(PIN_ENCODER_LEFT_DT));Serial.print("][");
  Serial.print( digitalRead(PIN_ENCODER_RIGHT_CLK)); Serial.print(" "); Serial.print(digitalRead(PIN_ENCODER_RIGHT_DT));Serial.print("][");

  //Sonar Checking
  digitalWrite(PIN_SONAR_PING, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_SONAR_PING, HIGH);
  delayMicroseconds(5);
  digitalWrite(PIN_SONAR_PING, LOW);
  Serial.print(pulseIn(PIN_SONAR_ECHO, HIGH,5000 ) / 29 / 2 ); Serial.print("]");
  
  //IMU checking
  Serial.print("[");
  mpu.Execute();
  Serial.print(mpu.GetAngX());
  Serial.print("  ");
  Serial.print(mpu.GetAngY());
  Serial.print("  ");
  Serial.print(mpu.GetAngZ());
  Serial.println("]");

  //Bluetooth Serial checking
  if (Serial.available() > 0){
    Serial3.print(Serial.read());  
  }
  if (Serial3.available() > 0){
    Serial.print(Serial3.read());  
  }
  
  
  delay(30);

}



  
