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

  // Motor Driver Setting
  pinMode(PIN_MOTOR_LEFT_ENB, OUTPUT);  pinMode(PIN_MOTOR_LEFT_IN3, OUTPUT); pinMode(PIN_MOTOR_LEFT_IN4, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_ENA, OUTPUT);  pinMode(PIN_MOTOR_RIGHT_IN1, OUTPUT); pinMode(PIN_MOTOR_RIGHT_IN2, OUTPUT);

  // Wheel Encoder Setting
  pinMode(PIN_ENCODER_LEFT_CLK, INPUT_PULLUP);  pinMode(PIN_ENCODER_RIGHT_CLK, INPUT_PULLUP);
  pinMode(PIN_ENCODER_LEFT_DT, INPUT_PULLUP);  pinMode(PIN_ENCODER_RIGHT_DT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_LEFT_CLK), updateLeftEncoder, CHANGE);  
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_RIGHT_CLK), updateRightEncoder, CHANGE);  

  // Line Sensor Setting
  pinMode(PIN_IR_LEFT, INPUT_PULLUP);  pinMode(PIN_IR_RIGHT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_IR_LEFT), updateLeftIR, CHANGE);  
  attachInterrupt(digitalPinToInterrupt(PIN_IR_RIGHT), updateRightIR, CHANGE);    
  
  // Sonar Setting
  pinMode(PIN_SONAR_PING, OUTPUT);     pinMode(PIN_SONAR_ECHO, OUTPUT);
  myServo.attach(PIN_SERVO);  myServo.write(MID_SONAR_ANGLE);// attaches the servo on PIN_SERVO to the servo object and set middle position

  // 
}




void loop() {
  cTime = millis();     // get current time for this iteration

  if (cTime > mTime) {  // motor testing time
    mTime = millis() + INTERVAL_MOTOR_TEST; // next motor testing time
    if (motorTestCount == 0){   
      analogWrite(PIN_MOTOR_RIGHT_ENA, 0);   digitalWrite( PIN_MOTOR_RIGHT_IN1, LOW);    digitalWrite( PIN_MOTOR_RIGHT_IN2 , LOW);
      analogWrite(PIN_MOTOR_LEFT_ENB, 0);   digitalWrite( PIN_MOTOR_LEFT_IN3, LOW);    digitalWrite( PIN_MOTOR_LEFT_IN4 , LOW);
      motorTestCount = 1;
    }else if (motorTestCount == 1){
      analogWrite(PIN_MOTOR_RIGHT_ENA, 255);   digitalWrite( PIN_MOTOR_RIGHT_IN1, HIGH);    digitalWrite( PIN_MOTOR_RIGHT_IN2 , LOW);
      analogWrite(PIN_MOTOR_LEFT_ENB, 0);   digitalWrite( PIN_MOTOR_LEFT_IN3, LOW);    digitalWrite( PIN_MOTOR_LEFT_IN4 , LOW);
      motorTestCount = 2;
    }else if (motorTestCount == 2){
      analogWrite(PIN_MOTOR_RIGHT_ENA, 255);   digitalWrite( PIN_MOTOR_RIGHT_IN1, LOW);    digitalWrite( PIN_MOTOR_RIGHT_IN2 , HIGH);
      analogWrite(PIN_MOTOR_LEFT_ENB, 0);   digitalWrite( PIN_MOTOR_LEFT_IN3, LOW);    digitalWrite( PIN_MOTOR_LEFT_IN4 , LOW);
      motorTestCount = 3;
    }else if (motorTestCount == 3){
      analogWrite(PIN_MOTOR_RIGHT_ENA, 0);   digitalWrite( PIN_MOTOR_RIGHT_IN1, LOW);    digitalWrite( PIN_MOTOR_RIGHT_IN2 , LOW);
      analogWrite(PIN_MOTOR_LEFT_ENB, 255);   digitalWrite( PIN_MOTOR_LEFT_IN3, HIGH);    digitalWrite( PIN_MOTOR_LEFT_IN4 , LOW);
      motorTestCount = 4;
    }else if (motorTestCount == 4){
      analogWrite(PIN_MOTOR_RIGHT_ENA, 0);   digitalWrite( PIN_MOTOR_RIGHT_IN1, LOW);    digitalWrite( PIN_MOTOR_RIGHT_IN2 , LOW);
      analogWrite(PIN_MOTOR_LEFT_ENB, 255);   digitalWrite( PIN_MOTOR_LEFT_IN3, LOW);    digitalWrite( PIN_MOTOR_LEFT_IN4 , HIGH);
      motorTestCount = 0;
    }
  }

  if (cTime > sTime) {  // sonar testing time
    sTime = millis() + INTERVAL_SONAR_TEST; 
    sonarAngle += STRID_SONAR_ANGLE;
    if (sonarAngle > MAX_SONAR_ANGLE)  sonarAngle = MIN_SONAR_ANGLE;
    myServo.write(sonarAngle);
    Serial.print("Sonar [  "); Serial.print(sonarAngle); Serial.print("]=");  Serial.println("Ping result");
  }
  if (cTime > iTime) {  //  IMU time
    iTime = millis() + INTERVAL_IMU_TEST; 
    Serial.println("IMU reading ");
  }
  
  if (cTime > dTime) {  //  DEBUG time
    dTime = millis() + INTERVAL_DEBUG_PRINT; 
    debugPrint();
  }

  if (statusIRRight[NOW] != statusIRRight[PREV] ){
     statusIRRight[PREV] = statusIRRight[NOW];
     Serial.print("IR:  "); Serial.print(statusIRLeft[NOW]); Serial.print("  "); Serial.println(statusIRRight[NOW]); 
  }
  if (statusIRLeft[NOW] != statusIRLeft[PREV] ){
     statusIRLeft[PREV] = statusIRLeft[NOW];
     Serial.print("IR:  "); Serial.print(statusIRLeft[NOW]); Serial.print("  "); Serial.println(statusIRRight[NOW]); 
  }

  
  if (counterRightWheel[NOW] != counterRightWheel[PREV] ){
    counterRightWheel[PREV] = counterRightWheel[NOW];
    Serial.print("Wheel:  "); Serial.print(counterRightWheel[NOW]); Serial.print("  "); Serial.println(counterLeftWheel[NOW]);
  }
  if (counterLeftWheel[NOW] != counterLeftWheel[PREV] ){
    counterLeftWheel[PREV] = counterLeftWheel[NOW];
    Serial.print("Wheel:  "); Serial.print(counterRightWheel[NOW]); Serial.print("  "); Serial.println(counterLeftWheel[NOW]);
  }

  checkSerialMessage();  
  
   

}


