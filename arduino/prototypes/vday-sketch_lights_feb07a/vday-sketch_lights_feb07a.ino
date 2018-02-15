void setup() {
  // put your setup code here, to run once:
for (int i=2; i < 12; i++) {
  pinMode(i, OUTPUT);
}
allLEDsOn();
delay(5000);
}

void allLEDsOff(void){
  for (int i = 2; i < 12; i++) {
    digitalWrite(i,LOW);
  }
}

void allLEDsOn(void){
  for (int i = 2; i < 12; i++) {
    digitalWrite(i,HIGH);
  }
}

void loop() {
  // put your main code here, to run repeatedly:


  
 for (int i =2; i < 12; i++){
    allLEDsOff();
    digitalWrite(i,HIGH);
    delay(60);
  }

  for (int i = 12; i > 2; i--){
    allLEDsOff();
    digitalWrite(i, HIGH);
    delay(60);
  }
}
