#include "Arduino.h"
#include "Protocol_IR_Display.h"
#include <IRremote.h>

int a = 2;  //For displaying segment "a"
int b = 3;  //For displaying segment "b"
int c = 4;  //For displaying segment "c"
int d = 5;  //For displaying segment "d"
int e = 6;  //For displaying segment "e"
int f = 8;  //For displaying segment "f"//the following function is used to display a number when the receiveChar is equal to that number 
int g = 9;  //For displaying segment "g"
//this following function initialize the pin of seven segment Display in Output mode

void init_pin_mode(){
  pinMode(a, OUTPUT);  //A
  pinMode(b, OUTPUT);  //B
  pinMode(c, OUTPUT);  //C
  pinMode(d, OUTPUT);  //D
  pinMode(e, OUTPUT);  //E
  pinMode(f, OUTPUT);  //F
  pinMode(g, OUTPUT);  //G
}
//this function will turn off the seven segment display
void turnOff()
{
  digitalWrite(a,LOW);
  digitalWrite(b,LOW);
  digitalWrite(c,LOW);
  digitalWrite(d,LOW);
  digitalWrite(e,LOW);
  digitalWrite(f,LOW);
  digitalWrite(g,LOW);
}
//the following function is used to display a digit in the seven segment display when the receiveChar is equal to that digit 
void displayDigit(char receiveChar){
 //Conditions for displaying segment a
 if(receiveChar!='1' && receiveChar != '4')
  digitalWrite(a,HIGH);
 
 //Conditions for displaying segment b
 if(receiveChar != '5' && receiveChar != '6')
  digitalWrite(b,HIGH);
 
 //Conditions for displaying segment c
 if(receiveChar != '2')
  digitalWrite(c,HIGH);
 
 //Conditions for displaying segment d
 if(receiveChar != '1' && receiveChar !='4' && receiveChar !='7')
  digitalWrite(d,HIGH);
 
 //Conditions for displaying segment e 
 if(receiveChar == '2' || receiveChar =='6' || receiveChar == '8' || receiveChar=='0')
  digitalWrite(e,HIGH);
 
 //Conditions for displaying segment f
 if(receiveChar != '1' && receiveChar !='2' && receiveChar!='3' && receiveChar !='7')
  digitalWrite(f,HIGH);
 if (receiveChar!='0' && receiveChar!='1' && receiveChar!='7')
  digitalWrite(g,HIGH);
}
