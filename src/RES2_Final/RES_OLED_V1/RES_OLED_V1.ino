#include <Wire.h>
#include <MPU6050_light.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include "Protocol.h"


SoftwareSerial mySerial (A2,A3);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MPU6050 mpu (Wire);


long cTime,  dTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(115200);

  Wire.begin();
  byte dStatus = display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  byte mStatus = mpu.begin();
  delay(100);  
  Serial.print(dStatus); Serial.print(" "); Serial.println(mStatus);
  delay(100);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");  


  displayTest();
  cTime = millis();
}



void loop() {
  // put your main code here, to run repeatedly:
  cTime = millis();
  checkSerialMessage(&Serial, msgBufferPointer_Serial0, &msgBufferPointer_Serial0);
//  checkSerialMessage(&Serial1, msgBufferPointer_Serial1, &msgBufferPointer_Serial1);  //only for Arduino Mega
  checkSerialMessage(&mySerial, msgBufferPointer_mySerial, &msgBufferPointer_mySerial);
    
  mpu.update();  // IMU update
  float angleZ = mpu.getAngleZ();
  if (cTime > dTime){
    dTime = cTime + 100;
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(angleZ);
    display.display(); 
    Serial.println(angleZ);
  }
}


void displayTest(){
  display.clearDisplay();
  display.display();
  delay(1000); // Pause for 2 seconds
  // Clear the buffer
  display.clearDisplay();
  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);
  delay(1000); // Pause for 2 seconds
  
  display.clearDisplay();
  display.setTextSize(1); // Draw 1X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("RES2 class"));
  display.display();      // Show initial text  
}
