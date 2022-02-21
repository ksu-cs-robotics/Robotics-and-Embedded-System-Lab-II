/*##########################################################################
# Project           : RES_Class, Sonar Scan, Drawing Location/Path of the robot 
#
# Program name      : Smoothing.py
#
# Author            : Jong-Hoon Kim
#
# Date created      : 04/12/2020
#
# Purpose           : 1) Read sample dataset
#      2) apply noisy filtering 
#     3) send message to serial port to plot values 
#           
#
# Revision History  :
#
# Date        Author      Ref    Revision (Date in MMDDYYYY format) 
# MMDDYYYY    name      1  v-xx   revision note. 
#
##########################################################################
##########################################################################
#   Instructional Note:  
#           
# SimpleKalmanFilter class created by Denys Sene, January, 1, 2017. Released under MIT License.         
# SimpleKalmanFilter(e_mea, e_est, q);
# e_mea: Measurement Uncertainty - How much do we expect to our measurement vary
# e_est: Estimation Uncertainty - Can be initilized with the same value as e_mea since the kalman filter will adjust its value.
# q: Process Noise (Variance) - usually a small number between 0.001 and 1 - how fast your measurement moves. Recommended 0.01. Should be tunned to your needs.
##########################################################################*/
 
#include "RES_Smoothing_Data.h"
#include "SimpleKalmanFilter.h"
#include <TinyMPU6050.h>
#include <Servo.h>

#define PIN_SONAR_PING              45
#define PIN_SONAR_ECHO              47
#define PIN_SERVO                   5

#define MIN_SONAR_ANGLE      30
#define MID_SONAR_ANGLE      90
#define MAX_SONAR_ANGLE      150
#define MAX_SONAR_VALUE       350      // e.g. 650 mm is max range 
#define STRID_SONAR_ANGLE     5

const int numReadings = 10;



//SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.001);
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

// imu sensor reading
MPU6050 mpu (Wire);

// create servo object to control a servo
Servo myServo;  


int readings[numReadings];      // the readings from the input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int sonar[2][180];
int sonarAngle  = 30;
int sonarScanDirection = 0;
float prev = 0.0; float now = 0.0; float alpha = 0.99;  float beta = 0.01;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // IMU init
  mpu.Initialize();
//  Serial.println("Starting calibration...");
//  mpu.Calibrate();

  // Sonar Setting
  pinMode(PIN_SONAR_PING, OUTPUT);     pinMode(PIN_SONAR_ECHO, INPUT);
  myServo.attach(PIN_SERVO);  myServo.write(MID_SONAR_ANGLE);// attaches the servo on PIN_SERVO to the servo object and set middle position

  // init for sonar reading
  for (int i = 0; i < 180; i++) {
    sonar[0][i] = 0; sonar[1][i] = 0;
  }



}

void loop() {
  // put your main code here, to run repeatedly:
  // IMU data sending
//  mpu.Execute();
//  Serial.print("imu ");
//  Serial.print((int)(mpu.GetAngZ()));
//  Serial.print(" ");
//  Serial.print((int)(mpu.GetAngX()));
//  Serial.print(" ");
//  Serial.println((int)(mpu.GetAngY()));

  // Servo motor position
  if (sonarScanDirection == 0){ // increase scan angle
    sonarAngle++;
    if (sonarAngle >= 150) sonarScanDirection = 1;     
  }else{
    sonarAngle--;
    if (sonarAngle <= 30) sonarScanDirection = 0;     
    
  }
  myServo.write(sonarAngle);
  delay(50);
  // sonar data sending
  digitalWrite(PIN_SONAR_PING, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_SONAR_PING, HIGH);
  delayMicroseconds(5);
  digitalWrite(PIN_SONAR_PING, LOW);
  int tmpSonarValue = (pulseIn(PIN_SONAR_ECHO, HIGH,5000 ) / 29 / 2) * 5 ;   // mm 
  if (tmpSonarValue == 0) tmpSonarValue = MAX_SONAR_VALUE;
  sonar[1][sonarAngle] = sonar[0][sonarAngle];
  sonar[0][sonarAngle] = tmpSonarValue;
  Serial.print("sonar ");
  Serial.print(sonarAngle);
  Serial.print(" ");
  Serial.print(sonar[1][sonarAngle]); 
  Serial.print(" ");
  Serial.println(sonar[0][sonarAngle]); 
 
 
}
