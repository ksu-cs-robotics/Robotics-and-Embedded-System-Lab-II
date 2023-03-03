#ifndef MYPROTOCOL_H
#define MYPROTOCOL_H


#define UGV_VERSION              1

#define MSG_GET_ECHO                 0x31   //'1' in message       message echo         
#define MSG_GET_SERVO                0x32   //'2' in message      
#define MSG_SET_SERVO                0x33   //'3' in message      
#define MSG_GET_SPEED                0x38   //'8' in message      
#define MSG_SET_SPEED                0x39   //'9' in message      

#define MESSAGE_BUFFER_SIZE          64

#define PIN_JOYSTICK_X    A0
#define PIN_JOYSTICK_Y    A1
#define PIN_JOYSTICK_SW    A2
#define INTERVAL_JOYSTICK   10
#define INTERVAL_IRSEND   1


#define FRAME_START_SYMBOL  '(' 
#define FRAME_END_SYMBOL  ')' 
#define FRAME_MODE_SYMBOL  '$' 
#define FRAME_DELIMITER_SYMBOL  ',' 

typedef struct{
  unsigned long value;
  char charRemote;
}remoteMessageMap;

// this array is used to map the IR remote key to a specific character
remoteMessageMap remoteMessagemapArray[] = {{16753245,'p'},{16736925,'v'},{16769565,'f'},{16720605,FRAME_START_SYMBOL},{16712445,FRAME_DELIMITER_SYMBOL},
                                            {16761405,FRAME_END_SYMBOL},{16769055,'/'},{16754775,'-'},{16748655 ,'#'},{ 16738455,'0'},
                                            {16750695 ,FRAME_MODE_SYMBOL},{ 16756815,'$'},{ 16724175,'1'},{16718055 ,'2'},{16743045 ,'3'},
                                            {16716015 ,'4'},{16726215 ,'5'},{16734885 ,'6'},{ 16728765,'7'},{ 16730805,'8'},
                                            {16732845 ,'9'},{4294967295 ,'I'}};





char convertRemoteSymboltoChar(unsigned long argValue){
  //this function returns a specific character based on the map table when a button in the remote is pressed
  for (int i=0;i<22;i++){
    if (remoteMessagemapArray[i].value == argValue){
      return remoteMessagemapArray[i].charRemote;
    }    
  }
  return 0x00;
}

long convertCharRemoteLong(char argChar){
  //this function returns a specific character based on the map table when a button in the remote is pressed
  for (int i=0;i<22;i++){
    if (remoteMessagemapArray[i].charRemote == argChar){
      return remoteMessagemapArray[i].value;
    }    
  }
  return 0;
}



#endif // MYPROTOCOL_H
