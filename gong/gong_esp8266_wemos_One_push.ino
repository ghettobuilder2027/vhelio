#include <Arduino.h>

#ifdef ESP32
  #include "SPIFFS.h"
#else
#endif

#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id;

const int GONG_PIN = D5;
const int LONG_PRESS_TIME  = 140; // 1000 milliseconds


int lastState = LOW;  // the previous state from the input pin
int currentState;     // the current reading from the input pin
unsigned long pressedTime  = 0;
bool isPressing = false;
bool isLongDetected = false;
bool isShortDetected = false;

int count_long = 0;

void playSound(char *sound) {
  file->open(sound);
  id = new AudioFileSourceID3(file);
  mp3->begin(id, out);
}

void setup()
{
  Serial.begin(115200);
  delay(200);
  SPIFFS.begin();
  Serial.printf("Bienvenue\n");
  pinMode(GONG_PIN, INPUT_PULLUP);
   
  
  audioLogger = &Serial;
  out = new AudioOutputI2S(0,1,32,0);
  out->SetPinout(D8,D4,3);
  out->SetGain(0.03);
  out->SetRate(22050);
  file = new AudioFileSourceSPIFFS("/coincoin.mp3");
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);
  
}

void loop() {
  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  } 
  
  currentState = digitalRead(GONG_PIN);

  if(lastState == HIGH && currentState == LOW) {        // button is pressed
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;
  } else if(lastState == LOW && currentState == HIGH) { // button is released
    isPressing = false;
    if (count_long == 0 ) isShortDetected= true;
    count_long = 0;
  }

  if(isPressing == true && isLongDetected == false) {
    long pressDuration = millis() - pressedTime;

    if( pressDuration > LONG_PRESS_TIME ) {
      Serial.println("A long press is detected");
      isLongDetected = true;
      isShortDetected= false;
    }
    
  }

  // save the the last state
  lastState = currentState;

  
    
  if (isLongDetected) {
    count_long++;
    Serial.print("count long = ");
    Serial.println (count_long);
    isLongDetected = isShortDetected = false;
    Serial.println (" long push");
    if (count_long == 1 ) ;//playSound("/gong.mp3");
  }
  if(isShortDetected) {
    if (mp3->isRunning()) mp3->stop();
    isLongDetected = isShortDetected = false;
    Serial.println (" short push");
    if (count_long == 0) playSound("/coincoin.mp3");
    
  }

  
 
}
