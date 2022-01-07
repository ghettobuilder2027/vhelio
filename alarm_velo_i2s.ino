#include <Arduino.h>
#include <Button2.h>         // https://github.com/LennartHennigs/Button2

#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"


// Libraries extracted from arduino repertory in https://github.com/jrowberg/i2cdevlib
#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif


AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id;


char* sons[] = {
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
MPU6050 accelgyro;

int16_t ax, ay, az, dx, dy, dz ;
int16_t gx, gy, gz, hx, hy, hz, jilt;

const int gongPin = D7;
Button2 gongBtn = Button2(gongPin);


int start = 0;
int total = 0;
bool sound = true ;// Set to true to really make noise (debug without noise is better...)
bool tentativeVol = false;
int deplacement = 0;
unsigned long myTime;
unsigned long delayTime;



const int nombreSons = sizeof(sons) / sizeof(sons[0]);

int indexSon = 0;

void changeSon() {
  indexSon = indexSon + 1;
  if (indexSon == nombreSons)
    indexSon = 0;
}

char* sonActuel() {
  return sons[indexSon];
}
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
    Serial.println(sonActuel());

    playSound(sonActuel());
  }
  /* if (btn == changeBtn) {
     Serial.println (" change pushed");
    changeSon();
    }*/

}


void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  Serial.begin(115200);
  delay(200);
  SPIFFS.begin();
  Serial.printf("Bienvenue\n");
  gongBtn.setPressedHandler(press);
  Serial.begin(115200);
  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  audioLogger = &Serial;
  out = new AudioOutputI2S(0, 1, 32, 0); //
  out->SetPinout(D8, D4, 3);          // bool AudioOutputI2S::SetPinout(int bclk, int wclk ou LRC, int dout)
  out->SetGain(0.9);
  out->SetRate(22050);
  Serial.println(sonActuel());
  file = new AudioFileSourceSPIFFS(sonActuel());
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);
  // delay before alarm is on
  delay(2000);
}

void loop() {
  gongBtn.loop();

  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  }
  else {
    // read raw accel/gyro measurements from device
    dx = ax; dy = ay; dz = az; hx = gx; hy = gy; hz = gz;
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    dx = dx - ax; dy = dy - ay; dz = dz - az;
    hx = hx - gx; hy = hy - gy; hz = hz - gz;

    // Mean of the different accelerations....
    jilt = abs(dx / 40) + abs(dy / 40) + abs(dz / 40) + (abs(hx / 6) + abs(hy / 6) + abs(hz / 6));

    Serial.print("a/g:\t");
    Serial.print(dx); Serial.print("\t"); Serial.print(dy); Serial.print("\t"); Serial.print(dz); Serial.print("\t");
    Serial.print(hx); Serial.print("\t"); Serial.print(hy); Serial.print("\t"); Serial.print(hz); Serial.print("\t");
    Serial.print(jilt); Serial.println("");

    // Threshold for mouvment
    if (jilt > 80) {
      if (start % 2 != 0) {
        total++;
        alarm();
        if (start > 10) start = start % 2;
      }
      start++;
    }
    // delay between 2 measurements
    else delay(200);
  }
}


void alarm () {
  if (!tentativeVol) {
    //first time
    tentativeVol = true;
    myTime = millis();
    deplacement = 1;
    Serial.println("First time ");
    beep();

  }
  else {
    deplacement++;
    delayTime = millis() - myTime;
    if (delayTime > 60000) {
      Serial.println(" First movment after 1 minute");
      beep();
      tentativeVol = true;
      deplacement = 1;
      myTime = millis();

    }
    else {
      Serial.print (" deplacement number :");
      Serial.println(deplacement);
      int i;
      for (i = 0; i < deplacement; i++) {
        //  will ring more and more within a minute
        beep();

      }
    }
  }
}
void beep () {
  int j;
  Serial.print("Beeeep  start = ");
  Serial.println(start);
  if (sound) {
    indexSon = 6;
    playSound(sonActuel());

  }
  else {
    delay(1340);
  }
}
