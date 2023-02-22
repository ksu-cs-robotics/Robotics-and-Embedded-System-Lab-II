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
#include <IRremote.h>
/************************************** ATR Lab Simple UGV Protocol *******************************************************/
#define UGV_VERSION              1

#define MSG_GET_ECHO                 0x31   //'1' in message       message echo         
#define MSG_GET_SERVO                0x32   //'2' in message      
#define MSG_SET_SERVO                0x33   //'3' in message      
#define MSG_GET_SPEED                0x38   //'8' in message      
#define MSG_SET_SPEED                0x39   //'9' in message      

#define MESSAGE_BUFFER_SIZE          64

typedef struct{
  unsigned long value;
  char charRemote;
}remoteMessageMap;


// this array is used to map the IR remote key to a specific character
remoteMessageMap remoteMessagemapArray[] = {{16753245,'p'},{16736925,'v'},{16769565,'f'},{16720605,'<'},{16712445,' '},
                                            {16761405,'>'},{16769055,'/'},{16754775,'-'},{16748655 ,'#'},{ 16738455,'0'},
                                            {16750695 ,'@'},{ 16756815,'$'},{ 16724175,'1'},{16718055 ,'2'},{16743045 ,'3'},
                                            {16716015 ,'4'},{16726215 ,'5'},{16734885 ,'6'},{ 16728765,'7'},{ 16730805,'8'},
                                            {16732845 ,'9'},{4294967295 ,'I'}};



IRsend irsend;

void setup()
{
  Serial.begin(9600);
  Serial.println("ver1--");
}

void loop() {
  if (Serial.available() > 0) {
    char tmpChar = Serial.read();    
    irsend.sendNEC(convertCharRemoteLong(tmpChar), 32); // Sony TV power code
    Serial.println(convertCharRemoteLong(tmpChar)); // Sony TV power code
  }
}





char convertRemoteSymboltoChar(unsigned long argValue){
  //this function returns a specific character based on the map table when a button in the remote is pressed
  for (int i=0;i<22;i++){
    if (remoteMessagemapArray[i].value == argValue){
      return remoteMessagemapArray[i].charRemote;
    }    
  }
  return 0x00;
}

long convertCharRemoteLong(char argChar){
  //this function returns a specific character based on the map table when a button in the remote is pressed
  for (int i=0;i<22;i++){
    if (remoteMessagemapArray[i].charRemote == argChar){
      return remoteMessagemapArray[i].value;
    }    
  }
  return 0;
}
