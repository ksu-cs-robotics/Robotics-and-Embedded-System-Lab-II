/**********************************************************************;
* Project           : RES_Class, Basic Function Check, Sensor, Motion,Comm 
*
* Program name      : RES_02_Controller_Board_01.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 04/19/2022
*
* Purpose           : Controling a Robot with simple SLAM
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/
/*********************************************************************
*   Instructional Note:  
*           Arduino Leonardo has 4 interrupt pins ( 0, 1, 2, 3  13 )
*           Arduino Mega has 6 interrupt pins ( 2, 3, 18, 19, 20, 21)
*                      
***********************************************************************/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ATR_Protocol.h"
#include "Config.h"



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


enum JoyImput {E_JOY_X, E_JOY_Y, E_SW1, E_SW2, E_SW3, E_SW4, E_SW5, E_SW6, E_SW7};
int joystick[9] = {0,0,0,0,0,0,0,0,0};    // joystick status {x,y,sw1,sw2,sw3, up,down,left,right}
int ir[2] = {0,0};            // ir status 
unsigned long cTime = 0;     // current time of each iteration 
unsigned long jTime = 0;     // joystic test interval
unsigned long mTime = 0;     // message check interval
unsigned long lTime = 0;     // OLED update interval



char msgBuffer[MESSAGE_BUFFER_SIZE]; 
char robotBuffer[MESSAGE_BUFFER_SIZE]; 
int msgBufferPointer = 0;
int robotBufferPointer = 0;


void printOLED(String argStr);

void setup() {
  // Serial Port Setup
//  Serial.begin(115200);   Serial3.begin(115200);
  Serial.begin(9600);   Serial1.begin(9600);
  Serial.println("Remote Controller Initialization Started");
  pinMode(PIN_SW_1, INPUT_PULLUP);pinMode(PIN_SW_2, INPUT_PULLUP); 
  pinMode(PIN_SW_4, INPUT_PULLUP); pinMode(PIN_SW_5, INPUT_PULLUP);
  pinMode(PIN_SW_6, INPUT_PULLUP); pinMode(PIN_SW_7, INPUT_PULLUP);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  printOLED("Program started", 1, 0, 0);
}



void loop() {
  cTime = micros();     // get current time for this iteration
  if (cTime > jTime) {  // Joystick control time
    jTime = cTime + INTERVAL_JOYSTICK; 
    checkJoystick();
  }

  if (cTime > mTime){
    mTime = cTime + INTERVAL_MESSAGE_CHECK;
    recieved_msg();
    recieved_robot();
  }

  if (cTime > lTime){
    lTime = cTime+ INTERVAL_OLED_UPDATE;
    checkOLED();
  }
  
}

void checkOLED(){
    String data_out= String(joystick[2])+"  "+String(joystick[3])+"  "+String(joystick[4])+"  "+String(joystick[5])+"  "+String(joystick[6])+"  "+String(joystick[7])+"  "+String(joystick[8])+"  "+String(joystick[0])+"  "+String(joystick[1]);                 
    printOLED(data_out);    
//    Serial.println("Update OLED ");

}

void checkJoystick(){
    joystick[E_JOY_X] = map(analogRead(PIN_JOY_X), 0, 1023, -255, 255);
    joystick[E_JOY_Y] = map(analogRead(PIN_JOY_Y), 0, 1023, -255, 255);
    joystick[E_SW1] = digitalRead(PIN_SW_1); 
    joystick[E_SW2] = digitalRead(PIN_SW_2); 
    joystick[E_SW4] = digitalRead(PIN_SW_4);
    joystick[E_SW5] = digitalRead(PIN_SW_5);
    joystick[E_SW6] = digitalRead(PIN_SW_6);
    joystick[E_SW7] = digitalRead(PIN_SW_7);
    send_py(joystick, 9, CMD_MSG);   
}


