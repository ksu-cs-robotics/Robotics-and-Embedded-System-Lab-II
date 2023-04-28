#include <SoftwareSerial.h>
#include "Protocol.h"


SoftwareSerial mySerial (A2,A3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
}



void loop() {
  // put your main code here, to run repeatedly:

  checkSerialMessage(&Serial, msgBufferPointer_Serial0, &msgBufferPointer_Serial0);
  checkSerialMessage(&Serial1, msgBufferPointer_Serial1, &msgBufferPointer_Serial1);
  checkSerialMessage(&mySerial, msgBufferPointer_mySerial, &msgBufferPointer_mySerial);


}
