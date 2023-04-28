#include <Wire.h>
#include <MPU6050_light.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include "Protocol.h"


SoftwareSerial mySerial (A2,A3);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MPU6050 mpu (Wire);


long cTime,  dTime , mTime= 0;

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
  
  pinMode(PIN_MOTOR_PWM_RIGHT,OUTPUT);pinMode(PIN_MOTOR_IN0_RIGHT,OUTPUT);pinMode(PIN_MOTOR_IN1_RIGHT,OUTPUT);
  pinMode(PIN_MOTOR_PWM_LEFT,OUTPUT);pinMode(PIN_MOTOR_IN0_LEFT,OUTPUT);pinMode(PIN_MOTOR_IN1_LEFT,OUTPUT);
  
  // Wheel Encoder Setting
  pinMode(PIN_WHEEL_ENCODER_CLK_LEFT, INPUT); pinMode(PIN_WHEEL_ENCODER_DT_LEFT, INPUT);
  pinMode(PIN_WHEEL_ENCODER_CLK_RIGHT, INPUT); pinMode(PIN_WHEEL_ENCODER_DT_RIGHT, INPUT);
 
  
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
  updateMotorSpeed();
}


void updateMotorSpeed(){
  if ( cTime > mTime) {
    mTime = millis() +  100;  

    if (cmd_vel[0] > 0){ //linear 
       motor_speed[LEFT] = cmd_vel[0] - cmd_vel[1];
       motor_speed[RIGHT] = cmd_vel[0]+ cmd_vel[1];               
    }else{
       motor_speed[LEFT] = cmd_vel[0] + cmd_vel[1];
       motor_speed[RIGHT] = cmd_vel[0]- cmd_vel[1];                     
    }

    if (motor_speed[RIGHT] > 255) { 
      motor_speed[RIGHT] = 255;
    }else if (motor_speed[RIGHT] < -255) {
      motor_speed[RIGHT] = -255;
    }
    if (motor_speed[LEFT] > 255) { 
      motor_speed[LEFT] = 255;
    }else if (motor_speed[LEFT] < -255) {
      motor_speed[LEFT] = -255;
    }
//    analogWrite( PIN_MOTOR_PWM_RIGHT, abs( motor_speed[RIGHT]));
//    analogWrite( PIN_MOTOR_PWM_LEFT, abs(motor_speed[LEFT]));
    analogWrite( PIN_MOTOR_PWM_RIGHT, 50);
    analogWrite( PIN_MOTOR_PWM_LEFT, 50);
    
    if (motor_speed[RIGHT] > 0){  
      digitalWrite( PIN_MOTOR_IN0_RIGHT, HIGH); digitalWrite( PIN_MOTOR_IN1_RIGHT, LOW);
    }else{
      digitalWrite( PIN_MOTOR_IN0_RIGHT, LOW); digitalWrite( PIN_MOTOR_IN1_RIGHT, HIGH);
    }
    if (motor_speed[LEFT] > 0 ){
      digitalWrite( PIN_MOTOR_IN0_LEFT, HIGH); digitalWrite( PIN_MOTOR_IN1_LEFT, LOW);      
    }else{
      digitalWrite( PIN_MOTOR_IN0_LEFT, LOW); digitalWrite( PIN_MOTOR_IN1_LEFT, HIGH);    
    }
    Serial.print(cmd_vel[0]);
    Serial.print(" : ");
    Serial.print(cmd_vel[1]);
    Serial.print(" = ");
    Serial.print(motor_speed[0]);
    Serial.print(" : ");
    Serial.println(motor_speed[1]);

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
