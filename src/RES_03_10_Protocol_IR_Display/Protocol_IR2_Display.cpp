#include "Arduino.h"
#include "Protocol_IR2_Display.h"
#include <IRremote.h>
char msgBuffer[MESSAGE_BUFFER_SIZE]; 
int msgBufferPointer = 0;
IRrecv receiver(RECEIVER_PIN); // create a receiver object of the IRrecv class
unsigned long key_value = 0;
char charBuffer[2];
char tmpChar;
int index=0;
decode_results results; 
int joyThrottle ; // 
int joyYaw ; // 
int joyRoll ; // 
int joyPitch ; // 
remoteMessageMap remoteMessagemapArray[22]; 
void evaluateStringCommand();
void evaluateBinaryCommand();
void echoCommand();

void init_receiver(){
  receiver.enableIRIn(); // enable the receiver
  receiver.blink13(true);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(A1, OUTPUT);
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
  for (int i = 0; i < MESSAGE_BUFFER_SIZE; i++){
    msgBuffer[i] = 0x00;
  }
  msgBufferPointer = 0;
  }
char convertRemoteSymboltoChar(){
//  this function returns a specific character based on the map table when a button in the remote is pressed
  for (int i=0;i<22;i++)
  {
    if (remoteMessagemapArray[i].value == results.value){
            return remoteMessagemapArray[i].charRemote;
    }
    
    }
  }
getValidchar readRemotechar(){
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
//this functions below simply show the corresponding number in the seven segment display when they are called
void Anort(){
  digitalWrite(2, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(3, HIGH);
  }
void Aone(){
  digitalWrite(A1, HIGH);
  digitalWrite(7, HIGH);
  }
void Atwo(){
  digitalWrite(2, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  }
void Athree(){
  digitalWrite(2,HIGH);
  digitalWrite(A1,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(6,HIGH);
  }
void Afour(){
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(A1,HIGH);
  digitalWrite(7,HIGH);
  }
void Afive()
{
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(6,HIGH);

  }
void Asix(){
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(5,HIGH);
  }
void Aseven(){
  digitalWrite(2,HIGH);
  digitalWrite(A1,HIGH);
  digitalWrite(7,HIGH);
  }
void Aeight(){
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(A1,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(6,HIGH);
  }
void Anine(){
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(A1,HIGH);
  digitalWrite(7,HIGH);
  }
void Boff(){
  digitalWrite(2, LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(A1,LOW);
  digitalWrite(5,LOW);
  digitalWrite(7,LOW);
  digitalWrite(6,LOW);
  }
//this upper functions simply show the corresponding number in the seven segment display when they are called
void updateCommand(){
   if (receiver.decode(&results)){
    getValidchar remoteChar =readRemotechar();
    if(remoteChar.checkValidchar == true){
      tmpChar = remoteChar.validChar;
        Serial.print('[');
        Serial.print(tmpChar);
        Serial.println(']');
    if(tmpChar == '1') {
      Boff();
      Aone();
    }
    else if(tmpChar == '2'){
      Boff();
      Atwo(); 
    }
    else if(tmpChar == '3'){
      Boff();
      Athree();
    }
    else if(tmpChar == '4'){
      Boff();
      Afour();
    }
    else if(tmpChar == '5'){
      Boff();
      Afive();
    }
    else if(tmpChar == '6'){
      Boff();
      Asix();
    }
    else if(tmpChar == '7'){
      Boff();
      Aseven();
    }
    else if(tmpChar == '8'){
      Boff();
      Aeight();
    }
    else if(tmpChar == '9'){
      Boff();
      Anine();
    }
    else if(tmpChar == '0'){
      Boff();
      Anort();
    }
    else if(tmpChar == 'p'){
      Boff();
    }
    else if(tmpChar == '>'){
      Boff();
    }
    if ((msgBufferPointer == 0)&&(tmpChar == '<')){           // '<' 0x3C 
      msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++; 
    }
    else if (msgBufferPointer == 1){
      if  ((tmpChar == '@')||(tmpChar == '!')){     // '!'  (0x21 ==> Binary Format)or '@' (0x40 ==> String format)
        msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++;   
      }else{
        msgBufferPointer = 0;
      }
    }
    else if (msgBufferPointer == 2){
      if (tmpChar == ' '){     // ' ' (0x20 ==> Space)
        msgBuffer[msgBufferPointer] = tmpChar; msgBufferPointer++;      
      }else{
        msgBufferPointer = 0;
      }
    }
    else if (msgBufferPointer > MESSAGE_BUFFER_SIZE){ 
      msgBufferPointer = 0; 
      if ( tmpChar == '<') {
        msgBuffer[0] = tmpChar;    
        msgBufferPointer = 1;          
      }
    }
    else if (msgBufferPointer > 2) {
     if (( msgBuffer[1] == '@')&& (tmpChar == '>')) {
        //echoCommand();
        evaluateStringCommand();
        msgBufferPointer = 0;        
      }
     else if  (( msgBuffer[1] == '!')&& (msgBuffer[3] == msgBufferPointer )) {
        evaluateBinaryCommand();
        msgBufferPointer = 0;
      }
     else{
        msgBuffer[msgBufferPointer] = tmpChar;msgBufferPointer++;
        
      }
    }
        
    }
    receiver.resume();
  }
 
 }

void evaluateStringCommand(){
  if (msgBuffer[3] == MSG_GET_ECHO)   echoCommand();
  else if (msgBuffer[3] == MSG_GET_STATUS)  statusCommand();
  else if (msgBuffer[3] == MSG_SET_JOYINPUT) setJoyInput();
  else if (msgBuffer[3] == MSG_EVALUATE_BINARY) evaluateBinaryCommand();
  
  }
void evaluateBinaryCommand(){
  Serial.println("N/A-");
  echoCommand();
  }

void echoCommand(){
  Serial.print("[");
  for(int i = 0; i <= msgBufferPointer; i++){
    Serial.print(msgBuffer[i]);
  }
  Serial.print("]");
  }

void statusCommand(){
//  Serial.print("[");Serial.print(autoFlag); Serial.print("="); Serial.print(mSpeedR);Serial.print(" : "); Serial.print(mSpeedL); 
//  Serial.print("  +  ");Serial.print(odometerR); Serial.print(" : "); Serial.print(odometerL); Serial.print(" + "); 
//  Serial.print(lineR); Serial.print(" : "); Serial.print(lineL); Serial.print(" + ");  Serial.print(sonar); Serial.println("]"); 
  }

void setJoyInput(){
  }
