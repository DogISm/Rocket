const int led1 = 8;
const int led2 = 9;
const int led3 = 10;
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  while(true) {
    ledController(led1);
    ledController(led2);
    ledController(led3); 
  }
  
}
void ledController(int led) {
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
}
