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
*
* Protocol Structure : Head '<', '@', ','
*                      Tail '#', '>'
*                      Delimiter ','
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/

#ifndef Protocol_h
#define Protocol_h

#include "Config.h"

#define RESET_MSG           'r'
#define ODOM_MSG            'o'  // odom values [ left, right] 
#define POSE_MSG            'p'  // pose: x, y, orientation  
#define VEL_MSG             'c'  // control message (liner_vel, angular_vel)
#define JOY_MSG             'j'  // joystic  message (x, y)
#define GENERAL_MSG         'g'
#define SCAN_MSG            's'  // ridar scan data

#define DEBUG_MSG           1           // 0 is off and 1 is on

#define MESSAGE_BUFFER_SIZE     128

void evaluateMessage(Stream *serialport, char msgBuffer[]);
void updateCmd_Vel(Stream *serialport, char msgBuffer[]);
void updateJoystick(Stream *serialport, char msgBuffer[]);
void updateReset(Stream *serialport, char msgBuffer[]);
void updatePose(Stream *serialport, char msgBuffer[]);
void sendCommand(Stream *serialport,String data[], int size, String type);




void checkSerialMessage1(Stream *serialport){ 
  if (serialport->available() > 0){
    char tmpChar = serialport->read();
    serialport->println( tmpChar);
  }
}

void checkSerialMessage(Stream *serialport, char msgBuffer[], int *msgBufferPointer){  
  if (serialport->available() > 0){
    char tmpChar = serialport->read();
//    serialport->print(tmpChar); serialport->print(" ");
//    serialport->println(*msgBufferPointer);    
    if ((*msgBufferPointer == 0) && (tmpChar == '<')){
      msgBuffer[*msgBufferPointer] = tmpChar; 
      *msgBufferPointer += 1;
    }     else if (*msgBufferPointer == 1){
      if (tmpChar == '@'){ 
        msgBuffer[*msgBufferPointer] = tmpChar; 
        *msgBufferPointer += 1; 
      }
      else{
        if (tmpChar == '<'){
          msgBuffer[0] = tmpChar;
          *msgBufferPointer = 1;
        }else{
          *msgBufferPointer = 0;          
        }
      }
    }     else if (*msgBufferPointer >= 2){
      if (tmpChar == '<'){ 
        msgBuffer[0] = tmpChar;
        *msgBufferPointer = 1;
      }
      else if (tmpChar == '>'){
        msgBuffer[*msgBufferPointer] = tmpChar;
        *msgBufferPointer = 0;
//          serialport->println("got it---");
          evaluateMessage(serialport, msgBuffer);
      }
      else{
        if (*msgBufferPointer >= (MESSAGE_BUFFER_SIZE-1)){
//          serialport->println("buffer overflow error---");
          *msgBufferPointer= 0;
        }else{
          msgBuffer[*msgBufferPointer] = tmpChar;
          *msgBufferPointer += 1;
       }
      }
    }

  }
}

void evaluateMessage(Stream *serialport, char msgBuffer[]){
//  Serial.println(msgBuffer);
  if (msgBuffer[3] == VEL_MSG){
    updateCmd_Vel(serialport, msgBuffer);
  }else if (msgBuffer[3] == JOY_MSG){
    updateJoystick(serialport, msgBuffer);
  }else if (msgBuffer[3] == POSE_MSG){
    updatePose(serialport, msgBuffer);
  
  }else if (msgBuffer[3] == RESET_MSG){
    updateReset(serialport, msgBuffer);
  }
}

void updateJoystick(Stream *serialport, char msgBuffer[]){
//  Serial.println(msgBuffer);
  char* command = strtok(msgBuffer, ",");
  int temp = 0;
  while (command != 0){
    if (temp == 2){
        joystick[0] = String(command).toInt();
//        Serial.print("j0=");Serial.print(command);Serial.print("=");Serial.println(joystick[0]);
    }
    else if (temp == 3){
        joystick[1] = String(command).toInt();
//        Serial.print("j1=");Serial.print(command);Serial.print("=");Serial.println(joystick[1]);
    }
    temp += 1;
    command = strtok(0, ",");
  }
}

void updateCmd_Vel(Stream *serialport, char msgBuffer[]){
  char* command = strtok(msgBuffer, ",");
  int temp = 0;
  while (command != 0){
    if (temp == 2){
        cmd_vel[LINEAR] = String(command).toFloat();
    }
    else if (temp == 3){
        cmd_vel[ANGULAR] = String(command).toFloat();
    }
    temp += 1;
    command = strtok(0, ",");
  }
}

void updatePose(Stream *serialport, char msgBuffer[]){
  char* command = strtok(msgBuffer, ",");
  int temp = 0;
  while (command != 0){
    if (temp == 2){
        pose[0] = String(command).toFloat();
    }
    else if (temp == 3){
        pose[1] = String(command).toFloat();
    }
    else if (temp == 4){
        pose[2] = String(command).toFloat();
    }
    temp += 1;
    command = strtok(0, ",");
  }
}

void updateScan(Stream *serialport, char msgBuffer[]){
  char* command = strtok(msgBuffer, ",");
  int temp = 0;
  while (command != 0){
    if (temp == 2){
        scanned_point[0] = String(command).toFloat();
    }
    else if (temp == 3){
        scanned_point[1] = String(command).toFloat();
    }
    temp += 1;
    command = strtok(0, ",");
    
  }
}


void updateReset(Stream *serialport, char msgBuffer[]){
  char* command = strtok(msgBuffer, ",");
  int temp = 0;
  while (command != 0){
    if (temp == 3){
        serialport->println(String(command));
    }
    else if (temp == 4){
        serialport->println(String(command));
    }
    temp += 1;
    command = strtok(0, ",");
  }
}





void sendCommand(Stream *serialport,String data[], int size, String type){
  String data_out;
  data_out = "<@," + type + "," ;
  for(int i = 0; i < size; i++){
    data_out += data[i];
    data_out += ",";
  }
  data_out += "#>";

  serialport->print(data_out);
  serialport->print("\n");
}




#endif
