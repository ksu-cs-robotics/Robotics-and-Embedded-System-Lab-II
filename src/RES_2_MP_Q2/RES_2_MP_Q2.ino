#include "Digit.h"
#include "Display.h"
#include "Protocol_Midterm_Project.h"
#include <Servo.h>

#define SERVO_PIN   A3
#define BUTTON_PIN  A2

const byte digit_pins[4] = {2, 3, 4, 5};  // From com1 - com4
const byte segment_pins[7]= {6,7,8,9,10,11,12};  // From a - g (abc...g)
const byte dp_pin = 13; //3;

char buf[5]={'1','2', '.','8', '5'};

int servoValue[]={1000,2000,0};
int servoSelection = 0;
int buttonFlag = 0;

Servo myServo;

int count = 0;
Display d(digit_pins, segment_pins, dp_pin);

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // digitalWrite(BUTTON_PIN, HIGH);
  myServo.attach(SERVO_PIN);
  for (int i = 0; i <= A5; i++) {
    pinMode(i, OUTPUT);
  }
}

String msg="";
void loop() {
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
    Serial.println(servoValue[servoSelection]);
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
  
  msg = String (servoValue[servoSelection]);
  d.print( msg );
  
  delayMicroseconds(10);
}
