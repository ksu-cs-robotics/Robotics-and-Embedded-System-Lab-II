/**********************************************************************;
* Project           : RES_Class, Communication, IR_Remote_Key_Display, RES_Protocol_Design, 
*
* Program name      : RES_03_10_Clock_Serialout.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 02/23/2023
*
* Purpose           : how to creat a time format from the system time, millis()
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/
void setup(){
  Serial.begin(9600);
  Serial.println("System Time to clock format");
}

void loop(){
  unsigned long currentMillis = millis();
  unsigned long seconds = currentMillis / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  currentMillis %= 1000;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;


  Serial.print(hours);   Serial.print(":"); 
  Serial.print(minutes);   Serial.print(":");  
  Serial.print(seconds);   Serial.print(":");  
  Serial.println(currentMillis);
  delay(100);
}
