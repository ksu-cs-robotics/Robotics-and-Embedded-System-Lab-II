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
#define PIN_JOY_X                    A0            
#define PIN_JOY_Y                    A1            
#define PIN_JOY_SW                    A2            

#define INTERVAL_JOYSTICK_TEST        30
#define INTERVAL_LED_TEST             1000
#define INTERVAL_MESSAGE              100

long cTime = 0;     // current time of each iteration 
long jTime = 0;     // joystic test interval
long lTime = 0;     // led test interval
long mTime = 0;     // message print interval

int joyX, joyY, joySW = 0;

void setup() {
  // Serial Port Setup
  Serial.begin(115200);   Serial3.begin(115200);
  Serial.println("Remote Controller Initialization Started");

  pinMode(PIN_IR_LED_LEFT_ANODE, OUTPUT);  pinMode(PIN_IR_LED_LEFT_CATHODE, OUTPUT);  
  pinMode(PIN_IR_LED_RIGHT_ANODE, OUTPUT); pinMode(PIN_IR_LED_RIGHT_CATHODE, OUTPUT);
  digitalWrite(PIN_IR_LED_LEFT_ANODE, LOW);  digitalWrite(PIN_IR_LED_RIGHT_ANODE, HIGH);  
  digitalWrite(PIN_IR_LED_LEFT_CATHODE, LOW);  digitalWrite(PIN_IR_LED_RIGHT_CATHODE, LOW);  
  pinMode(PIN_JOY_SW, INPUT);   digitalWrite(PIN_JOY_SW, HIGH);                  // or pinMode(PIN_JOY_SW, INPUT_PULLUP); 

}



void loop() {
  cTime = millis();     // get current time for this iteration
  if (cTime > jTime) {  // motor testing time
    jTime = millis() + INTERVAL_JOYSTICK_TEST; 
    joyX = analogRead(PIN_JOY_X);
    joyY = analogRead(PIN_JOY_Y);
    joySW = digitalRead(PIN_JOY_SW);
    
  }
  if (cTime > lTime) {  // led testing time
    lTime = millis() + INTERVAL_LED_TEST; 
    digitalWrite(PIN_IR_LED_LEFT_ANODE, !digitalRead(PIN_IR_LED_LEFT_ANODE));  
    digitalWrite(PIN_IR_LED_RIGHT_ANODE, !digitalRead(PIN_IR_LED_RIGHT_ANODE));
  }
  if (cTime > mTime) {  // message time
    mTime = millis() + INTERVAL_MESSAGE; 
    Serial3.print("J "); Serial3.print(joyX); Serial3.print(" "); Serial3.print(joyY); Serial3.print(" "); Serial3.println(joySW);
  }
  checkSerialMessage();
}


void debugPrint(){
  Serial.println("----"); 
}

void checkSerialMessage(){
  while (Serial.available() > 0){
    Serial3.write( Serial.read());
  }
  while (Serial3.available() > 0){
    Serial.write( Serial3.read());
  }
}
