#include <SoftwareSerial.h>

SoftwareSerial mySerial(A2,A3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(57600);
  Serial.println("Serial Nano");
  mySerial.println("mySerial Nano ");
  delay(1000);
}

long cTime, rTime = 0;
long cnt=0;
void loop() {
  cTime = millis();
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0 ){
    char tmp = Serial.read();
    mySerial.print("{"); mySerial.print(tmp); mySerial.print("}"); 
//    Serial.print("[");Serial.print(tmp);Serial.print("]");
  }
  if (mySerial.available() > 0 ){
    char tmp = mySerial.read();
//    Serial.write(tmp);
    Serial.print("[");Serial.print(tmp);Serial.print("]");
  }
  if (cTime > rTime){
    rTime = cTime+1;
    cnt++;
//    Serial.println(cnt);
//    mySerial.println(cnt);
//    mySerial.print("<@,");
//    mySerial.print(cnt);
//    mySerial.print(",100,100");
//    mySerial.println(">");
  }
}