void debugPrint(){
  Serial.print(motorTestCount);
  Serial.println("----"); 

}


void checkSerialMessage(){
  while (Serial.available() > 0){
    Serial3.write( Serial.read());
  }
  while (Serial3.available() > 0){
    String msg = Serial3.readStringUntil('\n');
    Serial.println(msg);
//    char tmpChar = Serial3.read();
//    Serial.write(tmpChar);
//    Serial3.write('[');
//    Serial3.write(tmpChar);
//    Serial3.write(']');
  }
  
}



// Interrupt Setting

void updateLeftEncoder(){
  if (digitalRead(PIN_ENCODER_LEFT_CLK) == 0){   
    if (digitalRead(PIN_ENCODER_LEFT_DT) == 0) counterLeftWheel[NOW]++; else counterLeftWheel[NOW]--;
  }else{
    if (digitalRead(PIN_ENCODER_LEFT_DT) == 0) counterLeftWheel[NOW]--; else counterLeftWheel[NOW]++;
  }
}

void updateRightEncoder(){
  if (digitalRead(PIN_ENCODER_RIGHT_CLK) == 0){   
    if (digitalRead(PIN_ENCODER_RIGHT_DT) == 0) counterRightWheel[NOW]++; else counterRightWheel[NOW]--;
  }else{
    if (digitalRead(PIN_ENCODER_RIGHT_DT) == 0) counterRightWheel[NOW]--; else counterRightWheel[NOW]++;
  }
}

void updateLeftIR(){
  // do some action when it changes
  statusIRLeft[NOW] = digitalRead(PIN_IR_LEFT);
}

void updateRightIR(){
  // do some action when it changes
  statusIRRight[NOW] = digitalRead(PIN_IR_RIGHT);
}
