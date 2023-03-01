#include "Digit.h"
#include "Display.h"

const byte digit_pins[4] = {2, 3, 4, 5};  // From com1 - com4
const byte segment_pins[7]= {6,7,8,9,10,11,12};  // From a - g (abc...g)
const byte dp_pin = 13; //3;

unsigned long rHours = 0;
unsigned long rMinutes = 0;
unsigned long timeAdjust = 0;
unsigned long currentMillis = 0;
unsigned long seconds = 0;
unsigned long minutes = 0;
unsigned long hours = 0;
unsigned long days = 0;
unsigned long milliSeconds = 1000;
  
char buf[5]={'1','2', '.','8', '5'};
int count = 0;
Display d(digit_pins, segment_pins, dp_pin);

void setup() {
  Serial.begin(9600);
  Serial.println("Display time on 4-Digit Display");
  //e.g. clock set at 10:20
  rHours = 10;
  rMinutes = 20;  
  timeAdjust = (rMinutes * 60 * 1000 )+ (rHours * 60 * 60 * 1000);  

  for (int i = 0; i <= A5; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  currentMillis = millis() + timeAdjust;
//  seconds
//  d.print("2340"); // Must be looped!
//  char *intStr = itoa(count++,buf,10);
//  d.print(intStr);
//  buf[0] =  minutes % 10;
//  buf[1] =  minutes - (10*buf[0]);
//  buf[3] =  seconds % 10;
//  buf[4] =  seconds - (10*buf[3]);
  String aa = String(minutes);
  String bb = String(".");
  String cc = String(seconds);
  snprintf(buf, sizeof(buf), "%s%s", aa,cc);
  
  d.print(aa); // Must be looped!
  Serial.println(buf);
  delay(10);
  updateTime();
  //delayMicroseconds(10);
}

char* intToStr(int data) {
    std::string strData = std::to_string(data);

    char* temp = new char[strData.length() + 1];
    strcpy(temp, strData.c_str());

   return temp;
}


void updateTime(){ 
  seconds = currentMillis / 1000;
  minutes = seconds / 60;
  hours = minutes / 60;
  days = hours / 24;
  milliSeconds = currentMillis % 1000;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
}  
