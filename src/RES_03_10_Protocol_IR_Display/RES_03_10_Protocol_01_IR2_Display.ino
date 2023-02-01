#include "Protocol_IR2_Display.h"
#include <IRremote.h>
unsigned long imuClock = 0;
unsigned long motorClock = 0;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
 // variable to store the key value

void setup() {
  // put your setup code here, to run once:
  initialization();
 
}


void loop() {
  currentMillis = millis();
  updateCommand();
  //updateSensors();  
  //updatePlan();
  //updateAction();
  previousMillis = currentMillis;  
}

void initialization(){
  currentMillis = millis();
  init_communication();
  init_pin();  
  init_receiver();
  message_mapping();
}

void init_communication(){
  Serial.begin(115200); Serial.println("Program Start...v04");
  init_buffer(); 
}

void init_pin(){}

void updatePlan(){}

void updateAction(){}

void updateSensors(){}
