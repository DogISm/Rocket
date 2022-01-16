/* Planning ---------------------------------------------------
States:
- 1 Ground Idle
- 2 Passive assent
- 3 1st burnout
- 4 Active assent
- 5 Appogee
- 6 Balistic desent
- 7 Drouge desent
- 8 Main desent
- 9 Touchdown
- 10 Abort
Planning --------------------------------------------------- */


#include <Arduino.h>
#include <mpu6050_tockn.h>
#include <wire.h>
#include <servo.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define OUTPUT_READABLE_ACCELGYRO
#define OUTPUT_READABLE_REALACCEL

//Declaration
Servo xtvc;
Servo ytvc;

//Declare functions
void setup();
void loop();
void flight();
void PIDx();
void PIDy();

int state = 1; //system state var

MPU6050 mpu6050(Wire);
Adafruit_BMP280 bmp;

Sd2Card card;
SdVolume voloume;
SdFile root;

//Pyros
int pyro1 = 20;
int pyro2 = 21;
int pyro3 = 22;
int pyro4 = 23;

//Variables
int16_t ax, ay, az;
int16_t gx, gy, gz;
double accelx;
double accely;
double accelz;

//Output Variables
const int SDChipSelect = 0;
int R_led = 9;
int B_led = 6;
int G_led = 2;
int Buzzer = 10;

int xtvc_pin = 3;
int ytvc_pin = 4;

//PID variables
float kp = 0;
float ki = 0;
float kd = 0;
float setpoint = 0;

//Time variables
float elapsedMS;
float MS;
float previouseMS;
int period = 50; //In milliseconds

//xPID variables
float xpid_p;
float xpid_i;
float xpid_d;
float xpid_total;
float xangle;
float xangle_error;
float xangle_previous_error;

//yPID variables
float ypid_p;
float ypid_i;
float ypid_d;
float ypid_total;
float yangle;
float yangle_error;
float yangle_previous_error;

void setup() {
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  
  MS = millis();

  while(!Serial) {
    Serial.println("No serial conection");
  }

  Serial.println();
  Serial.println();
  Serial.println("Hi, I am ready for doody, haha funny doody");
  Serial.println();
  Serial.println();

  pinMode(R_led, OUTPUT);
  pinMode(G_led, OUTPUT);
  pinMode(B_led, OUTPUT);

  digitalWrite(R_led, HIGH);
  digitalWrite(G_led, HIGH);
  digitalWrite(B_led, HIGH);

  //Baramater test
  Serial.println("Lets checkup on the BMP280's continuity");
  delay(1000);

  if(!bmp.begin()) {
    Serial.println("No BMP280 detected :(");
    delay(5000);
    Serial.println("Ill continue the start process now");
  }

  else {
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,      //normal operating mode
                    Adafruit_BMP280::SAMPLING_X2,      //temp oversampling
                    Adafruit_BMP280::SAMPLING_X16,     //presure oversampling
                    Adafruit_BMP280::FILTER_X16,       //filtering
                    Adafruit_BMP280::STANDBY_MS_500);  //standby time

    Serial.println("Sensor found! Here is some data from it.");
    Serial.println();
    Serial.println(F("Temp = "));
    Serial.println(bmp.readTemperature());
    Serial.println(" *C");

    Serial.println();
    Serial.println(F("Presure = "));
    Serial.println(bmp.readPressure());
    Serial.println(" Pa");

    Serial.println();
    Serial.println(F("Approx altitude = "));
    Serial.println(bmp.readAltitude(1013.25));
    Serial.println(" m");
    Serial.println();

    delay(1000);
  }

  //MPU 6050 test
  Serial.println("Looking for MPU6050");
  delay(1000);
  Serial.println("MPU6050 found!");
  Serial.println();

  //SD card test
  Serial.println("Now I am going to test the SD card.");
  delay(1000);
  Serial.println("Searching for an SD card...");
  if(!card.init(SPI_HALF_SPEED, SDChipSelect)) {
    Serial.println("SD card not found... thats a bummer... lol");
    Serial.println();
  }
  else {
    Serial.println("Found the SD card! Here is some info.");
    delay(1000);
    Serial.println();
    Serial.println("Card type:    ");
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
    if(!voloume.init(card)) {
      Serial.println("Couldnt find a fat16 or fat32 partition. \nMake sure that the card is formated");
      while(1);
    }
    uint32_t volumesize;
    Serial.println("Vol is of type FAT");
    Serial.println("Vol size (kb)");
    Serial.println(volumesize);
    delay(1000);
  }
  //Buzzer test
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

  //LED Test
  Serial.println("Now I will test the LED's");

  digitalWrite(R_led, LOW);
  delay(500);
  digitalWrite(R_led, HIGH);
  delay(500);

  digitalWrite(G_led, LOW);
  delay(500);
  digitalWrite(G_led, HIGH);
  delay(500);

  digitalWrite(B_led, LOW);
  delay(500);
  digitalWrite(B_led, HIGH);
  delay(1000);
  Serial.println("Great, done!");
  delay(1000);

  //Pyro test
  Serial.println();
  Serial.println("We are almost done now lets just check the pyro channels. \nJust in case anything is pluged in to them here is 10 seconds to stop the system and coment out the pyro test");
  Serial.println();
  delay(10000);

  pinMode(pyro1, OUTPUT);
  pinMode(pyro2, OUTPUT);
  pinMode(pyro3, OUTPUT);
  pinMode(pyro4, OUTPUT);
  digitalWrite(pyro1, LOW);
  digitalWrite(pyro2, LOW);
  digitalWrite(pyro3, LOW);
  digitalWrite(pyro4, LOW);
  delay(1000);
  
  Serial.println("We'll now test each pyro for 2 seconds");

  delay(1000);
  digitalWrite(pyro1, HIGH);
  Serial.println("Pyro 1 is on!");
  delay(2000);
  digitalWrite(pyro1, LOW);
  Serial.println("Pyro 1 is off!");

  delay(1000);
  digitalWrite(pyro2, HIGH);
  Serial.println("Pyro 2 is on!");
  delay(2000);
  digitalWrite(pyro2, LOW);
  Serial.println("Pyro 2 is off!");

  delay(1000);
  digitalWrite(pyro3, HIGH);
  Serial.println("Pyro 3 is on!");
  delay(2000);
  digitalWrite(pyro3, LOW);
  Serial.println("Pyro 3 is off!");

  delay(1000);
  digitalWrite(pyro4, HIGH);
  Serial.println("Pyro 4 is on!");
  delay(2000);
  digitalWrite(pyro4, LOW);
  Serial.println("Pyro 4 is off!");

  Serial.println("Pyro test done!");
  Serial.println();
  delay(500);

  //Servo test
  Serial.println("Time to test the TVC servos");
  delay(500);

  Serial.println("First the X");
  delay(500);
  xtvc.attach(xtvc_pin);
  xtvc.write(90);
  delay(500);
  xtvc.write(120);
  delay(500);
  xtvc.write(90);
  delay(500);
  xtvc.write(60);
  delay(500);
  xtvc.write(90);
  delay(500);

  Serial.println("Now the Y");
  delay(500);
  ytvc.attach(ytvc_pin);
  ytvc.write(90);
  delay(500);
  ytvc.write(120);
  delay(500);
  ytvc.write(90);
  delay(500);
  ytvc.write(60);
  delay(500);
  ytvc.write(90);
  delay(500);

  Serial.println("Great done!!");
  flight();
}

