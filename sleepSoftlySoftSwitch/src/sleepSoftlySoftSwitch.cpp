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
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "IoTClassroom_CNM.h"
#include "Button.h"
#include <Keypad_Particle.h>

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

// create Keypad object 
Keypad softKeypad = Keypad( makeKeymap(softKeys), rowPins, colPins, ROWS, COLS );

void wemoOne();
void wemoTwo();
void moveLock();

int i;


SYSTEM_MODE(MANUAL); //so we can log in to classroom router

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);


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


}

// loop() runs over and over again, as quickly as it can execute.
void loop(){
  char keyNum = softKeypad.getKey(); 
  if (keyNum){
    Serial.println(keyNum); //checking to verify which key
  }
}

//defining functions

void wemoOne(){  // needs written

}
void wemoTwo(){  // needs written

}
void moveLock(){  // needs written

}
 