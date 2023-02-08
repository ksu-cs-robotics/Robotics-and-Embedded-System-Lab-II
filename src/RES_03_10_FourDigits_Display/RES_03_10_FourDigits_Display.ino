#include "Digit.h"
#include "Display.h"

const byte digit_pins[4] = {2, 3, 4, 5};  // From com1 - com4
const byte segment_pins[7]= {6,7,8,9,10,11,12};  // From a - g (abc...g)
const byte dp_pin = 13; //3;

char buf[5]={'1','2', '.','8', '5'};
int count = 0;
Display d(digit_pins, segment_pins, dp_pin);

void setup() {
  for (int i = 0; i <= A5; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
//  d.print("2340"); // Must be looped!
  char *intStr = itoa(count++,buf,10);
  d.print(intStr);
//  d.print(buf); // Must be looped!
//  for(long i = 0; i < 1000000; i++) {}
  //delay(1);
  //delayMicroseconds(10);
}
