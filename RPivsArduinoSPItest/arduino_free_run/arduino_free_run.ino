#define CS 10
#define CLK 13
#define GPIO_CS 4

void se tup() {
  // put your setup code here, to run once:

  pinMode(CLK ,INPUT);
  pinMode(CS, INPUT);
  pinMode(GPIO_CS, INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
 delay(500);
 Serial.println(SPCR);
 
}
