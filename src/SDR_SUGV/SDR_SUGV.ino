#include "Arduino.h"
#include "SDR_SUGV_04.h"
#include "Protocol.h"
#include "Localization.h"
#include "Sensing.h"
#include "Action.h"

int stopFlag = 0;
int autoFlag = -1;   // Auto or Manual Control

unsigned long sonarClock = 0;
unsigned long lineClock = 0;
unsigned long odometerClock = 0;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

void setup() {
  // put your setup code here, to run once:
  initialization();
}


void loop() {
  currentMillis = millis();
  updateCommand();
  updateSensors();  
  updatePlan();
  updateAction();
  previousMillis = currentMillis;  
}

void updatePlan(){
  if (autoFlag == 1){ //Auto-driving    
  }else if (autoFlag == 0){
  }else{
  }
  if (stopFlag ){
    speedR = 0; speedL = 0;
  }
}

void updateAction(){

  updateMotor();
}



void updateSensors(){
  updateLineSensor(); 
  sonar = updateSonar();
}





void initialization(){
  currentMillis = millis();
  init_communication();
  init_pin();
  
}

void init_communication(){
  Serial.begin(115200); Serial.println("Program Start...v04");
  init_buffer(); 
}

void init_pin(){
  //Motor pin setting
  pinMode(MOTOR_LEFT_ENABLE, OUTPUT); pinMode(MOTOR_LEFT_A, OUTPUT); pinMode(MOTOR_LEFT_B, OUTPUT);
  pinMode(MOTOR_RIGHT_ENABLE, OUTPUT); pinMode(MOTOR_RIGHT_A, OUTPUT); pinMode(MOTOR_RIGHT_B, OUTPUT);
  //Sonar pin setting
  pinMode(SONAR_ECHO, INPUT); pinMode(SONAR_TRIG, OUTPUT);
  pinMode(LINE_LEFT, INPUT_PULLUP); pinMode(LINE_RIGHT, INPUT_PULLUP); 
  attachInterrupt(0, rightOdometer, CHANGE);
  attachInterrupt(1, leftOdometer, CHANGE);
}



