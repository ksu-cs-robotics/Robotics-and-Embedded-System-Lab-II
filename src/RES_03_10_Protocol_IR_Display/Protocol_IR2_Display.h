#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/************************************** ATR Lab Simple UAV Protocol *******************************************************/
#define UAV_VERSION              1

#define MSG_GET_IDENT                0x41   // 'A' out message         ATR protocol version
#define MSG_GET_STATUS               0x42   // 'B' out message         status data 
#define MSG_GET_RAW_IMU              0x43   // 'C' out message         9 DOF
#define MSG_GET_ECHO                 0x31   // '1' in message       message echo         
#define MSG_EVALUATE_BINARY          0x32   //'2' in message
#define MSG_SET_AUTO             0x61   // 'a' in message          Auto/Manual 
#define MSG_SET_JOYINPUT            0x62   // 'b' in message       Joystick input      

#define MESSAGE_BUFFER_SIZE   64
/***********IR Protocol**************/
#define RECEIVER_PIN 11
#define POW 0xFFA25D
#define FUNC 0xFFE21D
#define SPACE 0xFF02FD
#define ZERO 0xFF6897
#define ONE 0xFF30CF
#define TWO 0xFF18E7
#define THREE 0xFF7A85
#define FOUR 0xFF10EF
#define END 0xFF9867
extern char msgBuffer[MESSAGE_BUFFER_SIZE]; 
extern int msgBufferPointer;

extern int joyThrottle ; // 
extern int joyYaw ; // 
extern int joyRoll ; // 
extern int joyPitch ; // 

typedef struct{
  unsigned long value;
  char charRemote;
}remoteMessageMap;

extern remoteMessageMap remoteMessagemapArray[22];

typedef struct{
  bool checkValidchar;
  char validChar;
}getValidchar;

extern getValidchar vChar;

void evaluateCommand();
void updateCommand();
void init_buffer();
void init_receiver();
void message_mapping();


void statusCommand();
void setMotorPower();
void setJoyInput();

#endif /* PROTOCOL_H_ */
