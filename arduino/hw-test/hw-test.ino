#define LEDONE    2
#define LEDTWO    3
#define LEDTHREE  4
#define LEDFOUR   5

#define BUTTON    12

int controlByte;

long scanPreviousMillis = 0;
long cascadePreviousMillis = 0;

int scanMillis = 5000;
int cascadeMillis = 60;

int cascadeLed = 2;

bool buttonLock = false;

bool scanning = false;
bool scanComplete = false;

bool cascadeForward = true;

void setLeds(bool onOff=true) {
  for ( int i = 2; i < 12; i++ ) {
    digitalWrite(i, onOff);
  }
}

void cascadeLeds() {
  setLeds(false);
  digitalWrite(cascadeLed, true);
  if ( cascadeForward ) {
    cascadeLed++;
  } else {
    cascadeLed--;
  }
  Serial.println(cascadeLed);
  if ( 11 <= cascadeLed ) {
    cascadeForward = false;
  } else if ( 2 >= cascadeLed ) {
    cascadeForward = true;
  }
}

void setup() {
  for (int i=2; i < 12; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(BUTTON, INPUT);
  
  // Print to serial console
  Serial.begin(9600);
}

void loop() {
  
  unsigned long currentMillis = millis();
  
  unsigned int scanDiff = currentMillis - scanPreviousMillis;
  unsigned int cascadeDiff = currentMillis - cascadePreviousMillis;
  
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
      scanDiff = 0;
      scanPreviousMillis = currentMillis;
      scanning = true;
    }
  }

  if ( scanning && ( scanDiff > scanMillis ) && buttonLock ) {
    scanComplete = true;
    scanning = false;
  } 

  if ( cascadeDiff > cascadeMillis ) {
    cascadeDiff = 0;
    cascadePreviousMillis = currentMillis;
    if ( scanning ) {
      cascadeLeds();
    }
  }

  if ( !scanning && scanComplete ) {
    scanComplete = false;
    buttonLock = false;
    setLeds(false);
    Serial.println("SCAN_COMPLETE");
  }
}
