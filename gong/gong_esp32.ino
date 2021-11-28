#include <Button2.h>         // https://github.com/LennartHennigs/Button2
#include <Audio.h>           // https://github.com/schreibfaul1/ESP32-audioI2S
#include "Arduino.h"

 #ifdef ESP32
  #include "SPIFFS.h"
  #include "WiFi.h"
#else
  #include <ESP8266WiFi.h>
#endif
// Digital I/O used
#define I2S_DOUT      33 // DIN connection   22
#define I2S_BCLK      26  // Bit clock        25
#define I2S_LRC       25    // Left Right Clock 26

Audio audio;

const int gongPin = 12;
Button2 gongBtn = Button2(gongPin);



unsigned int volume =  21;


void press(Button2& btn) {
  if (btn == gongBtn) {
    Serial.println("coincoin pressed");
    audio.connecttoFS (SPIFFS, "gong.wav");
    audio.loop();
   

  }
}



void setup() {
  Serial.begin(115200);
  // Initialise la memoire flash (là ou sont stockés les mp3
  SPIFFS.begin(true); 
  // Initialise la connexion par I2S entre l'ESP et le max98357A
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  // Instancie une alerte sur le bouton qui appelera la fonction press si on fait un tap sur gongBtn
  //gongBtn.setTapHandler(press);
  gongBtn.setPressedHandler(press);

  
  // Définit le volume de base 
  audio.setVolume(volume); // 0...21
  //audio.connecttoFS (SPIFFS, "duck.wav");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (audio.isRunning()) {
    audio.loop();
    //Serial.println("running loop");
  }
  gongBtn.loop();

 
}

  
