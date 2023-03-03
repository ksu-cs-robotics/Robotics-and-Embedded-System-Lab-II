#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/************************************** ATR Lab Simple UGV Protocol *******************************************************/
#define UGV_VERSION              1

#define MSG_GET_ECHO                 0x31   // '1' in message       message echo         
#define MSG_GET_SERVO                0x32   //'2' in message      
#define MSG_SET_SERVO                0x33   //'3' in message      
#define MSG_GET_SELECTION            0x34   //'4' in message      
#define MSG_SET_SELECTION            0x35   //'5' in message      
#define MSG_GET_SPEED                0x38   //'8' in message      
#define MSG_SET_SPEED                0x39   //'9' in message      

#define FRAME_START_SYMBOL  '<' 
#define FRAME_END_SYMBOL  '>' 
#define FRAME_MODE_SYMBOL  '@' 
#define FRAME_DELIMITER_SYMBOL  ' ' 


#define MESSAGE_BUFFER_SIZE          64


extern char msgBuffer[MESSAGE_BUFFER_SIZE]; 
extern int msgBufferPointer;
extern char receiveChar;
extern int servoAngle[3];
extern int motorSpeed[2];

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
void printTmpchar();
void turnOff();
void printFinalstring();
void printServoValue();




#endif /* PROTOCOL_H_ */
