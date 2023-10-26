/* 
 * Project: CNM IoT cohort 11 - midterm smart room controller
            "Sleep Softly Soft Switch"
 * Author: Rebecca Snyder
 * Date: 2023-10-26
 * 
 * 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 * 
 * 
 * Must install libary "IoTClassroom_CNM" with ctrl-shift-p
 * must install library "Keypad_Particle" with ctrl-shift-p
 * must install library "Adafruit_SSD1306" with ctrl-shift-p
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "IoTClassroom_CNM.h"
#include "Button.h"
#include "Wemo.h"
#include <Keypad_Particle.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#define OLED_RESET D4

//declare global constants and variables related to soft keypad
const byte ROWS = 3;  
const byte COLS = 3;
char keyNum;
char softKeys [ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
};
byte rowPins[ROWS] = { D16, D15, D17 };
byte colPins[COLS] = { D18, D19, D14 };

//declare global constants and variables related to hue lights
const int BULB=3; 
int color;
int hueBrightness;
bool hueTurnOn;

const int MYWEMO_A = 2; // to be controled by upper left
const int MYWEMO_B = 2; // TESTING change to a different wemo later, to be controlled by upper right
bool wemoAState;
bool wemoBState;

const int MOTORPIN = D13; // for servo (lock function)
Servo myServo;
bool servoState;

// create Keypad object 
Keypad softKeypad = Keypad( makeKeymap(softKeys), rowPins, colPins, ROWS, COLS );

Adafruit_SSD1306 display(OLED_RESET); //declare object "display"






SYSTEM_MODE(MANUAL); //so we can log in to classroom router

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);




//begin trying out adafruit bitmap thing


//#define LOGO16_GLCD_HEIGHT 64 
//#define LOGO16_GLCD_WIDTH  128
static const unsigned char DDC_bmp[] = // "deep dive coding" bitmap
{ // 'dd', 128x64px
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xf3, 0xff, 0xff, 0xc0, 0x0e, 0x00, 0xc0, 0x0c, 0x01, 0xfc, 0x01, 0xc0, 0x83, 0x04, 0x00,
0xff, 0xf3, 0xff, 0xff, 0xc0, 0x0e, 0x00, 0xc0, 0x0c, 0x00, 0xfc, 0x00, 0xc1, 0x83, 0x04, 0x00,
0xff, 0xf3, 0xff, 0xff, 0xf3, 0xc7, 0x3c, 0xf3, 0xce, 0x38, 0xfe, 0x38, 0xf3, 0xc7, 0x9e, 0x3c,
0xff, 0x00, 0x7f, 0xff, 0xf3, 0xc7, 0x3f, 0xf3, 0xfe, 0x38, 0xfe, 0x38, 0xf3, 0xe7, 0x9e, 0x3f,
0xfe, 0x7f, 0x1f, 0xff, 0xf3, 0xc7, 0x07, 0xf0, 0x7e, 0x38, 0xfe, 0x3c, 0xf3, 0xe3, 0x1e, 0x07,
0xfc, 0xff, 0xcf, 0xff, 0xf3, 0xc7, 0x07, 0xf0, 0x7e, 0x00, 0xfe, 0x3c, 0xf3, 0xe3, 0x3e, 0x07,
0xfb, 0xff, 0xe7, 0xff, 0xf3, 0xc7, 0x1f, 0xf3, 0xfe, 0x01, 0xfe, 0x3c, 0xf3, 0xe3, 0x3e, 0x3f,
0xf3, 0xff, 0xf3, 0xff, 0xf3, 0xc7, 0x3c, 0xf3, 0xce, 0x3f, 0xfe, 0x38, 0xf3, 0xf2, 0x3e, 0x3c,
0xf7, 0x80, 0x7b, 0xff, 0xf3, 0xc7, 0x3c, 0xf3, 0xce, 0x3f, 0xfe, 0x38, 0xf3, 0xf0, 0x3e, 0x3c,
0xe7, 0x61, 0x39, 0xff, 0xc0, 0x0e, 0x00, 0xc0, 0x0c, 0x1f, 0xfc, 0x00, 0xc0, 0xf8, 0x7c, 0x00,
0xce, 0x9e, 0x58, 0xff, 0xc0, 0x0e, 0x00, 0xc0, 0x0c, 0x1f, 0xfc, 0x01, 0xc0, 0xf8, 0x7c, 0x00,
0x9e, 0x2d, 0x1e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0x9d, 0x73, 0x2e, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xbc, 0xb3, 0xee, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x3c, 0x0f, 0xff, 0xff, 0xff, 0xff,
0xbc, 0xcc, 0xee, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x3c, 0x0f, 0xff, 0xff, 0xff, 0xff,
0xbc, 0xcc, 0xee, 0x7f, 0xf8, 0x00, 0x7f, 0xff, 0xff, 0xe0, 0x3c, 0x0f, 0xff, 0xff, 0xff, 0xff,
0xbd, 0x37, 0x2e, 0x7f, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
0x9d, 0x3b, 0x0e, 0xff, 0xf0, 0x00, 0x3f, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xc6, 0x9c, 0x58, 0xff, 0xe0, 0x00, 0x3e, 0x00, 0xfe, 0x18, 0x38, 0x0e, 0x07, 0x0f, 0xe0, 0x80,
0xf7, 0x4c, 0xbb, 0xff, 0xe0, 0x7c, 0x1c, 0x00, 0x78, 0x00, 0x38, 0x0e, 0x04, 0x07, 0xc0, 0x00,
0xf3, 0x9e, 0x7b, 0xff, 0xe0, 0x7c, 0x18, 0x00, 0x38, 0x00, 0x38, 0x0e, 0x00, 0x03, 0x80, 0x00,
0xf3, 0xc1, 0xf3, 0xff, 0xe0, 0x7c, 0x18, 0x00, 0x30, 0x00, 0x38, 0x0e, 0x00, 0x03, 0x80, 0x00,
0xf7, 0xff, 0xfb, 0xff, 0xe0, 0x7f, 0xf8, 0x38, 0x30, 0x78, 0x3e, 0x0f, 0x07, 0x83, 0x83, 0x83,
0x87, 0xff, 0xf8, 0xff, 0xe0, 0x7f, 0xf8, 0x38, 0x30, 0x78, 0x3e, 0x0f, 0x07, 0x83, 0x83, 0x83,
0x03, 0xff, 0xf0, 0x3f, 0xe0, 0x7f, 0xf8, 0x38, 0x30, 0x78, 0x3e, 0x0f, 0x07, 0x83, 0x07, 0x83,
0x77, 0xff, 0xff, 0x3f, 0xe0, 0x7c, 0x18, 0x38, 0x30, 0x78, 0x3e, 0x0f, 0x07, 0x83, 0x07, 0x83,
0x3f, 0xff, 0xff, 0x7f, 0xe0, 0x7c, 0x18, 0x38, 0x30, 0x78, 0x3e, 0x0f, 0x07, 0x83, 0x83, 0x83,
0xbf, 0xff, 0xff, 0x7f, 0xe0, 0x7c, 0x18, 0x38, 0x30, 0x78, 0x3e, 0x0f, 0x07, 0x83, 0x83, 0x83,
0xbf, 0xfb, 0xfe, 0x7f, 0xe0, 0x78, 0x38, 0x38, 0x30, 0x38, 0x3e, 0x0f, 0x07, 0x83, 0x80, 0x03,
0x9f, 0x9b, 0x7e, 0xff, 0xf0, 0x00, 0x38, 0x00, 0x30, 0x00, 0x08, 0x06, 0x03, 0x01, 0x80, 0x03,
0xdf, 0x96, 0x7e, 0xff, 0xf0, 0x00, 0x38, 0x00, 0x38, 0x00, 0x08, 0x06, 0x03, 0x01, 0xc0, 0x83,
0xcf, 0xf7, 0xfd, 0xff, 0xf8, 0x00, 0x7c, 0x00, 0x78, 0x08, 0x08, 0x06, 0x03, 0x01, 0xf3, 0x83,
0xef, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83,
0xe7, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0x83,
0xf1, 0xff, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03,
0xfc, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x03,
0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x07,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc5, 0xad, 0xdf, 0xff, 0xff, 0x7f,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xbf, 0xdc, 0xa4, 0xd7, 0xfb, 0x35, 0xcf,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x90, 0xa2, 0x01, 0x85, 0xbc, 0x20, 0xd5, 0xf5, 0x55, 0x58,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xa5, 0x12, 0x85, 0xa9, 0x9d, 0x22, 0xd5, 0xf1, 0x55, 0x59,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x91, 0x52, 0x81, 0x8b, 0xc5, 0x2a, 0xd5, 0xf1, 0x51, 0x49,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };





//bitmap for "Internet of Things"
static const unsigned char IoT_bmp[] =  // "Internet of things" bitmap
{// 'graphicMaybeForBitmapIoT', 128x64px
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xfc, 0x38, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xf0, 0x9c, 0x1c, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0x19, 0xb9, 0x9e, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xfc, 0x18, 0x38, 0x1e, 0x67, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xf8, 0x7c, 0x18, 0x3c, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xda, 0x3c, 0xc8, 0x3c, 0x26, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0x9b, 0x3c, 0xc1, 0x19, 0x0e, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0x1f, 0x3c, 0x00, 0x83, 0x0e, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xcf, 0x98, 0xff, 0xf1, 0x9c, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xf8, 0x67, 0x8f, 0xff, 0xff, 0x98, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xf0, 0x03, 0x9f, 0xff, 0xff, 0x90, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xec, 0x01, 0xff, 0xff, 0xff, 0xc3, 0x31, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xce, 0x7b, 0xff, 0xff, 0xff, 0xe3, 0x70, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0x87, 0x3f, 0xff, 0xff, 0xff, 0xf9, 0x7c, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xe3, 0x8f, 0xff, 0xff, 0xff, 0xfc, 0x78, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xf1, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x87, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xfc, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x83, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x79, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x71, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x84, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe6, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe6, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x06, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8e, 0x27, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdc, 0x71, 0xff, 0xfe, 0x3f, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x20, 0x7f, 0xc8, 0x3f, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x80, 0x70, 0x09, 0xbf, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xc8, 0x30, 0xe8, 0x7f, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x98, 0x11, 0xe0, 0x1f, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x12, 0x11, 0xc7, 0x1f, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x82, 0xc1, 0xe1, 0x9f, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc2, 0xc0, 0xe4, 0x1f, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0xe4, 0x44, 0x7f, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x66, 0x0f, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
//end trying out adafruit bitmap thing




void setup() {
  Serial.begin(9600);
  waitFor(Serial.isConnected,15000);

  WiFi.on();
  WiFi.clearCredentials();
  WiFi.setCredentials("IoTNetwork"); //connecting to classroom router
  
  WiFi.connect();
  while(WiFi.connecting()) {
    Serial.printf(".");
  }
  Serial.printf("\n\n");

  hueBrightness = 1; //start light at lower level
  color = 0; //start with red
  hueTurnOn = true; //start with hue light on
  delay(5000); // waiting for network to wake
  setHue(BULB,hueTurnOn,HueRainbow[color%7],hueBrightness,255); // initialize the hue light
  wemoAState = false;
  wemoBState = false;
  wemoWrite(MYWEMO_A, wemoAState);
  wemoWrite(MYWEMO_B,wemoBState);
  myServo.attach(MOTORPIN);
  servoState = 0;
  myServo.write(180); //locks when rebooting or reflashing
  Serial.printf("initialized to locked\n");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initalize adafruit OLCD


  //begin trying weird stuff for OLCD in setup

  // miniature bitmap display
  display.clearDisplay();
  display.drawBitmap(0, 0, DDC_bmp, 128, 64, 1); //deep dive coding bitmap on OLCD
  display.display();
  Serial.printf("setup drawing on OLCD");
  // end trying weird stuff for OLCD in setup
}




// loop() runs over and over again, as quickly as it can execute.
void loop(){
  char keyNum = softKeypad.getKey(); 
  if (keyNum){
    Serial.println(keyNum); //print the key number for debugging
    if (keyNum == '2') { // increment color if lower center pressed
      color++;
      Serial.printf("Setting bulb %i to color %06i\n",BULB,HueRainbow[color%7]);
      setHue(BULB,hueTurnOn,HueRainbow[color%7],hueBrightness,255);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.printf("Follow iotfreq on Instagram for the latest updates on IoT boot camp\n");
      display.display(); //sends to OLCD
    }
    if (keyNum == '4') {  // lower right pressed
      if (hueBrightness <= 255) {
        hueBrightness = hueBrightness + 50; // increase brightness by an amount
        hueTurnOn = true; // ensure light is on
        Serial.printf("Setting bulb %i to color %06i\n",BULB,HueRainbow[color%7]);
        setHue(BULB,hueTurnOn,HueRainbow[color%7],hueBrightness,255);
      }
    }
    if (keyNum == '3') {  //lower left pressed
      if (hueBrightness > 0) {
        hueBrightness = hueBrightness - 50; //decrease by an amount
        Serial.printf("Setting bulb %i to color %06i\n",BULB,HueRainbow[color%7]);
        setHue(BULB,hueTurnOn,HueRainbow[color%7],hueBrightness,255);
      }
      else {
        hueTurnOn = false; // turn it off if the brightness is zero or less
        Serial.printf("Setting bulb %i to color %06i\n",BULB,HueRainbow[color%7]);
        setHue(BULB,hueTurnOn,HueRainbow[color%7],hueBrightness,255);
      }

    }
    if (keyNum == '6') {   // upper left pressed, wemo A
      wemoAState = !wemoAState;
      wemoWrite(MYWEMO_A, wemoAState);
      Serial.printf("wemo #%d set to %d\n", MYWEMO_A, wemoAState);
      display.clearDisplay();
      display.drawBitmap(0, 0, IoT_bmp, 128, 64, 1); //deep dive coding bitmap on OLCD
      display.display();
      Serial.printf("wemo A, IOT drawing on OLCD");
    }
    if (keyNum == '7') {  //upper right pressed, wemo B
      wemoBState = !wemoBState;
      wemoWrite(MYWEMO_B,wemoBState);
      Serial.printf("wemo #%d set to %d\n", MYWEMO_B, wemoBState);
      display.clearDisplay();
      display.drawBitmap(0, 0, IoT_bmp, 128, 64, 1); //deep dive coding bitmap on OLCD
      display.display();
      Serial.printf("wemo B, IOT drawing on OLCD");
    }
    if (keyNum == '8')  { // upper center, lock or unlock with servo
      if (servoState){
        myServo.write(180); //lock
        servoState = !servoState;
        Serial.printf("locked \n");
        delay(100);
      }
      else{
       myServo.write(0); //unlock
       servoState = !servoState;
       Serial.printf("unlocked \n");
       delay(100);
      }
    }
  }
}


 