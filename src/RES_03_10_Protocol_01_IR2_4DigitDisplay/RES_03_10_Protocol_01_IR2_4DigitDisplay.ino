/**********************************************************************;
* Project           : RES_Class, Communication, IR_Remote_Key_Display, RES_Protocol_Design, 
*
* Program name      : RES_03_10_IR2_4DigitDisplay.ino
*
* Author            : Mohammad Insanur Rahman Shuvo
*
* Date created      : 02/07/2023
*
* Purpose           : How to display a 4 digit number in a seven segment display which will be the last digit of the data frame
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/

#include "Protocol_IR_Display.h"
#include "Segment.h"
#include "Digit.h"
#include "Display.h"
#include <IRremote.h>
#include <Servo.h>

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
Servo myservo; 
extern Display d;
void setup() {
  // put your setup code here, to run once:
  initialization();
 
}

//"<@ 2 1000 2000 1500>"

void loop() {
  currentMillis = millis();
  sensing();
  thinking();
  acting();
  previousMillis = currentMillis;  
}

void sensing(){
  
}
void thinking(){
  updateCommand();
}
void acting(){
  updateServo();
}
void updateServo(){
  myservo.write(servoSpeed[0]);
  d.print(String(servoSpeed[0]));
}

void initialization(){
  currentMillis = millis();
  myservo.attach(A1);   
  init_communication();
  init_pin();  
  init_receiver();
  message_mapping();
  init_pin_mode();
}

void init_communication(){
  Serial.begin(115200); Serial.println("Program Start...v04");
  init_buffer(); 
}

void init_pin(){}
