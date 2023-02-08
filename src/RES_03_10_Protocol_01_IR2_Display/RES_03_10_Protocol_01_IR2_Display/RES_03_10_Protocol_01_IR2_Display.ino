/**********************************************************************;
* Project           : RES_Class, Communication, IR_Remote_Key_Display, RES_Protocol_Design, 
*
* Program name      : RES_03_10_Protocol_01_IR2_Display.ino
*
* Author            : Mohammad Insanur Rahman Shuvo
*
* Date created      : 02/06/2023
*
* Purpose           : How to display 0-9 in a seven segment display when that corresponding key will be pressed in the IR remote and create a message frame using user defined protocol by taking the input from IR remote. 
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/

#include "Protocol_IR_Display.h"
#include <IRremote.h>
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

void setup() {
  // put your setup code here, to run once:
  initialization();
 
}

void loop() {
  currentMillis = millis();
  updateCommand();
  previousMillis = currentMillis;  
}

void initialization(){
  currentMillis = millis();
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
