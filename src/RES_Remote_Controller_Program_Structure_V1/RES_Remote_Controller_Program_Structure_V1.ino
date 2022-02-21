/**********************************************************************;
* Project           : RES_Class, Basic Function Check, Sensor, Motion,Comm 
*
* Program name      : RES_Basic_Function_Check.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 04/05/2020
*
* Purpose           : Inititalization of a Simple Rescue Robot 
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



#define PIN_IR_LED_LEFT_ANODE         6            // D6  - Anode   (+)   
#define PIN_IR_LED_LEFT_CATHODE       7            // D7  - Cathode (-)     
#define PIN_IR_LED_RIGHT_ANODE        8            // D8  - Anode   (+)
#define PIN_IR_LED_RIGHT_CATHODE      9            // D9  - Cathode (-)
#define PIN_JOY_X                    A2            
#define PIN_JOY_Y                    A1            
#define PIN_JOY_SW                    A0            

#define INTERVAL_JOYSTICK             30000     //0.03  
#define INTERVAL_LED_UPDATE            1000      //0.001
#define INTERVAL_MESSAGE_CHECK         1000       //0.001

#define RES_PROTOCOL_MAX_BUF_SIZE   64

char msgBuf[RES_PROTOCOL_MAX_BUF_SIZE];
int msgBufPnt = 0;

int joystick[3] = {0,0,0};    // joystick status
int joySWFlag = 0;            // push button flag
int ir[2] = {0,0};            // ir status 
unsigned long cTime = 0;     // current time of each iteration 
unsigned long jTime = 0;     // joystic test interval
unsigned long mTime = 0;     // message check interval
unsigned long lTime = 0;     // LED update interval



void setup() {
  // Serial Port Setup
  Serial.begin(115200);   Serial3.begin(115200);
  Serial.println("Remote Controller Initialization Started");

  pinMode(PIN_IR_LED_LEFT_ANODE, OUTPUT);  pinMode(PIN_IR_LED_LEFT_CATHODE, OUTPUT);  
  pinMode(PIN_IR_LED_RIGHT_ANODE, OUTPUT); pinMode(PIN_IR_LED_RIGHT_CATHODE, OUTPUT);
  digitalWrite(PIN_IR_LED_LEFT_ANODE, LOW);  digitalWrite(PIN_IR_LED_RIGHT_ANODE, LOW);  
  digitalWrite(PIN_IR_LED_LEFT_CATHODE, LOW);  digitalWrite(PIN_IR_LED_RIGHT_CATHODE, LOW);  
  pinMode(PIN_JOY_SW, INPUT);   digitalWrite(PIN_JOY_SW, HIGH);                  // or pinMode(PIN_JOY_SW, INPUT_PULLUP); 

}



void loop() {
  cTime = micros();     // get current time for this iteration
  if (cTime > jTime) {  // Joystick control time
    jTime = cTime + INTERVAL_JOYSTICK; 
    joystick[0] = map(analogRead(PIN_JOY_X), 0, 1023, -255, 255);
    joystick[1] = map(analogRead(PIN_JOY_Y), 0, 1023, -255, 255);
    int tmpJoySW = digitalRead(PIN_JOY_SW);

    /////////////////////////////////////////////////////////////////////////////
    //Using joySWFlag, you should make toggle button and update value on joystick[2] 
    //////////////////////////////////////////////////////////////////////////
        
    Serial3.print("Joy "); Serial3.print(joystick[0]); Serial3.print(" "); Serial3.print(joystick[1]); Serial3.print(" "); Serial3.println(joystick[2]);     
  }

  if (cTime > lTime){
    lTime = cTime+ INTERVAL_LED_UPDATE;
    digitalWrite(PIN_IR_LED_LEFT_ANODE, ir[0]);
    digitalWrite(PIN_IR_LED_RIGHT_ANODE, ir[1]);
  }
  if (cTime > mTime){
    checkSerialMessage();
  }
}


void debugPrint(){
  Serial.println("----"); 
}

void checkSerialMessage(){
  while (Serial.available() > 0){
    char a = Serial.read();
    Serial3.print(a);
  }

  while (Serial3.available() > 0){
    char tmpChar = Serial3.read();
    Serial.print(tmpChar );                   // message forward to USB for drawing data
    if (msgBufPnt >= RES_PROTOCOL_MAX_BUF_SIZE){
      Serial.println("Message Overflow");
      if ((tmpChar != '\n') || (tmpChar != '\r')){
        msgBuf[0] = tmpChar;
        msgBufPnt = 1;
      }
    }else{      
      if ((tmpChar == '\n') || (tmpChar == '\r')){
        msgBuf[msgBufPnt] = '\0';
        checkFrame();
        msgBufPnt = 0;
      }else{
        msgBuf[msgBufPnt] = tmpChar;
        msgBufPnt++;                    
      }
    }
  }
}

bool checkFrame(){
 char *p = msgBuf;
 String str;
 int cnt =0;
// while ((str = strtok_r(p, ";", &p)) != NULL) // delimiter is the semicolon
 str = strtok_r(p, " ", &p);                    // delimiter is the space
 if (str == "ir"){
    while ((str = strtok_r(p, " ", &p)) != NULL){
      if (cnt == 0){          //left Line value
        ir[0] = str.toInt();
      }else if (cnt == 1){    //right Line value
        ir[1] = str.toInt();
      }
      cnt++;
    }
 }
   
}
