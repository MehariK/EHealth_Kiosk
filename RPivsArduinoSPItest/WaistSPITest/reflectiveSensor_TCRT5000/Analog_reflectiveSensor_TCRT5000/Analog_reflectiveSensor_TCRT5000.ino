//long int count2=0;
int count1=1;
int flg=0;
int tcrt;
int digSignalRead = 13;
void setup(){
pinMode(digSignalRead, INPUT);
digitalWrite(digSignalRead, HIGH);
Serial.begin(9600);
}
void loop(){
//tcrt = analogRead(A0);
tcrt = digitalRead(digSignalRead);
Serial.println("This is the digital reading");
Serial.println(tcrt);
if (tcrt==1 & flg==0){
count1++;
flg=1;
}
if (tcrt==0) flg=0;

 
Serial.println(count1);


 
}
