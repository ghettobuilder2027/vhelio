#include <Arduino.h>
#include <Button2.h>         // https://github.com/LennartHennigs/Button2


#ifdef ESP32
  #include "SPIFFS.h"
#else
  #include <ESP8266WiFi.h>
#endif
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"


AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *duck,*coincoin;
AudioOutputI2S *out;
AudioFileSourceID3 *id_duck,*id_coincoin;

const int gongPin = 12;
Button2 gongBtn = Button2(gongPin);



void press(Button2& btn) {
  if (btn == gongBtn) {
    file = new AudioFileSourceSPIFFS("/coincoin.mp3");
    id = new AudioFileSourceID3(file);
    mp3->begin(id, out);

  }
}
void setup()
{
  Serial.begin(115200);
  delay(200);
  SPIFFS.begin();
  Serial.printf("Bienvenue\n");
  gongBtn.setTapHandler(press);

  audioLogger = &Serial;
  file = new AudioFileSourceSPIFFS("/duck.mp3");
  id = new AudioFileSourceID3(duck);
  
  out = new AudioOutputI2S(0,1);
  mp3 = new AudioGeneratorMP3();
  
  mp3->begin(id, out);
}

void loop()
{
  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  } else {
     gongBtn.loop();
  }
}