void recieved_robot(){
  if (Serial1.available() > 0){
    char tmpChar = Serial1.read();
    if (robotBufferPointer == 0){
       if(tmpChar == '<'){  // first preamble
          robotBuffer[robotBufferPointer] = tmpChar; 
          robotBufferPointer++;
       }
    } else if (robotBufferPointer == 1){
      if (tmpChar == '@'){
        robotBuffer[robotBufferPointer] = tmpChar; // second preamble 
        robotBufferPointer++;
      }else if (tmpChar == '<'){ 
        robotBuffer[0] = tmpChar;  //restart
      }else{
        robotBufferPointer = 0; //error
      }
    } else if (robotBufferPointer == 2){
      if (tmpChar == ' '){    // thrid preamble
        robotBuffer[robotBufferPointer] = tmpChar; // second preamble 
        robotBufferPointer++;
      }else if (tmpChar == '<'){ 
        robotBuffer[0] = tmpChar;  //restart
        robotBufferPointer = 1;  //restart
      }else{
        robotBufferPointer = 0; //error
      }   
    } else {
      if ((robotBufferPointer > 5) && (tmpChar == '>')){
        robotBuffer[robotBufferPointer] = tmpChar;
        robotBufferPointer = 0;
//        evaluateCommand();
        evaluateCommand(robotBuffer);
        for (int i = 0; i < MESSAGE_BUFFER_SIZE; i++) robotBuffer[robotBufferPointer] = 0x00;
      } else{
        if (robotBufferPointer > MESSAGE_BUFFER_SIZE){
          robotBufferPointer = 0;
          for (int i = 0; i < MESSAGE_BUFFER_SIZE; i++) robotBuffer[robotBufferPointer] = 0x00;
        }else{
          robotBuffer[robotBufferPointer] = tmpChar;
          robotBufferPointer++;
        }
      }
    }
  }
}


void recieved_msg(){
  if (Serial.available() > 0){
    char tmpChar = Serial.read();
    if (msgBufferPointer == 0){
       if(tmpChar == '<'){  // first preamble
          msgBuffer[msgBufferPointer] = tmpChar; 
          msgBufferPointer++;
       }
    } else if (msgBufferPointer == 1){
      if (tmpChar == '@'){
        msgBuffer[msgBufferPointer] = tmpChar; // second preamble 
        msgBufferPointer++;
      }else if (tmpChar == '<'){ 
        msgBuffer[0] = tmpChar;  //restart
      }else{
        msgBufferPointer = 0; //error
      }
    } else if (msgBufferPointer == 2){
      if (tmpChar == ' '){    // thrid preamble
        msgBuffer[msgBufferPointer] = tmpChar; // second preamble 
        msgBufferPointer++;
      }else if (tmpChar == '<'){ 
        msgBuffer[0] = tmpChar;  //restart
        msgBufferPointer = 1;  //restart
      }else{
        msgBufferPointer = 0; //error
      }   
    } else {
      if ((msgBufferPointer > 5) && (tmpChar == '>')){
        msgBuffer[msgBufferPointer] = tmpChar;
        msgBufferPointer = 0;
//        evaluateCommand();
        evaluateCommand(msgBuffer);
        for (int i = 0; i < MESSAGE_BUFFER_SIZE; i++) msgBuffer[msgBufferPointer] = 0x00;
      } else{
        if (msgBufferPointer > MESSAGE_BUFFER_SIZE){
          msgBufferPointer = 0;
          for (int i = 0; i < MESSAGE_BUFFER_SIZE; i++) msgBuffer[msgBufferPointer] = 0x00;
        }else{
          msgBuffer[msgBufferPointer] = tmpChar;
          msgBufferPointer++;
        }
      }
    }
  }
}


void send_py(String data[], int size, String type){
  String data_out;
  data_out = "<! " + type + " " + String(size) + " ";
  for(int i = 0; i < size; i++){
    data_out += data[i];
    data_out += " ";
  }
  data_out += "#>";

//  Serial.print(data_out);
//  Serial.print("\n");
  Serial1.print(data_out);
  Serial1.print("\n");
}

void send_py(int data[], int size, String type){
  String data_out;
  data_out = "<! " + type + " " + String(size) + " ";
  for(int i = 0; i < size; i++){
    data_out += String(data[i]);
    data_out += " ";
  }
  data_out += "#>";

//  Serial.print(data_out);
//  Serial.print("\n");
  Serial1.print(data_out);
  Serial1.print("\n");  
}




void printOLED(String argStr){
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(argStr);
  display.display();
}
void printOLED(String argStr, int argTextSize, int argX, int argY){
  display.clearDisplay();
  display.setTextSize(argTextSize);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(argX, argY);     // Start at top-left corner
  display.println(argStr);
  display.display();
}
