#include <Servo.h>

#define LEDONE    2
#define LEDTWO    3
#define LEDTHREE  4
#define LEDFOUR   5

#define BUTTON    12

#define SERVO     13

Servo scanServo;

int controlByte;

long scanPreviousMillis = 0;
long cascadePreviousMillis = 0;
long servoPreviousMillis = 0;

int scanMillis = 12500;
int cascadeMillis = 60;
int servoMillis = 15;

bool buttonLock = false;

bool scanning = false;
bool scanComplete = false;


void setLeds(bool onOff=true) {
  for ( int i = 2; i < 11; i++ ) {
    digitalWrite(i, onOff);
  }
}

int cascadeLed = 2;
bool cascadeForward = true;
void cascadeLeds() {
  setLeds(false);
  digitalWrite(cascadeLed, true);
  if ( cascadeForward ) {
    cascadeLed++;
  } else {
    cascadeLed--;
  }
//  Serial.println(cascadeLed);
  if ( 10 <= cascadeLed ) {
    cascadeForward = false;
  } else if ( 2 >= cascadeLed ) {
    cascadeForward = true;
  }
}

int servoPos = 0;
bool servoForward = true;
void servoMove() {
//  Serial.println("MOVING SERVO");
  scanServo.write(servoPos);
  if ( servoForward ) {
    servoPos++;
  } else {
    servoPos--;
  }
  if ( 180 <= servoPos ) {
    servoForward = false;
  } else if ( 0 >= servoPos ) {
    servoForward = true;
  }
}

void setup() {
  for (int i=2; i < 11; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(BUTTON, INPUT);
  scanServo.attach(SERVO);
  
  // Print to serial console
  Serial.begin(9600);

  Serial.println("Device is ready...");
}

void loop() {
  
  unsigned long currentMillis = millis();
  
  unsigned int scanDiff = currentMillis - scanPreviousMillis;
  unsigned int cascadeDiff = currentMillis - cascadePreviousMillis;
  unsigned int servoDiff = currentMillis - servoPreviousMillis;
  
//  Serial.println(digitalRead(BUTTON));

  bool button = digitalRead(BUTTON);

  if ( button && !buttonLock ) { // button being pressed
    buttonLock = true;
    scanComplete = false;
    setLeds(true);
    Serial.println("STARTUP");
  }

  if ( buttonLock ) { // button has been pressed
    
  }

  if ( 0 < Serial.available() ) {
    controlByte = Serial.read();
    Serial.print("REMOTE COMMAND RECEIVED: ");
    Serial.println(controlByte);
    if ( 49 == controlByte ) { // 1
      delay(2000);
      scanDiff = 0;
      scanPreviousMillis = currentMillis;
      scanning = true;
    }
  }

  if ( scanDiff > scanMillis ) {
//    Serial.println(scanDiff);
    if ( scanning && buttonLock ) {
      scanComplete = true;
      scanning = false;
    }
  }

  if ( cascadeDiff > cascadeMillis ) {
    cascadeDiff = 0;
    cascadePreviousMillis = currentMillis;
    if ( scanning ) {
      cascadeLeds();
    }
  }

  if ( servoDiff > servoMillis ) {
    servoDiff = 0;
    servoPreviousMillis = currentMillis;
    if ( scanning ) {
      servoMove();
    }
  }

  if ( !scanning && scanComplete ) {
    scanComplete = false;
    buttonLock = false;
    setLeds(false);
    Serial.println("SCAN_COMPLETE");
  }
}
