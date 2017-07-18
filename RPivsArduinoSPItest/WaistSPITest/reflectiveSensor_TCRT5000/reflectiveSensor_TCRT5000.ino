int digitalInputPin = 4;
int count = 0;

void setup() {
  // put your setup code here, to run once:



pinMode(digitalInputPin, INPUT);
digitalWrite(digitalInputPin, HIGH);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
if (digitalRead(digitalInputPin) == 1){
  count++;
}
Serial.println(count);

}
