#define LEDONE    3
#define LEDTWO    5
#define LEDTHREE  6
#define LEDFOUR   9

#define BUTTON    7

int scanTimer = 0;
bool buttonLock = false;
bool ledState = false;

void setLeds(bool onOff=true) {
  digitalWrite(LEDONE, onOff);
  digitalWrite(LEDTWO, onOff);
  digitalWrite(LEDTHREE, onOff);
  digitalWrite(LEDFOUR, onOff);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDONE, OUTPUT);
  pinMode(LEDTWO, OUTPUT);
  pinMode(LEDTHREE, OUTPUT);
  pinMode(LEDFOUR, OUTPUT);
  pinMode(BUTTON, INPUT);

  // Print to serial console
  Serial.begin(9600);
}

void loop() {

//  Serial.println(digitalRead(BUTTON));

  bool button = digitalRead(BUTTON);

  if ( button && !buttonLock ) {
    buttonLock = true;
    ledState = true;
    scanTimer = 0;
  }
  
  setLeds(ledState);

  scanTimer += 100;  

  if ( scanTimer >= 3000 && buttonLock ) {
    buttonLock = false;
    ledState = false;
    Serial.println("SCAN_COMPLETE");
  }

  delay(100);
  
}
