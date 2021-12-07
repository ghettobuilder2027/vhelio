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


char* sons[]= {
  "/2poire2.mp3",
  "/coincoin.mp3",
  "/duck.mp3",
  "/gong_grave.mp3",
  "/gong.mp3",
  "/grenouille.mp3",
  "/paquebot.mp3",
  "/train1.mp3",
  "/voiture.mp3",
  "/ruecapclaude.mp3",
  "/ruecapclaude2.mp3",
  "/ruecapclaude5.mp3"
};

const int nombreSons = sizeof(sons) / sizeof(sons[0]);

int indexSon = 0;

void changeSon() {
  indexSon = indexSon + 1;
  if(indexSon == nombreSons)
    indexSon = 0;
}

char* sonActuel() {
  return sons[indexSon];
}
const int gongPin = D5;
Button2 gongBtn = Button2(gongPin);
const int changePin = D7;
Button2 changeBtn = Button2(changePin);

void playSound(char *sound) {
  file->open(sound);
  out->SetGain(3.9);
  id = new AudioFileSourceID3(file);
  mp3->begin(id, out);
}

void press(Button2& btn) {
  if (btn == gongBtn) {
    if (mp3->isRunning()) mp3->stop(); 
    Serial.println (" gong pushed");
    Serial.print("playing sound : ");
   
    playSound(sonActuel());
  }
  if (btn == changeBtn) {
    Serial.println (" change pushed");
   changeSon();
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

 
  audioLogger = &Serial;
  out = new AudioOutputI2S(0,1,32,0);
  out->SetPinout(D8,D4,3);
  out->SetGain(0.9);
  out->SetRate(22050);
  file = new AudioFileSourceSPIFFS(sonActuel());
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);
  
}

void loop()
{
  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  } 
  gongBtn.loop();
  changeBtn.loop();
}
