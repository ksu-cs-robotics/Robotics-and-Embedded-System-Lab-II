#include "Arduino.h"
#include "Protocol_IR_Display.h"
#include <IRremote.h>

char msgBuffer[MESSAGE_BUFFER_SIZE]; 
int msgBufferPointer = 0;
int RECEIVER_PIN = A0; //initialize the receiver pin
IRrecv receiver(RECEIVER_PIN); // create a receiver object of the IRrecv class
unsigned long key_value = 0;
char charBuffer[2]; //this buffer is used to hold the corresponding IR remote keys value to check whether one of them contains a valid character
char tmpChar;
int index=0;
String tokenizedString[3]= {"","",""};//empty buffer to hold the data from the frame
int servoSpeed[3] = {0,0,0};
decode_results results; 
remoteMessageMap remoteMessagemapArray[22]; // this array is used to map the IR remote key to a specific character
remoteMessageMap remoteMessagemapArray2[] = {{16753245,'p'},{16736925,'v'},{16769565,'f'},{16720605,'<'},{16712445,' '},{16761405,'>'},{16769055,'/'},{16754775,'-'},
{16748655 ,'#'},{ 16738455,'0'},{16750695 ,'@'},{ 16756815,'$'},{ 16724175,'1'},{16718055 ,'2'},{16743045 ,'3'},{16716015 ,'4'},{16726215 ,'5'},{16734885 ,'6'},
{ 16728765,'7'},{ 16730805,'8'},{16732845 ,'9'},{4294967295 ,'I'}};

const byte digit_pins[4] {2, 3, 4, 5}; // From com1 - com4
const byte segment_pins[7] {6, 7, 8, 9, 10, 11, 12}; // From a - g (abc...g)
const byte dp_pin = 13;

void evaluateStringCommand();
void evaluateBinaryCommand();
void echoCommand();
void getServoCommand();
void setServoCommand();


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

void message_mapping(){
  //this function maps each key of the IR remote to a specific character
  remoteMessagemapArray[0].value = 16753245;
  remoteMessagemapArray[0].charRemote = 'p';
  remoteMessagemapArray[1].value =                                                                                                                                         16736925;
  remoteMessagemapArray[1].charRemote = 'v';
  remoteMessagemapArray[2].value =                                                                                                                                         16769565;
  remoteMessagemapArray[2].charRemote = 'f';
  remoteMessagemapArray[3].value =                                                                                                                                         16720605;
  remoteMessagemapArray[3].charRemote = '<';
  remoteMessagemapArray[4].value =                                                                                                                                         16712445;
  remoteMessagemapArray[4].charRemote = ' ';
  remoteMessagemapArray[5].value =                                                                                                                                         16761405;
  remoteMessagemapArray[5].charRemote = '>';
  remoteMessagemapArray[6].value =                                                                                                                                         16769055;
  remoteMessagemapArray[6].charRemote = '/';
  remoteMessagemapArray[7].value =                                                                                                                                         16754775;
  remoteMessagemapArray[7].charRemote = '-';
  remoteMessagemapArray[8].value =                                                                                                                                         16748655;
  remoteMessagemapArray[8].charRemote = '#';
  remoteMessagemapArray[9].value =                                                                                                                                         16738455;
  remoteMessagemapArray[9].charRemote = '0';
  remoteMessagemapArray[10].value =                                                                                                                                         16750695;
  remoteMessagemapArray[10].charRemote = '@';
  remoteMessagemapArray[11].value =                                                                                                                                         16756815;
  remoteMessagemapArray[11].charRemote = '$';
  remoteMessagemapArray[12].value =                                                                                                                                         16724175;
  remoteMessagemapArray[12].charRemote = '1';
  remoteMessagemapArray[13].value =                                                                                                                                         16718055;
  remoteMessagemapArray[13].charRemote = '2';
  remoteMessagemapArray[14].value =                                                                                                                                         16743045;
  remoteMessagemapArray[14].charRemote = '3';
  remoteMessagemapArray[15].value =                                                                                                                                         16716015;
  remoteMessagemapArray[15].charRemote = '4';
  remoteMessagemapArray[16].value =                                                                                                                                         16726215;
  remoteMessagemapArray[16].charRemote = '5';
  remoteMessagemapArray[17].value =                                                                                                                                         16734885;
  remoteMessagemapArray[17].charRemote = '6';
  remoteMessagemapArray[18].value =                                                                                                                                         16728765;
  remoteMessagemapArray[18].charRemote = '7';
  remoteMessagemapArray[19].value =                                                                                                                                         16730805;
  remoteMessagemapArray[19].charRemote = '8';
  remoteMessagemapArray[20].value =                                                                                                                                         16732845;
  remoteMessagemapArray[20].charRemote = '9';
  remoteMessagemapArray[21].value =                                                                                                                                         4294967295;
  remoteMessagemapArray[21].charRemote = 'I';
}
void init_buffer(){
  //this function initialize the buffer to 0x00 and buffer pointer to 0
  for (int i = 0; i < MESSAGE_BUFFER_SIZE; i++){
    msgBuffer[i] = 0x00;
  }
  msgBufferPointer = 0;
}

