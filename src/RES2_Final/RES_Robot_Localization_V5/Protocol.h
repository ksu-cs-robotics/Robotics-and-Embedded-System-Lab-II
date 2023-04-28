/**********************************************************************;
* Project           : RES_Class, Localization with Dead Reckoning 
*
* Program name      : Protocol.h
*
* Author            : Jong-Hoon Kim
*
* Date created      : 04/25/2023
*
* Purpose           : Localization of a Simple Rescue Robot 
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/

#ifndef Protocol_h
#define Protocol_h
#include "Config.h"

#define ODOM_MSG                         'o'  // pose x, y, z,  liner vel, angular vel
#define RESET_MSG                        'r'
#define CTL_MSG                          'c'  // control message (liner_vel, angular_vel)
#define GENERAL_MSG                      'g'
#define TORQUE_MSG                       't'


void evaluateMessage();
void updateControl();
void updateReset();

void checkSerialMessage(){
  if (Serial.available() > 0){
    char tmpChar = Serial.read();

    if ((msgBufferPointer == 0) && (tmpChar == '<')){
      msgBuffer[msgBufferPointer] = tmpChar; 
      msgBufferPointer++;
    }
    else if (msgBufferPointer == 1){
      if (tmpChar == '@'){ 
        msgBuffer[msgBufferPointer] = tmpChar; 
        msgBufferPointer++; 
      }
      else{
        if (tmpChar == '<'){
          msgBuffer[0] = tmpChar;
          msgBufferPointer = 1;
        }else{
          msgBufferPointer = 0;          
        }
      }
    }

    else if (msgBufferPointer >= 2){
      if (tmpChar == '<'){ 
        msgBuffer[0] = tmpChar;
        msgBufferPointer = 1;
      }
      else if (tmpChar == '>'){
        msgBuffer[msgBufferPointer] = tmpChar;
        msgBufferPointer = 0;
        evaluateMessage();
      }
      else{
        msgBuffer[msgBufferPointer] = tmpChar;
        msgBufferPointer++;
      }
    }
    Serial.println(msgBufferPointer);
  }
}

void evaluateMessage(){
    Serial.println(msgBuffer[3]);
  if (msgBuffer[3] == CTL_MSG){
    Serial.println("ctl===============");
    updateControl();
  }else if (msgBuffer[3] == RESET_MSG){
    Serial.println("reset=================");
    updateReset();
  }
}

void updateControl(){
  char* command = strtok(msgBuffer, ",");
  int temp = 0;
  while (command != 0){
    if (temp == 2){
        Serial.println(temp);
        Serial.println(String(String(command).toFloat()));
        cmd_vel[LINEAR] = String(command).toFloat();
    }
    else if (temp == 3){
        Serial.println(temp);
        Serial.println(String(String(command).toFloat()));
        cmd_vel[ANGULAR] = String(command).toFloat();
    }
    temp += 1;
    command = strtok(0, ",");
  }
}

void updateReset(){
  char* command = strtok(msgBuffer, ",");
  int temp = 0;
  while (command != 0){
    if (temp == 3){
        Serial.println(String(command));
    }
    else if (temp == 4){
        Serial.println(String(command));
    }
    temp += 1;
    command = strtok(0, ",");
  }
}





void sendCommand(String data[], int size, String type){
  String data_out;
  data_out = "<@," + type + "," + String(size) + ",";
  for(int i = 0; i < size; i++){
    data_out += data[i];
    data_out += ",";
  }
  data_out += "#>";

  Serial.print(data_out);
  Serial.print("\n");
}




#endif
