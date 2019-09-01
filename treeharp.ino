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
//if you need more than 4 notes, at the moment you will need to manually add to the function.
const int NOTES[] = {
    330, // E
    494, // B
    392, // G
    440, // A
    494 // B
    };
  
const byte PIEZO_PIN = 6; //connct buzzer to pin 6

// ADDING OTHER VARIABLES ------------------------------------------------------------------

float minDistance = 100000;
float angleAtMinDist = 0;

  int maxDist = 300;   
  int noteArrayLength = sizeof(NOTES) / sizeof(NOTES[0]);
  int b = maxDist/ noteArrayLength;
  int chunkNum = 1;
  

  

// SETUP ------------------------------------------------------------------

    void setup()
    {
    // bind the RPLIDAR driver to the arduino hardware serial
      lidar.begin(Serial2);
      FastLED.addLeds<NEOPIXEL, 6>(leds, 16);
      
      Serial.begin(9600);
      Serial.print("The length of the array is");
      Serial.println(noteArrayLength);
    
    // set pin modes
      pinMode(RPLIDAR_MOTOR, OUTPUT);
      //digitalWrite(LED_ENABLE, HIGH);
    }


// FUNCTIONS ------------------------------------------------------------------------------ //

  // - found this as  an alternative to .length > int num_elements = sizeof( array ) / sizeof( array[0] );

 void playNotes(){
  chunkNum = minDistance/b;

 if (chunkNum >= -1 && chunkNum <= 1){
    tone(PIEZO_PIN, NOTES[0]);
        Serial.println("playing segment 1");
 }

    if (chunkNum >= 1.1 && chunkNum <= 2){
      tone(PIEZO_PIN, NOTES[1]);
      Serial.println("playing segment 2");
    }

        if (chunkNum >= 2.1 && chunkNum <= 3){
          tone(PIEZO_PIN, NOTES[2]);
          Serial.println("playing segment 3");
        }
            if (chunkNum >= 3.1 && chunkNum <= 4){
              tone(PIEZO_PIN, NOTES[3]);
              Serial.println("playing segment 4");
        }

          else if (chunkNum > (noteArrayLength + 1)){
                 noTone(PIEZO_PIN);
                 Serial.println("you're too far away");
          }
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
            
            playNotes();
            Serial.println(minDistance);  
            Serial.print("The current chunkNum is "); 
            Serial.println(chunkNum);  
        }
        else 
             { 
                noTone(PIEZO_PIN);
                Serial.println("LOOP I am not playing sound from the LOOP"); 
             }
      minDistance = 100000;
      //chunkNum = 11;
    }
    
      else
      {
        if (distance > 0 && distance < minDistance)
        {
          minDistance = distance;
          angleAtMinDist = angle;
          //chunkNum = 0;
          //Serial.println("I am not resetting");
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