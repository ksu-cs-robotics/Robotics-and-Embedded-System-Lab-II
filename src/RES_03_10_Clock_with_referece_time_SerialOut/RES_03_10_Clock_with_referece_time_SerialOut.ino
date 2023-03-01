/**********************************************************************;
* Project           : RES_Class, Communication, IR_Remote_Key_Display, RES_Protocol_Design, 
*
* Program name      : RES_03_10_Clock_Serialout.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 02/23/2023
*
* Purpose           : how to creat a time format from millis()
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/
unsigned long rDays = 0;
unsigned long rHours = 0;
unsigned long rMinutes = 0;
unsigned long timeAdjust = 0;
void setup(){
  Serial.begin(9600);
  Serial.println("System Time to clock format");
  //e.g. clock set at 10:20
  rDays = 23;
  rHours = 10;
  rMinutes = 20;  
  timeAdjust = (rMinutes * 60 * 1000 )+ (rHours * 60 * 60 * 1000)+ (rDays * 24 * 60 * 60 * 1000)
}

void loop(){ 
  unsigned long currentMillis = millis() + timeAdjust;
  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  unsigned long milliSeconds = currentMillis % 1000;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;

  Serial.print(currentMillis); Serial.print(",");  
  Serial.print(days);   Serial.print(","); 
  Serial.print(hours);   Serial.print(":"); 
  Serial.print(minutes);   Serial.print(":");  
  Serial.print(seconds);   Serial.print(":");  
  Serial.println(milliSeconds); 
  
  delay(100);
}
