#ifndef Config_h
#define Config_h

#define PIN_JOY_X A8
#define PIN_JOY_Y A9
#define PIN_SW_1  53
#define PIN_SW_2  65
#define PIN_SW_3  0 //somehow no pin connection found
#define PIN_SW_4  61
#define PIN_SW_5  60
#define PIN_SW_6  59
#define PIN_SW_7  58



#define MESSAGE_BUFFER_SIZE              64
#define INTERVAL_JOYSTICK             30000     //0.03  
#define INTERVAL_OLED_UPDATE          100000      //0.1
#define INTERVAL_MESSAGE_CHECK         1000       //0.001

#define RES_PROTOCOL_MAX_BUF_SIZE   64

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)


#endif