//int aaa = {{'A',10},{{'B',20}{{'C',30}}
char convertRemoteSymboltoChar(){
  //this function returns a specific character based on the map table when a button in the remote is pressed
  for (int i=0;i<22;i++){
    if (remoteMessagemapArray[i].value == results.value){
      return remoteMessagemapArray[i].charRemote;
    }
    
  }
}

long convertCharRemoteLong(char argChar){
  //this function returns a specific character based on the map table when a button in the remote is pressed
  for (int i=0;i<22;i++){
    if (remoteMessagemapArray[i].charRemote == argChar){
      return remoteMessagemapArray[i].value;
    }
    
  }
}

getValidchar readRemotechar(){
  //this function returns the valid character only once when a button is pressed in the IR remote
  getValidchar vChar;
  char remoteChar = convertRemoteSymboltoChar();
  charBuffer[index]=remoteChar;
  index++;
  index = index % 2;
    
  int prevIndex = ((index - 1) == -1 ? 1 : (index-1));
    
  if(charBuffer[index] == 'I' && charBuffer[prevIndex] != 'I'){
    vChar.validChar = charBuffer[prevIndex];
    vChar.checkValidchar = true;
    return vChar;
  } 
  vChar.checkValidchar = false;
  return vChar;
}

void printTmpchar(){
  //this function simply print the IR remote key value to the serial monitor
  Serial.print('[');
  Serial.print(tmpChar);
  Serial.println(']');
}



void updateCommand(){
  if (receiver.decode(&results)){
    getValidchar remoteChar =readRemotechar();
    if(remoteChar.checkValidchar == true){
      tmpChar = remoteChar.validChar; //tmpChar holds the mapped character for the IR remote key which is currently pressed
      printTmpchar(); 
      if ((msgBufferPointer == 0)&&(tmpChar == '<')){           // '<' 0x3C 
        msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++; 
      }
      else if (msgBufferPointer == 1){
        if  ((tmpChar == '@')||(tmpChar == '!')){     // '!'  (0x21 ==> Binary Format)or '@' (0x40 ==> String format)
          msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++;   
        }
        else{
          msgBufferPointer = 0;
        }
      }
      else if (msgBufferPointer == 2){
        if (tmpChar == ' '){     // ' ' (0x20 ==> Space)
          msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++;      
        } 
        else{
          msgBufferPointer = 0;
        }
      } 
      else if (msgBufferPointer > MESSAGE_BUFFER_SIZE){ 
        msgBufferPointer = 0; 
        if ( tmpChar == '<'){
          msgBuffer[0] = tmpChar;    
          msgBufferPointer = 1;          
        }
      }
      else if (msgBufferPointer > 2){
        if (( msgBuffer[1] == '@')&& (tmpChar == '>')){
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
  
 // d.print(tokenizedString[2].c_str());
}

void evaluateStringCommand(){
 
  if (msgBuffer[3] == MSG_GET_ECHO)   echoCommand();
  else if (msgBuffer[3] == MSG_GET_SERVO) getServoCommand();
  else if (msgBuffer[3] == MSG_SET_SERVO) setServoCommand();
  else if (msgBuffer[3] == MSG_EVALUATE_BINARY) evaluateBinaryCommand();
  
}
void evaluateBinaryCommand(){
  Serial.println("N/A-");
  //echoCommand();
}

void getServoCommand(){
  Serial.print(servoSpeed[0]); Serial.print(":");
  Serial.print(servoSpeed[1]); Serial.print(":");
  Serial.println(servoSpeed[2]);
}

void setServoCommand(){
 char *p = msgBuffer;
 String str;
 int cnt =0;
 while ((str = strtok_r(p, " ", &p)) != NULL){ // delimiter is the space
    if (cnt == 0){ 
    }else if(cnt == 1){         
    }else if(cnt == 2){         
      servoSpeed[0] = str.toInt();
    }else if (cnt == 3){   
      servoSpeed[1] = str.toInt();
    }else if (cnt == 4){    
      servoSpeed[2] = str.toInt();
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
