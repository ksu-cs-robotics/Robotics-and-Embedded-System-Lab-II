/**********************************************************************;
* Project           : RES_Class, DC Motor Control, H-Bridge Motor Driver 
*
* Program name      : RES_Rotary_Encoder_Basic.ino
*
* Author            : Jong-Hoon Kim
*
* Date created      : 03/10/2020
*
* Purpose           : H-Bridge Motor Driver 
*
* Revision History  :
*
* Date        Author      Ref    Revision (Date in MMDDYYYY format) 
* MMDDYYYY    name      1  v-xx   revision note. 
*
*********************************************************************/
/*********************************************************************
*   Instructional Note:  
*           Arduino Leonardo has 4 interrupt pins ( 0, 1, 2, 3  13 )
*           Arduino Mega has 6 interrupt pins ( 2, 3, 18, 19, 20, 21)
*   reference website ( https://www.pjrc.com/teensy/td_libs_Encoder.html )                    
***********************************************************************/

#define PIN_WHEEL_ENCODER_CLK_LEFT    2
#define PIN_WHEEL_ENCODER_DT_LEFT    4
#define PIN_WHEEL_ENCODER_SW_LEFT    5
#define PIN_MOTOR_PWM_LEFT    8
#define PIN_MOTOR_IN0_LEFT    9
#define PIN_MOTOR_IN1_LEFT    10

void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);
  Serial.println("Motor Control Started");
  pinMode(PIN_WHEEL_ENCODER_CLK_LEFT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_WHEEL_ENCODER_CLK_LEFT), updateEncoder, CHANGE);  
}


int encoderPin_X  = 0;
int encoderPin_Y  = 0;
long counter  =0;

int targetPosition = 0;

void loop() {
  if (Serial.available() > 0){
    char tmpChar = Serial.read();
    if (tmpChar == 'a') targetPosition = 200;
    else if (tmpChar == 's') targetPosition = 0;
    else if (tmpChar == 'd') targetPosition = -200;
    else  {
      Serial.print(counter);
      Serial.print("  = ");
      Serial.println(targetPosition);
      Serial3.print(counter);
      Serial3.print("  = ");
      Serial3.println(targetPosition);
    }
  }
   if (Serial3.available() > 0){
    char tmpChar = Serial3.read();
    if (tmpChar == 'a') targetPosition = 200;
    else if (tmpChar == 's') targetPosition = 0;
    else if (tmpChar == 'd') targetPosition = -200;
    else  {
      Serial3.print(counter);
      Serial3.print("  = ");
      Serial3.println(targetPosition);
      Serial.print(counter);
      Serial.print("  = ");
      Serial.println(targetPosition);
     }
  }
  if (targetPosition < counter){
    analogWrite(PIN_MOTOR_PWM_LEFT, 255);   digitalWrite( PIN_MOTOR_IN0_LEFT, HIGH);    digitalWrite( PIN_MOTOR_IN1_LEFT , LOW);
    
  }else if (targetPosition > counter){
    analogWrite(PIN_MOTOR_PWM_LEFT, 255);   digitalWrite( PIN_MOTOR_IN0_LEFT , LOW);    digitalWrite( PIN_MOTOR_IN1_LEFT , HIGH);
    
  }else{
    analogWrite(PIN_MOTOR_PWM_LEFT, 0);   digitalWrite( PIN_MOTOR_IN0_LEFT, LOW);    digitalWrite( PIN_MOTOR_IN1_LEFT , LOW);
  }
}



void updateEncoder(){
  encoderPin_X = digitalRead(PIN_WHEEL_ENCODER_CLK_LEFT);
  encoderPin_Y = digitalRead(PIN_WHEEL_ENCODER_DT_LEFT);

  if (encoderPin_X == 0){   
    if (encoderPin_Y == 0){
      counter++;
    }else{
      counter--;
    }
  }else{
    if (encoderPin_Y == 0){
      counter--;
    }else{
      counter++;
    }    
  }
}
