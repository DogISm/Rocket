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

int TVCXpin = 3;
int TVCYpin = 4;
Servo TVCXservo;
Servo TVCYservo;



void setup() {
  while(!Serial){
  }
  Serial.println();Serial.println();
  Serial.println("Hi, I am your flight computor!");
  Serial.println();Serial.println();
  
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);

  digitalWrite(R_LED, HIGH);
  digitalWrite(G_LED, HIGH);
  digitalWrite(B_LED, HIGH);


  //Now Barometer
  Serial.println("First, let's see if the BMP280 Barometer is connected. Standby...");
  delay(1000);
  if (!bmp.begin()) {
    Serial.println("Could not find the BMP280 sensor");
    delay(5000);
    Serial.println("Proceeding with the rest of the startup process");
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
  Serial.println("Now we'll look for the the MPU6050 IMU. Standby...");
  delay(1000);
  Serial.println();
  Serial.println("MPU6050 Detected!");
 
  Serial.println("Great! Onward to the SD card");
  delay(1000);


  //Now the SD card
  Serial.print("Looking for an SD card. Standby...");
  if (!card.init(SPI_HALF_SPEED, SDchipSelect)) {
    Serial.println("Card not detected");
    Serial.println();
    delay(4000);
    Serial.println("We'll continue with the startup without the SD card now :(");
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
    while (1);
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
Serial.println("Buzzer test skipped");
//  Serial.println("Testing buzzer");
//  delay(500);
//  tone(Buzzer, 2000); delay(50); noTone(Buzzer); delay(75);
//  tone(Buzzer, 2000); delay(50); noTone(Buzzer); delay(200);
//  tone(Buzzer, 1000); delay(50); noTone(Buzzer); delay(75);
//  tone(Buzzer, 1000); delay(50); noTone(Buzzer); delay(400);
//
//  tone(Buzzer, 1319); delay(50); noTone(Buzzer);
//  delay(50);
//  tone(Buzzer, 1760); delay(50); noTone(Buzzer);
//  delay(50);
//  tone(Buzzer, 2217); delay(50); noTone(Buzzer);
//  delay(50);
//  tone(Buzzer, 2637); delay(100); noTone(Buzzer);
//  delay(100);
//  tone(Buzzer, 2217); delay(50); noTone(Buzzer);
//  delay(50);
//  tone(Buzzer, 2637); delay(200); noTone(Buzzer);
//  delay(400);
//  int slide = 5000;
//  for (int i = 0; i <= 120; i++) {
//    tone(Buzzer, slide); delay(10); noTone(Buzzer);
//    slide = slide - 40;
//  }
//  noTone(Buzzer);

  Serial.println("Now let's test the LEDs");
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
  digitalWrite(B_LED, HIGH);
  delay(1000);
  Serial.println("Great, done!");
  delay(1000);


  //Test pyro channels
  Serial.println();
  Serial.println("To finish up here, let's test all the pyro channels");
  Serial.println();
  pinMode(Pyro1, OUTPUT);
  pinMode(Pyro2, OUTPUT);
  pinMode(Pyro3, OUTPUT);
  pinMode(Pyro4, OUTPUT);
  digitalWrite(Pyro1, LOW);
  digitalWrite(Pyro2, LOW);
  digitalWrite(Pyro3, LOW);
  digitalWrite(Pyro4, LOW);
  delay(1000);

          Serial.println("We'll now cycle through each channel, turning each one on for 2 seconds");
          delay(1000);
          digitalWrite(Pyro1, HIGH);
          Serial.println("Pyro 1 is on!");
          delay(2000);
          digitalWrite(Pyro1, LOW);
          Serial.println("Pyro 1 is off");
          delay(1000);
          digitalWrite(Pyro2, HIGH);
          Serial.println("Pyro 2 is on!");
          delay(2000);
          digitalWrite(Pyro2, LOW);
          Serial.println("Pyro 2 is off");
          delay(1000);
          digitalWrite(Pyro3, HIGH);
          Serial.println("Pyro 3 is on!");
          delay(2000);
          digitalWrite(Pyro3, LOW);
          Serial.println("Pyro 3 is off");
          delay(1000);
          digitalWrite(Pyro4, HIGH);
          Serial.println("Pyro 4 is on!");
          delay(2000);
          digitalWrite(Pyro4, LOW);
          Serial.println("Pyro 4 is off");
          delay(1000);
  
  Serial.println();
  Serial.println("Done with the pyro channel testing");
  Serial.println();
  Serial.println();
  delay(500);
  
  Serial.println("Now time to check the TVC servos");
  delay(500);
  delay(500);
  Serial.println("First, the X servo");
  delay(500);
  TVCXservo.attach(TVCXpin);
  TVCXservo.write(90); delay(500);
  TVCXservo.write(120); delay(500);
  TVCXservo.write(90); delay(500);
  TVCXservo.write(60); delay(500);
  TVCXservo.write(90); delay(500);
  Serial.println("Now, the Y servo");
  delay(500);
  TVCYservo.attach(TVCYpin);
  TVCYservo.write(90); delay(500);
  TVCYservo.write(120); delay(500);
  TVCYservo.write(90); delay(500);
  TVCYservo.write(60); delay(500);
  TVCYservo.write(90); delay(500);
  Serial.println("All done! That concludes the startup process!");
  delay(1000);
}

void loop() {

  digitalWrite(R_LED, LOW);
  delay(100);
  digitalWrite(R_LED, HIGH);
  delay(100);

  digitalWrite(G_LED, LOW);
  delay(100);
  digitalWrite(G_LED, HIGH);
  delay(100);

  digitalWrite(B_LED, LOW);
  delay(100);
  digitalWrite(B_LED, HIGH);
  delay(100);

}
