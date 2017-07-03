int PinTest = 8;

void setup() {
  // put your setup code here, to run once:
pinMode(PinTest,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

digitalWrite(PinTest, HIGH);
//delay(1000);
digitalWrite(PinTest, LOW);
//delay(1000);

}
