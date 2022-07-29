/*
Flight modes:
0 - On pad, idle/ no or go
1 - Stage 1
2 - Stage 2
3 - Balistic decent
4 - Chute decent
5 - Touchdown
6 - Abort

Mode desc:
0 - waiting for liftoff/no or go
1 - In stage one flight
2 - In stage two flight
3 - Ballistic missile coming down
4 - Chutes deploy and rocket is drifting down
5 - rocket is on the ground chillin

Modes:
0 - Pre flight checks
1 -
2 - Lights second stage motors
3 - Waits for BMP to detect deploy height
4 - Fire pyro 3 and 4 for chute deploy
5 - Wait

Mode colours
0 - Blue solid untill checks are done, if checks fail red flashing. If checks ok green solid
1 - Blue solid
2 - Blue solid
3 - Red solid
4 - Blue solid
5 - Green solid
6 - Red flashing

Switch cases:
0-1 - MPU detects constant acceleration
1-2 - MPU detects decelleration then time delay till switch
2-3 - MPU detects decelleration
3-4 - BMP detects rocket is below 100 ft from takeoff height
4-5 - MPU detects NO movement for 2 seconds
*/

#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Servo.h>
#include "MPU6050.h"

int R_LED = 9;
int G_LED = 2;
int B_LED = 6;
int Buzzer = 10;

int state = 0;
int abortstate = 1;

Adafruit_BMP280 bmp;

int16_t ax, ay, az;
int16_t gx, gy, gz;
#define OUTPUT_READABLE_ACCELGYRO

Sd2Card card;
SdVolume volume;
SdFile root;
const int SDchipSelect = 0;

int Pyro1 = 20;
int Pyro2 = 21;
int Pyro3 = 22;
int Pyro4 = 23;

void setup() {
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);

  pinMode(Pyro1, OUTPUT);
  pinMode(Pyro2, OUTPUT);
  pinMode(Pyro3, OUTPUT);
  pinMode(Pyro4, OUTPUT);


  digitalWrite(R_LED, HIGH);
  digitalWrite(G_LED, HIGH);
  digitalWrite(B_LED, HIGH);

  digitalWrite(Pyro1, LOW);
  digitalWrite(Pyro2, LOW);
  digitalWrite(Pyro3, LOW);
  digitalWrite(Pyro4, LOW);
}

