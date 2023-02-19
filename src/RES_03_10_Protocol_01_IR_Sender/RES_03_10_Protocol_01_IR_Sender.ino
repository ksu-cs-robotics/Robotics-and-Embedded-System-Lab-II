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
  Serial.println("ver1--");
  message_mapping();
}

void loop() {
  if (Serial.available() > 0) {
    char a = Serial.read();    
//    irsend.sendSony(0xa90, 12); // Sony TV power code    
    irsend.sendNEC(convertCharRemoteLong1(a), 32); // Sony TV power code
//    irsend.sendNEC(convertCharRemoteLong1(0xFFFFFFFF), 32); // Sony TV power code
    Serial.println(convertCharRemoteLong1(a)); // Sony TV power code
  }
}
long convertCharRemoteLong1(char argChar){
  //this function returns a specific character based on the map table when a button in the remote is pressed
  long out = -1;
  for (int i=0;i<22;i++){
//   Serial.print("["); Serial.print(i); Serial.print("]");Serial.print("{"); Serial.print(argChar); Serial.print("}");
//   Serial.println(remoteMessagemapArray[i].charRemote);
    if (remoteMessagemapArray[i].charRemote == argChar){
//      Serial.println(remoteMessagemapArray[i].charRemote);
      out = remoteMessagemapArray[i].value;
    }
  }
  return out;
}
