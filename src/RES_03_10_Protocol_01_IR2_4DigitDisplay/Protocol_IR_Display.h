#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/************************************** ATR Lab Simple UAV Protocol *******************************************************/
#define UAV_VERSION              1

#define MSG_GET_ECHO                 0x31   // '1' in message       message echo         
#define MSG_EVALUATE_BINARY          0x32   //'2' in message      
#define MESSAGE_BUFFER_SIZE          64


extern char msgBuffer[MESSAGE_BUFFER_SIZE]; 
extern int msgBufferPointer;
extern char receiveChar;

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
void init_pin_mode();
void message_mapping();
void printTmpchar();
void turnOff();
void printLED();
void printFinalstring();




#endif /* PROTOCOL_H_ */
