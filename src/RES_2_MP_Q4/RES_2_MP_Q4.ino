#include "Digit.h"
#include "Display.h"
#include "Protocol_Midterm_Project.h"
#include <IRremote.h>
#include <Servo.h>

#define SERVO_PIN   A3
#define BUTTON_PIN  A2

const byte digit_pins[4] = {2, 3, 4, 5};  // From com1 - com4
const byte segment_pins[7]= {6,7,8,9,10,11,12};  // From a - g (abc...g)
const byte dp_pin = 13; //3;

char buf[5]={'1','2', '.','8', '5'};


int servoSelection = 0;
int buttonFlag = 0;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

Servo myServo;
Display d(digit_pins, segment_pins, dp_pin);
int count = 0;

void setup() {
  initialization();
  Serial.begin(9600);


}

String msg="";

void loop() {
  currentMillis = millis();
  
  sensing();
  thinking();
  acting();
  previousMillis = currentMillis;    
}



void sensing(){
  checkButton();
}

void thinking(){
  
}
void acting(){
  updateServo();
}
void updateServo(){
  myServo.write(servoAngle[servoSelection]);
  d.print(String(servoAngle[servoSelection]));
}


void checkButton(){
  if (Serial.available() > 0){
    char tmpChar = Serial.read();
    if (tmpChar == '1'){
      servoSelection = 0;
    }else if (tmpChar == '2'){
      servoSelection = 1;
    }else if (tmpChar == '3'){
      servoSelection = 2;      
    }
    Serial.print(msg);
    Serial.print(":");
    Serial.print(servoSelection);
    Serial.print("=");
    Serial.println(servoAngle[servoSelection]);
  }

  if ((digitalRead(BUTTON_PIN) == LOW) && (buttonFlag == 0) ){
    digitalWrite(13,HIGH);
    buttonFlag = 1;
    servoSelection++;
    if (servoSelection > 2) servoSelection =0;
  }else {
    digitalWrite(13,LOW);
    if ((digitalRead(BUTTON_PIN) == HIGH) && (buttonFlag == 1) ){
      buttonFlag = 0;
    }
  }
}


void initialization(){
  currentMillis = millis();
  myServo.attach(SERVO_PIN); 
  init_communication();
  init_receiver();
  init_pin();
}

void init_communication(){
  Serial.begin(9600); Serial.println("Program Start...v04");
  init_buffer(); 
}

void init_pin(){  
  pinMode(BUTTON_PIN, INPUT_PULLUP); // digitalWrite(BUTTON_PIN, HIGH);
  pinMode(2, OUTPUT);    pinMode(3, OUTPUT);    pinMode(4, OUTPUT);    pinMode(5, OUTPUT);  
  pinMode(6, OUTPUT);    pinMode(7, OUTPUT);    pinMode(8, OUTPUT);    pinMode(9, OUTPUT);  
  pinMode(10, OUTPUT);     pinMode(11, OUTPUT);    pinMode(12, OUTPUT);    pinMode(13, OUTPUT);  
 
}
