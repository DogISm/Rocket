#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards

const int buttonRPin = 3;
const int buttonLPin = 5;
int pos = 90;
int buttonRState = 0;
int buttonLState = 0;
void setup() 
{ 
  Serial.begin(9600);
  pinMode(buttonRPin, INPUT);
  pinMode(buttonLPin, INPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
} 
 
void loop() 
{ 
  buttonRState = digitalRead(buttonRPin);
  buttonLState = digitalRead(buttonLPin);
  if (buttonRState == HIGH) {
    Serial.println("Yes");
    pos+=10;
    Serial.println(pos);
    buttonRState = 0;
    myservo.write(pos);

  }
  else if (buttonLState == HIGH) {
    Serial.println("NO");
    pos-=10;
    Serial.println(pos);
    buttonLState = 0;
    myservo.write(pos);

  }
  

  delay(2000);
  
} 
