
/**********************************************************************;
* Project           : RES_Class, String Tokenizer for Protocal Design 
*
* Program name      : RES_String_Tokenizer.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 04/05/2020
*
* Purpose           : Protocal Design : Parsing string message*                    
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/
/*********************************************************************
*   Instructional Note:  
*       [1] Frame
*           There any serveral options for making a message frame 
*              (1) Serial.readStringUntil('\n\r') [Unexpected delay may occur]
*                   -- example "Joy 10 12 1\n\r"
*              (2) Ues exteral buffer and finding a frame end symbol (you can choose a symbol such as '\n','\r', etc.)
*                   -- example "Joy 10 12 1\n\r"
*              (3) [check RES_Serial_Message_Protocol] Ues exteral buffer, and finding series of frame start symbols and then payload size (you can choose a word such as "<$@")
*                   -- example message ( Joy command = 0x01, first value = 10, second value = 12, third vaule = 1
*                   -----------"<$@"+0x04+0x01+0x00+0x0A+0x00+0x0C+0x00+0x01
*                   ---------- 3byts(start word)+1byte(payload)+1byte(command type)+2bytes(first value)+2bytes(second value)+2bytes(third value)   
*                   
*                   
*       [2] Interpret frame 
*           (1)Uing stringtokener for split words in the frame  
*           (2)check the splitted word and convert it to value if needed
***********************************************************************/

#define ATR_PROTOCOL_MAX_BUF_SIZE   64

char msgBuf[ATR_PROTOCOL_MAX_BUF_SIZE];
int msgBufPnt = 0;

int joystick[3] = {0,0,0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Started...");
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly: 
  updateSerialMessage();

}

void updateSerialMessage(){
//  digitalWrite(LED_BUILTIN, HIGH);
//  if (Serial.available()){
//    Serial.print("[");
//    String msg = Serial.readStringUntil('\n\r');  // "Serial.readStringUntil('\n\r')" possiblly make a short delay if message didn't have '\r'"
//    Serial.print(msg);
//    Serial.println("]");    
//  }
//  digitalWrite(LED_BUILTIN, LOW);


  while (Serial.available() > 0){
    char tmpChar = Serial.read();
    if (msgBufPnt >= ATR_PROTOCOL_MAX_BUF_SIZE){
      Serial.println("Message Overflow");
      if ((tmpChar != '\n') || (tmpChar != '\r')){
        msgBuf[0] = tmpChar;
        msgBufPnt = 1;
      }
    }else{      
      if ((tmpChar == '\n') || (tmpChar == '\r')){
        msgBuf[msgBufPnt] = '\0';
        checkMessage();
        msgBufPnt = 0;
      }else{
        msgBuf[msgBufPnt] = tmpChar;
        msgBufPnt++;                    
      }
    }
  }
}

bool checkMessage(){
//  Serial.println(msgBuf);
 char *p = msgBuf;
 String str;
 int cnt =0;
// while ((str = strtok_r(p, ";", &p)) != NULL) // delimiter is the semicolon
 str = strtok_r(p, ";", &p);
 Serial.println(str);
 if (str == "Joystick"){
    Serial.println("Joy ");
    while ((str = strtok_r(p, ";", &p)) != NULL){
      if (cnt == 0){          //joy x value
        joystick[0] = str.toInt();
      }else if (cnt == 1){    //joy y value
        joystick[1] = str.toInt();
      }else if (cnt == 2){    //joy sw value
        joystick[2] = str.toInt();
      }
      cnt++;
    }
    Serial.print(joystick[0]); Serial.print(" ");
    Serial.print(joystick[1]); Serial.print(" ");
    Serial.println(joystick[2]);
 }else if (str == "Command"){

 }else if (str == "Message"){
    while ((str = strtok_r(p, ";", &p)) != NULL){
      Serial.print("[");      
      Serial.print(str);      
      Serial.println("]");      
    }

 }
   
}
