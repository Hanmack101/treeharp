/*
import libraries and basic structure 
use min distance - may be able to reuse a lot of the space segmenter previously built
- remove angle segmentation

-write bacis segmenter
-refactor to divide by the length of the NOTES array

for reference 
*  262, // C
  277, // C#
  294, // D
  311, // D#
  330, // E
  349, // F
  370, // F#
  392, // G
  415, // G#
  440, // A
  466, // A#
  494, // B
  523  // C next octave
*/





/// This sketch code is based on the RPLIDAR driver library provided by RoboPeak

#include <RPLidar.h>
#include <FastLED.h>
#include <Arduino.h>

// You need to create an driver instance 
RPLidar lidar;
CRGB leds[16];

// Change the pin mapping based on your needs.
/////////////////////////////////////////////////////////////////////////////
//#define LED_ENABLE  12 // The GPIO pin for the RGB led's common lead. 
#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor.
// This pin should connected with the RPLIDAR's MOTOCTRL signal 
//////////////////////////////////////////////////////////////////////////////

// ADDING PIEZO COMPONENTS ------------------------------------------------------------------

// Need GDE for twinkle twinkle G 392  D 294 E 330
const int NOTES[] = {
  392,  //G
  294,  //D
  330}; //E 
  
const byte PIEZO_PIN = 6; //connct buzzer to pin 6

// ADDING OTHER VARIABLES ------------------------------------------------------------------

float minDistance = 100000;
float angleAtMinDist = 0;

// SETUP ------------------------------------------------------------------

    void setup()
    {
    // bind the RPLIDAR driver to the arduino hardware serial
      lidar.begin(Serial2);
      FastLED.addLeds<NEOPIXEL, 6>(leds, 16);
      
      Serial.begin(9600);
    
    // set pin modes
      pinMode(RPLIDAR_MOTOR, OUTPUT);
      //digitalWrite(LED_ENABLE, HIGH);
    }


// FUNCTIONS ------------------------------------------------------------------------------ //

//------ BASIC SPACE SEGMENTER ------
//  void therminFunction(){
//    tone(PIEZO_PIN, angleAtMinDist + 100);
//  }

//------ BASIC SPACE SEGMENTER ------
//   void bLedFunction( ){
//    if(angleAtMinDist <180){
//      tone(PIEZO_PIN, NOTES[1]);//  ~ key C
//      Serial.println("blue light");
//      }
//  }

//------NESTED IF/ELSE SPACE SEGMENTER------
//void divideSpace(){} - use later
 void twinkle()
 {
        Serial.print("I am calling twinkle");
   }

// END OF FUNCTIONS ------------------------------------------------------------------------------ //

// SETUP ------------------------------------------------------------------

void loop()
{
  if (IS_OK(lidar.waitPoint()))
  {
    //perform data processing here...
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    //displayColor(distance, distance);

    if (lidar.getCurrentPoint().startBit)
    {
      if (minDistance < 100000)
      {
         Serial.println("ready to proceede with Harp playing");
           
           if(minDistance > 100 && minDistance < 250) // adjustable doughnut ring //add angle condition here (and keep calling twinkle)
           { 
            twinkle();
            

             }
             else 
             { 
                noTone(PIEZO_PIN);
             }
    }
    }
    
      else
      {
        if (distance > 0 && distance < minDistance)
        {
          minDistance = distance;
          angleAtMinDist = angle;
        }
      }
  }
  
  else
  {
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor
    
    // try to detect RPLIDAR... 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100)))
    {
      //detected...
      lidar.startScan();
      analogWrite(RPLIDAR_MOTOR, 255);
      delay(1000);
    }
  }
} //END OF LOOP



