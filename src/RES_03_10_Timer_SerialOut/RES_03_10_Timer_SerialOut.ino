/**********************************************************************;
* Project           : RES_Class, Communication, IR_Remote_Key_Display, RES_Protocol_Design, 
*
* Program name      : RES_03_10_Timer_SerialOut.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 02/23/2023
*
* Purpose           : how to use a 4-digit timer and understanding non-blocking code
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/
#include "Segment.h"
#include "Digit.h"
#include "Display.h"

const byte digit_pins[4] = {2, 3, 4, 5};  // From com1 - com4
const byte segment_pins[7]= {6,7,8,9,10,11,12};  // From a - g (abc...g)
const byte dp_pin = 13; //3;
Display d(digit_pins, segment_pins, dp_pin);

unsigned long currentMillis = 0;
unsigned long nextMillis = 0;
unsigned long minutes = 0;
unsigned long seconds = 0;
unsigned long fraction = 0;
void setup(){
  Serial.begin(9600);
  Serial.println("4-digit Timer");  
  for (int i = 0; i <= A5; i++) {
    pinMode(i, OUTPUT);
  }  
}
String msg = "0";
String strFraction = "0";
String strMinutes = "0";
String strSeconds = "0";

void loop(){ 
  currentMillis = millis();
  if (currentMillis > nextMillis){
    fraction = currentMillis / 10;
    seconds = currentMillis / 1000;
    minutes = seconds / 60;

//    minutes %=60;
//    if (minutes < 10) msg ="0";
//    msg = msg + String (minutes);
//    seconds %= 60; 
//    if (minutes < 10) msg = msg+ "0";
//    msg = msg + String(minutes);

    fraction = fraction&10;
    if ( fraction < 10) strFraction = "0"+String (fraction); else strFraction = String (fraction) ;    
    minutes = minutes % 60;
    if (minutes < 10 ) strMinutes = "0"+String (minutes);  else strMinutes = String (minutes) ;
    seconds = seconds % 60;
    if (seconds < 10) strSeconds = "0"+String (seconds); else strSeconds = String (seconds) ;
    msg = strSeconds+"."+strFraction;
//    msg = strMinutes+"."+strSeconds;
    Serial.println(msg);   
    nextMillis = currentMillis + 10;
  }
  
  d.print(msg);
//  d.print("61.11");
}
