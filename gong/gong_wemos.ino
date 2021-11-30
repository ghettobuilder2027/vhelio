#include <Arduino.h>
#include <Button2.h>         // https://github.com/LennartHennigs/Button2

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

const int gongPin = D5;
Button2 gongBtn = Button2(gongPin);
const int coincoinPin = D7;
Button2 coincoinBtn = Button2(coincoinPin);

void playSound(char *sound) {
  file = new AudioFileSourceSPIFFS(sound);
  id = new AudioFileSourceID3(file);
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id, out);

}

void press(Button2& btn) {
  if (btn == gongBtn) {
    Serial.println (" gong pushed");
    playSound("/gong.mp3");
  }
  if (btn == coincoinBtn) {
    Serial.println (" coincoin pushed");
    playSound("/coincoin.mp3");
  }
  
}
void setup()
{
  Serial.begin(115200);
  delay(200);
  SPIFFS.begin();
  Serial.printf("Bienvenue\n");
  gongBtn.setPressedHandler(press);
  coincoinBtn.setPressedHandler(press);

  audioLogger = &Serial;
  out = new AudioOutputI2S(0,1);
  out->SetPinout(D8,D4,3);
  playSound("/coincoin.mp3");
  
}

void loop()
{
  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  } else {
     gongBtn.loop();
     coincoinBtn.loop();
  }
}
