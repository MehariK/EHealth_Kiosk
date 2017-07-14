#include <SoftwareSerial.h>

//copied from MySignal for reference only
/*
 * #define S0PIN   4
 * #define S1PIN   5
 * #define S2PIN   6
 * #define ENABLE  7
 */
#define txPIN 0
#define rxPIN 1
SoftwareSerial mySerial(8,9); //Rx, Tx
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  mySerial.begin(57600);
  mySerial.println("Hello this is for test");

  pinMode(txPIN,OUTPUT);

  //copied from MYSignal.h
  //this is enable Sensor UART
 digitalWrite(txPIN, HIGH);
  delay(20);
 digitalWrite(txPIN, LOW);

delay(10);
//Serial.write(0x55);

  //Serial.begin(115200);
  while(Serial.read()>=0);

  //digitalWrite(S0PIN, LOW);
  //digitalWrite(S1PIN, HIGH);
  //digitalWrite(S2PIN, LOW);
  delay(10);
  digitalWrite(txPIN, LOW);

  delay(30);

  digitalWrite(txPIN, HIGH);
 //disable sensor UART
 /*
  * digitalWrite(S0PIN, HIGH);
  * digitalWrite(S1PIN, HIGH);
  * digitalWrite(S2PIN, HIGH);
  * digitalWrite(ENABLE, HIGH);
  * ;
  */
  

}

void loop() {
  // put your main code here, to run repeatedly:
//copied from MYSignal from the following function

//int MySignalClass::getPulsioximeter(uint8_t format)

  uint8_t sensorValueBPM;
  uint8_t sensorValueO2;
  uint8_t byte_read[6];

 while(!(Serial.available() > 0))

  if (Serial.available() > 4)
  {
              // Me quedo aqui hasta que detecte el byte 1
              while (byte_read[1] < 128)
              {
                byte_read[1] = Serial.read();
              }
              delay(20); // No bajar
          
              for (int i = 2; i < 6; i++)
              {
                byte_read[i] = Serial.read();
                delay(20);  // No bajar
              }
          
              //Comprobacion de si el dato es valido
              byte_read[1] &= 0b00110000;
              if (byte_read[1] != 0)
              {
                //Dato no valido
                return 2;
              }
          
              //Segunda comprobacion de si el dato es valido
              byte_read[2] &= 0b10000000;
              if (byte_read[2] != 0)
              {
                //Dato no valido
                return 2;
              }
          
              //byte 2
              //byte 3 -> PULSE HIGH
              //byte 4 -> PULSE LOW
              //byte 5 -> SPO2
          
              byte_read[3] &= 0b01000000;
              byte_read[4] &= 0b01111111;
          
              if (byte_read[3] == 0) {
                sensorValueBPM = byte_read[4];
              }
          
              if (byte_read[3] == 0b01000000) {
                sensorValueBPM = byte_read[4] + 0b10000000;
              }
          
              sensorValueO2 = byte_read[5];
          
          
              if ((sensorValueBPM >= 30) && (sensorValueBPM <= 200) )
              {
                  mySerial.println("Pulse:");
                  mySerial.println(sensorValueBPM);
                  
                //return sensorValueBPM;
              }
              else if ((sensorValueO2 >= 75) && (sensorValueO2 <= 100))
              {
                  mySerial.println("SP02:");
                  mySerial.println(sensorValueO2);
          
                //return sensorValueO2;
              }
              else
              {
                mySerial.println("Reading is not correct");
              }



  }
  else
  {
    mySerial.println("There is no serial buffer coming");
    int  byte_corrupted[10]= { 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4};
            for (int i = 0; i < 10; i++)
              {
                byte_read[i] = Serial.read();
                mySerial.println(byte_read[i]);
              }
  }


  
 
}

  