void loop() {
	//if rocket angle > ±30 degreeezzz from 0
	//state = 10
}

void flight(){
	while (1==1) {


		if(state == 1) { // Ground Idle
      mpu6050.update();
			if (mpu6050.getAccY()  < -1.6) { //-2.5 Launch -1.6 Test
        Serial.println("Launch Detected");
        digitalWrite(B_led, LOW);
        state = 2;
      }
		}



		if(state == 2) { //Passive assent
			mpu6050.update();
			if (mpu6050.getAccY() > 0.1) {
        Serial.println("Burnout detected");
        digitalWrite(B_led, LOW);
        state = 3;
      }
		}


		if(state == 3) { //1st burnout
			delay(2000);//Time delay untill 2nd engine ignite (pyro1)
      digitalWrite(pyro1, HIGH);
      delay(2000);
      digitalWrite(pyro1, LOW);
      digitalWrite(B_led, HIGH);
      digitalWrite(G_led, LOW);
      state = 4;
		}


		if(state == 4) { //Active assent
			PIDx();//TVCx PID run
			PIDy();//TVCy PID run
			if (mpu6050.getAccY() > 0.1) {
        Serial.println("2nd Burnout detected");
        state = 5;
      }
		}


		if(state == 5) { //Main burnout/Appogee
			state = 6;
		}


		if(state == 6) { //Balistic desent
			delay(3000);
			state = 7;
		}


		if(state == 7) { //drouge desent
      digitalWrite(pyro2, HIGH);
      delay(2000);
      digitalWrite(pyro2, LOW);
			//Wait untill x meters from launch height then switch states to 8
		}


		if(state == 8) { //main desent
			digitalWrite(pyro3, HIGH);
      digitalWrite(pyro4, HIGH);
			delay(2000);
			digitalWrite(pyro3, LOW);
      digitalWrite(pyro4, LOW);
			if (mpu6050.getAccY() < -9.75) {
        Serial.println("Touchdown Detected");
        state = 9;
      }
		}


		if(state == 9) { //touchdown
			while(state == 9){
				digitalWrite(B_led, LOW);
				tone(Buzzer, 2000);
				delay(3000);
				digitalWrite(B_led, HIGH);
				tone(Buzzer, 2000);
				delay(3000);
			}
		}


		if(state == 10) { //abort
			//check if rocket is above 5m
			//if rocket > 5m
				//deploy pyro 2,3,4
				//Disable TVC if active and lock pyro 1 (in void loop "digitalWrite(pyro1, LOW)") so it cant ignite the 2nd engine
			//else
				//lock pyro1
				//disable TVC
		}
	}
}



void PIDx(){//pid x
	//PID X
	//run pid calculations
	//total pid
	//map to pwm
	//write to servos
}

void PIDy(){//pid y
	//PID Y
	//run pid calculations
	//total pid
	//map to pwm
	//write to servos
}
