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


#define LOGO16_GLCD_HEIGHT 64 
#define LOGO16_GLCD_WIDTH  128
static const unsigned char logo16_glcd_bmp[] =
{ 0B00000000, 0B11000000,
  0B00000001, 0B11000000,
  0B00000001, 0B11000000,
  0B00000011, 0B11100000,
  0B11110011, 0B11100000,
  0B11111110, 0B11111000,
  0B01111110, 0B11111111,
  0B00110011, 0B10011111,
  0B00011111, 0B11111100,
  0B00001101, 0B01110000,
  0B00011011, 0B10100000,
  0B00111111, 0B11100000,
  0B00111111, 0B11110000,
  0B01111100, 0B11110000,
  0B01110000, 0B01110000,
  0B00000000, 0B00110000 };





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
  Serial.printf("initialized to locked");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initalize adafruit OLCD


  //begin trying weird stuff for OLCD in setup

  // miniature bitmap display
  display.clearDisplay();
  display.drawBitmap(0, 0,  logo16_glcd_bmp, 16, 16, 1);
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
    }
    if (keyNum == '7') {  //upper right pressed, wemo B
      wemoBState = !wemoBState;
      wemoWrite(MYWEMO_B,wemoBState);
      Serial.printf("wemo #%d set to %d\n", MYWEMO_B, wemoBState);
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


 