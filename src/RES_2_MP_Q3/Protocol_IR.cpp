#include "Arduino.h"
#include "Protocol_IR_Display.h"
#include "Segment.h"
#include "Digit.h"
#include "Display.h"
#include "IRremote.h"


char msgBuffer[MESSAGE_BUFFER_SIZE]; 
int msgBufferPointer = 0;
int RECEIVER_PIN = A0; //initialize the receiver pin
IRrecv receiver(RECEIVER_PIN); // create a receiver object of the IRrecv class
unsigned long key_value = 0;
char charBuffer[2]; //this buffer is used to hold the corresponding IR remote keys value to check whether one of them contains a valid character
char tmpChar;
int index=0;
String tokenizedString[3]= {"","",""};//empty buffer to hold the data from the frame
int servoAngle[3] = {0,0,0};
int motorSpeed[2] = {0,0};

decode_results results; 
remoteMessageMap remoteMessagemapArray[22]; // this array is used to map the IR remote key to a specific character
remoteMessageMap remoteMessagemapArray2[] = {{16753245,'p'},{16736925,'v'},{16769565,'f'},{16720605,'<'},{16712445,' '},{16761405,'>'},{16769055,'/'},{16754775,'-'},
{16748655 ,'#'},{ 16738455,'0'},{16750695 ,'@'},{ 16756815,'$'},{ 16724175,'1'},{16718055 ,'2'},{16743045 ,'3'},{16716015 ,'4'},{16726215 ,'5'},{16734885 ,'6'},
{ 16728765,'7'},{ 16730805,'8'},{16732845 ,'9'},{4294967295 ,'I'}};
  

const byte digit_pins[4] {2, 3, 4, 5}; // From com1 - com4
const byte segment_pins[7] {6, 7, 8, 9, 10, 11, 12}; // From a - g (abc...g)
const byte dp_pin = 13;
Display d(digit_pins, segment_pins, dp_pin);

void evaluateStringCommand();
void evaluateBinaryCommand();
void echoCommand();
void getServoCommand();
void setServoCommand();
void getSpeedCommand();
void setSpeedCommand();
void getServoBinaryCommand();
void setServoBinaryCommand();
void getSpeedBinaryCommand();
void setSpeedBinaryCommand();
bool checkCRC();

void init_receiver(){
  receiver.enableIRIn(); // enable the receiver
  receiver.blink13(true);
}
void init_pin_mode(){
  pinMode(2, OUTPUT);  
  pinMode(3, OUTPUT);  
  pinMode(4, OUTPUT);  
  pinMode(5, OUTPUT);  
  pinMode(6, OUTPUT);  
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);  
  pinMode(9, OUTPUT);  
  pinMode(10, OUTPUT);  
  pinMode(11, OUTPUT);  
  pinMode(12, OUTPUT);  
  pinMode(13, OUTPUT);
}


void init_buffer(){
  //this function initialize the buffer to 0x00 and buffer pointer to 0
  for (int i = 0; i < MESSAGE_BUFFER_SIZE; i++){
    msgBuffer[i] = 0x00;
  }
  msgBufferPointer = 0;
}

char convertRemoteSymboltoChar2(){
  //this function returns a specific character based on the map table when a button in the remote is pressed
  for (int i=0;i<22;i++){
    if (remoteMessagemapArray2[i].value == results.value){
      return remoteMessagemapArray2[i].charRemote;
    }    
  }
  return 0x00;
}

getValidchar readRemotechar2(){
  getValidchar vChar;
  char remoteChar = convertRemoteSymboltoChar2();
  vChar.validChar = remoteChar;
  if ((remoteChar == 'I') | (remoteChar == 0x00)){
    vChar.checkValidchar = false;
//    Serial.print("no{"); Serial.print(remoteChar); Serial.println("}");
    return vChar;      
  }else{
    vChar.checkValidchar = true;
//    Serial.print("ok{"); Serial.print(remoteChar); Serial.println("}");
    return vChar;  
  }
}


void printTmpchar(){
  //this function simply print the IR remote key value to the serial monitor
  Serial.print('[');
  Serial.print(tmpChar);
  Serial.println(']');
}



void updateCommand(){
  if (receiver.decode(&results)){
//    getValidchar remoteChar =readRemotechar();
    getValidchar remoteChar =readRemotechar2();
//    Serial.print("[[["); Serial.print(results.value);Serial.println("]]]");
    if(remoteChar.checkValidchar == true){
      tmpChar = remoteChar.validChar; //tmpChar holds the mapped character for the IR remote key which is currently pressed
      printTmpchar(); 
      if ((msgBufferPointer == 0)&&(tmpChar == FRAME_START_SYMBOL)){           // '<' 0x3C 
        msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++; 
      }
      else if (msgBufferPointer == 1){
        if  ((tmpChar == FRAME_MODE_SYMBOL)||(tmpChar == '!')){     // '!'  (0x21 ==> Binary Format)or '@' (0x40 ==> String format)
          msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++;   
        }
        else{
          msgBufferPointer = 0;
        }
      }
      else if (msgBufferPointer == 2){
        if (tmpChar ==  FRAME_DELIMITER_SYMBOL){     // ' ' (0x20 ==> Space)
          msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++;      
        } 
        else{
          msgBufferPointer = 0;
        }
      } 
      else if (msgBufferPointer > MESSAGE_BUFFER_SIZE){ 
        msgBufferPointer = 0; 
        if ( tmpChar == FRAME_END_SYMBOL){
          msgBuffer[0] = tmpChar;    
          msgBufferPointer = 1;          
        }
      }
      else if (msgBufferPointer > 2){
        if (( msgBuffer[1] == FRAME_MODE_SYMBOL)&& (tmpChar == FRAME_END_SYMBOL)){
          echoCommand();
          evaluateStringCommand();
          msgBufferPointer = 0;        
        }
        else if  (( msgBuffer[1] == '!')&& (msgBuffer[3] == msgBufferPointer )){
          evaluateBinaryCommand();
          msgBufferPointer = 0;
        }
        else{
          msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++;
        } 
      }
        
    }
    receiver.resume();
  }

}

