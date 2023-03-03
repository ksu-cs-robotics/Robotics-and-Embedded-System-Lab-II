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
#include "Queue.h"
#include "MyProtocol.h"

IRsend irsend;

int joyStick[]={0,0,0};
int servoSelection = 0;
int buttonFlag = 0;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long joystickUpdateTime = 0;
unsigned long joystickSendingTime = 0;
unsigned long IRSendingTime = 0;

char msgBuffer[MESSAGE_BUFFER_SIZE];
int msgBufferPointer = 0;
Queue<char> myQueue(MESSAGE_QUEUE_SIZE);

void setup(){
  initCommunication();
  initPinMode();  
  joystickUpdateTime = millis()+INTERVAL_JOYSTICK;
  joystickSendingTime = millis()+INTERVAL_JOYSTICK_VALUE;
}


void loop() {
  currentMillis = millis();
  sensing();
  thinking();
  acting();
  previousMillis = currentMillis; 
//  delay(10);
}

void sensing(){
  updateJoystick();
  checkButton();
}
void thinking(){
  checkSerialMessage();
}
void acting(){
  sendMessage();
}

void checkSerialMessage(){
  while (Serial.available()>0){
    char tmpChar = Serial.read();  
//    Serial.print(tmpChar);  
    if (myQueue.count()< MESSAGE_QUEUE_SIZE){
      myQueue.push(tmpChar);
    }else{
      Serial.println("Message Queue is Full... Slowdown...");
    }
  }
}



bool pushIntValue(int argInt){
  if (myQueue.count()< MESSAGE_QUEUE_SIZE){
    String tmpIntValue = String(argInt);
    char* char_array = new char[tmpIntValue.length()];
    strcpy(char_array, tmpIntValue.c_str());
    for(int i = 0; i < tmpIntValue.length(); i++) 
      myQueue.push(char_array[i]);
  }
  return false;
}

  
void sendMessage(){
  if ((myQueue.count() > 0)&& (IRSendingTime < currentMillis )){
    char tmpChar = myQueue.pop();
    Serial.print(tmpChar);
    irsend.sendNEC(convertCharRemoteLong(tmpChar), 32); 
    IRSendingTime = currentMillis + INTERVAL_IRSEND;
  }
}

void setJoyValue(){
  if ((joystickSendingTime < currentMillis) && (myQueue.count() <= 0)){
    myQueue.push(FRAME_START_SYMBOL); myQueue.push(FRAME_MODE_SYMBOL); myQueue.push(FRAME_DELIMITER_SYMBOL);
    myQueue.push(MSG_SET_SERVO);
    myQueue.push(FRAME_DELIMITER_SYMBOL);
    pushIntValue(joyStick[0]);
    myQueue.push(FRAME_DELIMITER_SYMBOL);
    pushIntValue(joyStick[1]);
    myQueue.push(FRAME_DELIMITER_SYMBOL);
    pushIntValue(joyStick[2]);
    myQueue.push(FRAME_END_SYMBOL);
  }
}

void initCommunication(){
  Serial.begin(9600);
  Serial.println("Sender v00");  
}

void initPinMode(){
  pinMode(PIN_JOYSTICK_X, INPUT); pinMode(PIN_JOYSTICK_Y, INPUT);  pinMode(PIN_JOYSTICK_SW, INPUT_PULLUP);  //Pinmode setup  
}


void updateJoystick(){
  if (joystickUpdateTime < currentMillis){
    joyStick[0] = map(analogRead(PIN_JOYSTICK_X), 0, 1024, 0, 180);
    joyStick[1] = map(analogRead(PIN_JOYSTICK_Y), 0, 1024, 0, 180);
    joyStick[2] = digitalRead(PIN_JOYSTICK_SW); 
    joystickUpdateTime = currentMillis + INTERVAL_JOYSTICK;
//    Serial.println(joyStick[servoSelection]);
  }
}




void checkButton(){
  if ((digitalRead(PIN_JOYSTICK_SW) == LOW) && (buttonFlag == 0) ){
    buttonFlag = 1;
    servoSelection++;
    if (servoSelection > 2) servoSelection =0;
  }else if ((digitalRead(PIN_JOYSTICK_SW) == HIGH) && (buttonFlag == 1) ){
    //Serial.println(servoSelection);
    setJoyValue();
    buttonFlag = 0;
  }
}