void loop(){
  while(state == 0){
    digitalWrite(B_LED, LOW);
    Serial.println();Serial.println();
    Serial.println("Flight computor ready for duty!");
    Serial.println();
    Serial.println();

    //Now Barometer
    Serial.println("Checking barometer continuity");
    delay(1000);
    if (!bmp.begin()) {
      Serial.println("Could not find the BMP280 sensor");
      state = 6;
      break;
      delay(5000);
      Serial.println("ABORT");
    }
    else{
      bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
      Serial.println("Found the BMP280 sensor! Here's some data...");
      Serial.println();
      Serial.print(F("Temperature = "));
      Serial.print(bmp.readTemperature());
      Serial.println(" *C");

      Serial.print(F("Pressure = "));
      Serial.print(bmp.readPressure());
      Serial.println(" Pa");

      Serial.print(F("Approx altitude = "));
      Serial.print(bmp.readAltitude(1013.25));
      Serial.println(" m"); Serial.println();
      delay(1000);
    }

    //Now the IMU
    Serial.println("Checking MPU 6050 continuity");
    delay(1000);
    Serial.println();
    Serial.println("MPU6050 Detected!");
    delay(1000);


    //Now the SD card
    Serial.print("Chekcing SD card");
    if (!card.init(SPI_HALF_SPEED, SDchipSelect)) {
      state = 6;
      break;
      Serial.println();
      delay(4000);
    } else {
      Serial.println("Found an SD card! Here's some information.");
      delay(1000);
      Serial.println();
      Serial.print("Card type:         ");
      switch (card.type()) {
        case SD_CARD_TYPE_SD1:
          Serial.println("SD1");
          break;
        case SD_CARD_TYPE_SD2:
          Serial.println("SD2");
          break;
        case SD_CARD_TYPE_SDHC:
          Serial.println("SDHC");
          break;
        default:
          Serial.println("Unknown");
      }
      if (!volume.init(card)) {
      Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
      state = 6;
      break;
    }
    Serial.print("Clusters:          ");
    Serial.println(volume.clusterCount());
    Serial.print("Blocks x Cluster:  ");
    Serial.println(volume.blocksPerCluster());
    Serial.print("Total Blocks:      ");
    Serial.println(volume.blocksPerCluster() * volume.clusterCount());
    Serial.println();

    uint32_t volumesize;
    Serial.print("Volume type is:    FAT");
    Serial.println(volume.fatType(), DEC);

    volumesize = volume.blocksPerCluster();
    volumesize *= volume.clusterCount();
    volumesize /= 2;
    Serial.print("Volume size (Kb):  ");
    Serial.println(volumesize);
    Serial.print("Volume size (Mb):  ");
    volumesize /= 1024;
    Serial.println(volumesize);
    Serial.print("Volume size (Gb):  ");
    Serial.println((float)volumesize / 1024.0);
    Serial.println();
    Serial.println();
    delay(1000);
    }
    Serial.println("Testing buzzer");
    delay(500);
    tone(Buzzer, 2000); delay(50); noTone(Buzzer); delay(75);
    tone(Buzzer, 2000); delay(50); noTone(Buzzer); delay(200);
    tone(Buzzer, 1000); delay(50); noTone(Buzzer); delay(75);
    tone(Buzzer, 1000); delay(50); noTone(Buzzer); delay(400);

    tone(Buzzer, 1319); delay(50); noTone(Buzzer);
    delay(50);
    tone(Buzzer, 1760); delay(50); noTone(Buzzer);
    delay(50);
    tone(Buzzer, 2217); delay(50); noTone(Buzzer);
    delay(50);
    tone(Buzzer, 2637); delay(100); noTone(Buzzer);
    delay(100);
    tone(Buzzer, 2217); delay(50); noTone(Buzzer);
    delay(50);
    tone(Buzzer, 2637); delay(200); noTone(Buzzer);
    delay(400);

    int slide = 5000;
    for (int i = 0; i <= 120; i++) {
      tone(Buzzer, slide); delay(10); noTone(Buzzer);
      slide = slide - 40;
    }
    noTone(Buzzer);

    Serial.println("Now let's test the LEDs");
    digitalWrite(B_LED, HIGH);
    digitalWrite(R_LED, LOW);
    delay(500);
    digitalWrite(R_LED, HIGH);
    delay(500);

    digitalWrite(G_LED, LOW);
    delay(500);
    digitalWrite(G_LED, HIGH);
    delay(500);

    digitalWrite(B_LED, LOW);
    delay(500);
    delay(1000);
    Serial.println("Great, done!");
    delay(1000);

    //Test pyro channels
    Serial.println();
    Serial.println("To finish up here, let's test all the pyro channels");
    Serial.println();
    delay(10000);
            //PLEASE COMMENT THIS OUT BEFORE ACTUALL FLIGHT...
            Serial.println("We'll now cycle through each channel, turning each one on for 2 seconds");
            delay(1000);
            digitalWrite(Pyro1, HIGH);
            Serial.println("Pyro 1 is live!");
            delay(2000);
            digitalWrite(Pyro1, LOW);
            Serial.println("Pyro 1 is off");
            delay(1000);
            digitalWrite(Pyro2, HIGH);
            Serial.println("Pyro 2 is live!");
            delay(2000);
            digitalWrite(Pyro2, LOW);
            Serial.println("Pyro 2 is off");
            delay(1000);
            digitalWrite(Pyro3, HIGH);
            Serial.println("Pyro 3 is live!");
            delay(2000);
            digitalWrite(Pyro3, LOW);
            Serial.println("Pyro 3 is off");
            delay(1000);
            digitalWrite(Pyro4, HIGH);
            Serial.println("Pyro 4 is live!");
            delay(2000);
            digitalWrite(Pyro4, LOW);
            Serial.println("Pyro 4 is off");
            delay(1000);

    Serial.println();
    Serial.println("Done with the pyro channel testing");
    Serial.println();
    Serial.println();
    delay(500);
    digitalWrite(G_LED, LOW);
    digitalWrite(R_LED, HIGH);
    digitalWrite(B_LED, HIGH);


    //switch case
    forever(){
      if(){
        state = 2
      }
      else{
        
      }
    }
  }
  while(state == 1){

  }
  while(state == 2){

  }
  while(state == 3){

  }
  while(state == 4){

  }
  while(state == 5){

  }
  while(state == 6){
    while(1){
      digitalWrite(G_LED, HIGH);
      digitalWrite(R_LED, LOW);
      digitalWrite(B_LED, HIGH);
      Serial.println("ABORT");
      delay(500);
      digitalWrite(G_LED, HIGH);
      digitalWrite(R_LED, HIGH);
      digitalWrite(B_LED, HIGH);
      delay(500);
    }
  }
}
