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


char *sons[] {
  "/2poire2.mp3",
  "/coincoin.mp3",
  "/duck.mp3",
  "/gong_coin_gong_vraiment_grave.mp3",
  "/gong.mp3",
  "/grenouille.mp3",
  "/mp3.txt",
  "/paquebot.mp3",
  "/train1.mp3",
  "/voiture.mp3"
};
int nb_sons = 10;
char *son ;

const int gongPin = D5;
Button2 gongBtn = Button2(gongPin);
const int changePin = D7;
Button2 changeBtn = Button2(changePin);

void playSound(char *sound) {
  file->open(sound);
  id = new AudioFileSourceID3(file);
  mp3->begin(id, out);
}

void press(Button2& btn) {
  if (btn == gongBtn) {
    if (mp3->isRunning()) mp3->stop(); 
    Serial.println (" gong pushed");
    playSound("/gong.mp3");
  }
  if (btn == changeBtn) {
    Serial.println (" change pushed");
    
  }
  
}
void setup()
{
  Serial.begin(115200);
  delay(200);
  SPIFFS.begin();
  Serial.printf("Bienvenue\n");
  gongBtn.setPressedHandler(press);
  changeBtn.setPressedHandler(press);

  *son = sons[0];
  Serial.println(son);

  audioLogger = &Serial;
  out = new AudioOutputI2S(0,1,32,0);
  out->SetPinout(D8,D4,3);
  out->SetGain(3.99);
  out->SetRate(22050);
  file = new AudioFileSourceSPIFFS("/coincoin.mp3");
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);
  
}

void loop()
{
  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  } 
  gongBtn.loop();
  coincoinBtn.loop();
}
