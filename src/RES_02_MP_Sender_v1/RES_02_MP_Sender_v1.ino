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
#include "MyProtocol.h"





IRsend irsend;

int joyStick[]={0,0,0};
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long joystickUpdateTime = 0;
unsigned long joystickSendingTime = 0;

char msgBuffer[MESSAGE_BUFFER_SIZE];

void setup(){
  initCommunication();
  initPinMode();  
  joystickUpdateTime = millis()+INTERVAL_JOYSTICK;
  joystickSendingTime = millis()+INTERVAL_JOYSTICK;
}


void loop() {
  currentMillis = millis();
  sensing();
  thinking();
  acting();
  previousMillis = currentMillis;  
  if (Serial.available() > 0) {
    char tmpChar = Serial.read();  
    if (tmpChar ==  'a'){
      echoCommand(); 
    }else if (tmpChar ==  'b'){
      getServoValues();
    }else if (tmpChar ==  'c'){
      setServoValues();
    }else if (tmpChar ==  'd'){
      getCurrentSelection();
    }else if (tmpChar ==  '1'){
      setCurrentSelection(1);
    }else if (tmpChar ==  '2'){
      setCurrentSelection(2);
    }else if (tmpChar ==  '3'){
      setCurrentSelection(3);
    }else if (tmpChar ==  '0'){
      sendValue(1000);
    }else if (tmpChar ==  '9'){
      sendValue(2000);
    }
  }

  updateJoystick();
//  sendJoystickValue();
  previousMillis = currentMillis; 
}


void sensing(){
  
}
void thinking(){
  
}

void acting(){
  
}


void initCommunication(){
  Serial.begin(9600);
  Serial.println("Sender v00");  
}

void initPinMode(){
  pinMode(PIN_JOYSTICK_X, INPUT); pinMode(PIN_JOYSTICK_Y, INPUT);  pinMode(PIN_JOYSTICK_SW, INPUT_PULLUP);  //Pinmode setup  
}

void sendValue(int argInt){
  String b = String(argInt); b = "1000"
  char* char_array = new char[b.length() + 1];  char_array = {0x00, 0x00, 0x00, 0x00, 0x00}
  strcpy(char_array, b.c_str());   
  for (int i = 0; i < b.length()+1; i++){
    irsend.sendNEC(convertCharRemoteLong(char_array[i]), 32); 
    delay(INTERVAL_IRSEND);
  }
  irsend.sendNEC(convertCharRemoteLong(' '), 32); 
  delay(INTERVAL_IRSEND);
}

void updateJoystick(){
  if (joystickUpdateTime < currentMillis){
    joyStick[0] = map(analogRead(PIN_JOYSTICK_X), 0, 1024, 0, 180);
    joyStick[1] = map(analogRead(PIN_JOYSTICK_Y), 0, 1024, 0, 180);
    joyStick[2] = digitalRead(PIN_JOYSTICK_SW); 
    joystickUpdateTime = currentMillis + INTERVAL_JOYSTICK;
    //Serial.println(joyStick[0]);
  }
}

void echoCommand(){
  sendFrameHeader();
  irsend.sendNEC(convertCharRemoteLong(MSG_GET_ECHO), 32); 
  delay(10);
  sendFrameTailer();
}

void sendFrameHeader(){
    irsend.sendNEC(convertCharRemoteLong('<'), 32); 
    delay(10);
    irsend.sendNEC(convertCharRemoteLong('@'), 32); 
    delay(10);
    irsend.sendNEC(convertCharRemoteLong(' '), 32);
    delay(10);  
}
void sendFrameTailer(){
    irsend.sendNEC(convertCharRemoteLong('>'), 32);
    delay(10);  
}

void getServoValues(){
  
}

"<@ 3 joyStick[0] joyStick[1] joyStick[2]>"

void setServoValues(){
  sendFrameHeader();
  irsend.sendNEC(convertCharRemoteLong(MSG_SET_SERVO), 32); 
  delay(10);
  irsend.sendNEC(convertCharRemoteLong(' '), 32); 
  delay(10);
  sendValue(joyStick[0]);
  sendValue(joyStick[1]);
  sendValue(joyStick[2]);  
  sendFrameTailer();
  
}
void getCurrentSelection(){
  
}
void setCurrentSelection(int argSelection){
}




void sendJoystickValue(){
  if (joystickSendingTime < currentMillis){
    irsend.sendNEC(convertCharRemoteLong('<'), 32); 
    delay(10);
    irsend.sendNEC(convertCharRemoteLong('@'), 32); 
    delay(10);
    irsend.sendNEC(convertCharRemoteLong(' '), 32);
    delay(10);
    irsend.sendNEC(convertCharRemoteLong(MSG_SET_SERVO), 32); 
    delay(10);
    irsend.sendNEC(convertCharRemoteLong(' '), 32); 
    delay(10);
    sendValue(joyStick[0]);
    sendValue(joyStick[1]);
    sendValue(joyStick[2]);
    delay(10);
    irsend.sendNEC(convertCharRemoteLong('>'), 32);
    delay(10);
     Serial.print(joystickSendingTime); Serial.print("=");Serial.print(joyStick[0]); Serial.print(":"); Serial.print(joyStick[1]); Serial.print(":"); Serial.println(joyStick[2]); 
    joystickSendingTime = currentMillis + INTERVAL_JOYSTICK;
  }
}
