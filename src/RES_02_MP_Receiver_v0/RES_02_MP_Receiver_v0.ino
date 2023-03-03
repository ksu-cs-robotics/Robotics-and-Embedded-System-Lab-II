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
int joySelection = 0;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long joystickUpdateTime = 0;
unsigned long joystickSendingTime = 0;

char msgBuffer[MESSAGE_BUFFER_SIZE];

void setup(){
  initCommunication();
  initPinMode();  
  initMsgBuffer();
  joystickUpdateTime = millis()+INTERVAL_JOYSTICK;
  joystickSendingTime = millis()+INTERVAL_JOYSTICK;
}

void initCommunication(){
  Serial.begin(9600);
  Serial.println("Receiver v00");  
}

void initPinMode(){
  pinMode(PIN_JOYSTICK_X, INPUT); pinMode(PIN_JOYSTICK_Y, INPUT);  pinMode(PIN_JOYSTICK_SW, INPUT_PULLUP);  //Pinmode setup  
}

void initMsgBuffer(){
  for(int i = 0; i < MESSAGE_BUFFER_SIZE; i++){
    msgBuffer[i]= 0x00;
  }
}

void loop() {
  currentMillis = millis();
  sensing();
  thinking();
  acting();
  previousMillis = currentMillis; 
}

void sensing(){
  updateJoystick();
}
void thinking(){
  checkSerialMessage();
}
void acting(){
  sendingMessage();
}


void updateJoystick(){
  if (joystickUpdateTime < currentMillis){
    joyStick[0] = map(analogRead(PIN_JOYSTICK_X), 0, 1024, 0, 180);
    joyStick[1] = map(analogRead(PIN_JOYSTICK_Y), 0, 1024, 0, 180);
    joyStick[2] = digitalRead(PIN_JOYSTICK_SW); 
    joystickUpdateTime = currentMillis + INTERVAL_JOYSTICK;
    Serial.println(joyStick[joySelection]);
  }
}
