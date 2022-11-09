#include "Arduino.h"
#include "Protocol.h"

char msgBuffer[MESSAGE_BUFFER_SIZE]; 
int msgBufferPointer = 0;

int joyThrottle ; // 
int joyYaw ; // 
int joyRoll ; // 
int joyPitch ; // 

void evaluateStringCommand();
void evaluateBinaryCommand();
void echoCommand();

void init_buffer(){
  for (int i = 0; i < MESSAGE_BUFFER_SIZE; i++){
    msgBuffer[i] = 0x00;
  }
  msgBufferPointer = 0;
}


void updateCommand(){
  if (Serial.available() > 0){
    char tmpChar = Serial.read();
    if ((msgBufferPointer == 0)&&(tmpChar == '<')){           // '<' 0x3C 
      msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++;
    }else if (msgBufferPointer == 1){
      if  ((tmpChar == '@')||(tmpChar == '!')){     // '!'  (0x21 ==> Binary Format)or '@' (0x40 ==> String format)
        msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++;      
      }else{
        msgBufferPointer = 0;
      }
    }else if (msgBufferPointer == 2){
      if (tmpChar == ' '){     // ' ' (0x20 ==> Space)
        msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++;      
      }else{
        msgBufferPointer = 0;
      }
    }else if (msgBufferPointer > MESSAGE_BUFFER_SIZE){ 
      msgBufferPointer = 0; 
      if ( tmpChar == '<') {
        msgBuffer[0] = tmpChar;    
        msgBufferPointer = 1;          
      }
    }else if (msgBufferPointer > 2) {
      if (( msgBuffer[1] == '@')&& (tmpChar == '>')) {
        //echoCommand();
        evaluateStringCommand();
        msgBufferPointer = 0;        
      }else if  (( msgBuffer[1] == '!')&& (msgBuffer[3] == msgBufferPointer )) {
        evaluateBinaryCommand();
        msgBufferPointer = 0;
      }else{
        msgBuffer[msgBufferPointer] = tmpChar;
        msgBufferPointer++;
      }
    }
  }  
}




void evaluateStringCommand(){
  if (msgBuffer[3] == MSG_GET_ECHO)   echoCommand();
  else if (msgBuffer[3] == MSG_GET_STATUS)  statusCommand();
  else if (msgBuffer[3] == MSG_SET_JOYINPUT) setJoyInput();
  
}
void evaluateBinaryCommand(){
  Serial.print("N/A-");
  echoCommand();
}

void echoCommand(){
  Serial.print("[");
  for(int i = 0; i <= msgBufferPointer; i++){
    Serial.print(msgBuffer[i]);
  }
  Serial.println("]");
}

void statusCommand(){
//  Serial.print("[");Serial.print(autoFlag); Serial.print("="); Serial.print(mSpeedR);Serial.print(" : "); Serial.print(mSpeedL); 
//  Serial.print("  +  ");Serial.print(odometerR); Serial.print(" : "); Serial.print(odometerL); Serial.print(" + "); 
//  Serial.print(lineR); Serial.print(" : "); Serial.print(lineL); Serial.print(" + ");  Serial.print(sonar); Serial.println("]"); 
}

void setJoyInput(){
}
