/**********************************************************************;
* Project           : RES_Class, Communication, IR_Remote_Key_Display, RES_Protocol_Design, 
*
* Program name      : RES_03_10_Protocol_01_IR_Sender.ino
*
* Author            : Jong Hoon Kim
*
* Date created      : 02/15/2023
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
#include <IRremote.h>

IRsend irsend;

void setup()
{
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char a = Serial.read();    
//    irsend.sendSony(0xa90, 12); // Sony TV power code
    
    irsend.sendNEC(convertIRMessage(a); 32); // Sony TV power code
    
  }
}

long convertIRMessage(char argChar){
  for (int i = 0; i < 22; i++){
    
  }
}