void printLED(){
  
  d.print(tokenizedString[2].c_str());
}

void evaluateStringCommand(){
  if (msgBuffer[3] == MSG_GET_ECHO)   echoCommand();
  else if (msgBuffer[3] == MSG_GET_SERVO) getServoCommand();
  else if (msgBuffer[3] == MSG_SET_SERVO) setServoCommand();
  else if (msgBuffer[3] == MSG_GET_SPEED) getSpeedCommand();
  else if (msgBuffer[3] == MSG_SET_SPEED) setSpeedCommand();
  
}

bool checkCRC(){
  byte crcVal=0;
  for (int i =0; i < msgBufferPointer; i++){
    crcVal += msgBuffer[i];
  }
  if (crcVal == msgBuffer[msgBufferPointer]){
    return true;
  }else{
    return false;
  }
}

//size of message = 1byte
//three servo values = 2byte(int)+ 2 byte (int) + 2 byte (int)
//
//'<''!'' '0x0B(int servo1)(int servo2)(int servo3)(one byte CRC) 
//
//byte testMessage[]={'<','!',' ',0x0B,0x00,0x07,0xB0 

void evaluateBinaryCommand(){
  if (checkCRC()){
    if (msgBuffer[4] == MSG_GET_ECHO)   echoCommand();
    else if (msgBuffer[4] == MSG_GET_SERVO) getServoBinaryCommand();
    else if (msgBuffer[4] == MSG_SET_SERVO) setServoBinaryCommand();
    else if (msgBuffer[4] == MSG_GET_SPEED) getSpeedBinaryCommand();
  //  else if (msgBuffer[4] == MSG_SET_SPEED) setSpeedBinaryCommand();
  }else{
    Serial.println(" CRC Error ");
  }
}

void setServoBinaryCommand(){
  servoAngle[0] = msgBuffer[5];
  servoAngle[0] +=(uint16_t)msgBuffer[6] << 8; 
  servoAngle[1] = msgBuffer[7];
  servoAngle[1] +=(uint16_t)msgBuffer[8] << 8; 
  servoAngle[2] = msgBuffer[9];
  servoAngle[2] +=(uint16_t)msgBuffer[10] << 8; 
}

void makeServoBinaryCommand(){
  int servo1  = 1000;  // 0b 0000 0111 1101 0000
  
  byte temp1 = servo1 << 8; //0b 1101 0000 0000 0000
  
  byte temp2 = servo1 >> 8; //0b 0000 0000 0000 0111 
  byte servo2Byte [2];
  servo2Byte[0] = temp1;
  servo2Byte[1] = temp2;
  
  int servo2 = 1001;
  int servo3 = 1002;
  
}


void getServoBinaryCommand(){
  getServoCommand();
}
void getSpeedBinaryCommand(){
  getSpeedCommand();
}

void getServoCommand(){
  Serial.print("Serve Poistion: ");
  Serial.print(servoAngle[0]); Serial.print(":");
  Serial.print(servoAngle[1]); Serial.print(":");
  Serial.println(servoAngle[2]);
}

void getSpeedCommand(){
  Serial.print("Motor Speed: ");  Serial.print(motorSpeed[0]); Serial.print(":");
  Serial.println(motorSpeed[1]);

}

void setServoCommand(){
 char *p = msgBuffer;
 String str;
 int cnt =0;
 while ((str = strtok_r(p, " ", &p)) != NULL){ // delimiter is the space
    if (cnt == 0){ 
    }else if(cnt == 1){         
    }else if(cnt == 2){         
      servoAngle[0] = str.toInt();
    }else if (cnt == 3){   
      servoAngle[1] = str.toInt();
    }else if (cnt == 4){    
      servoAngle[2] = str.toInt();
    }
    cnt++; 
 }
}

void setSpeedCommand(){
 char *p = msgBuffer;
 String str;
 int cnt =0;
 while ((str = strtok_r(p, " ", &p)) != NULL){ // delimiter is the space
    if (cnt == 0){ 
    }else if(cnt == 1){         
    }else if(cnt == 2){         
      motorSpeed[0] = str.toInt();
    }else if (cnt == 3){   
      motorSpeed[1] = str.toInt();
    }
    cnt++; 
 }
}


void echoCommand(){
  Serial.print("[");
  for(int i = 0; i <= msgBufferPointer; i++){
    Serial.print(msgBuffer[i]);
  }
  Serial.println("]");
}
