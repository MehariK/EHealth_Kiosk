int digitalInputPin = 8;
int count = 0;

void setup() {
  // put your setup code here, to run once:

pinMode(digitalInputPin, INPUT);
digitalWrite(digitalInputPin, LOW);
Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:
if (digitalRead(digitalInputPin) == 1){
  count++;
}
Serial.println(count);

}
