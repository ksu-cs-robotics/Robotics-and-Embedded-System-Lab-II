#include "Arduino.h"
#include "Action.h"
#include "Sensing.h"
#include "Protocol.h"
#include "Localization.h"
#include "SDR_SUGV_04.h"


void updateMotor(){
  if( speedR > 0){
    digitalWrite(MOTOR_RIGHT_A, LOW); digitalWrite(MOTOR_RIGHT_B, HIGH); analogWrite(MOTOR_RIGHT_ENABLE, speedR); 
  }else if ( speedR < 0){
    digitalWrite(MOTOR_RIGHT_A, HIGH); digitalWrite(MOTOR_RIGHT_B, LOW); analogWrite(MOTOR_RIGHT_ENABLE, abs(speedR)); 
  }else {
    digitalWrite(MOTOR_RIGHT_A, LOW); digitalWrite(MOTOR_RIGHT_B, LOW); analogWrite(MOTOR_RIGHT_ENABLE, 0);    
  }
  if( speedL > 0){
    digitalWrite(MOTOR_LEFT_A, HIGH); digitalWrite(MOTOR_LEFT_B, LOW); analogWrite(MOTOR_LEFT_ENABLE, speedL);     
  }else if ( speedL < 0){
    digitalWrite(MOTOR_LEFT_A, LOW); digitalWrite(MOTOR_LEFT_B, HIGH ); analogWrite(MOTOR_LEFT_ENABLE, abs(speedL)); 
  }else {
    digitalWrite(MOTOR_LEFT_A, LOW); digitalWrite(MOTOR_LEFT_B, LOW); analogWrite(MOTOR_LEFT_ENABLE, 0);         
  }
}


