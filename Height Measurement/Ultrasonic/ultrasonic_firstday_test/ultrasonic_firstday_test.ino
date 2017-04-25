#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
int cmmd1[]={0x22,0x00,0x00,0x22};//low byte stored in the sensor for the distance threshold.
int i;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello world?");
  
}

void loop() { // run over and over
  A_Mode_Setup();
  if (mySerial.available()) {
    Serial.println("There is some data in the chip!");
    mySerial.write(mySerial.read());
    
  }
 /* if (Serial.available()) {
    mySerial.write(Serial.read());
  }
  */
}


void A_Mode_Setup(){ 
  //write the data into the URM37 EEPROM
  for (i=0;i<4;i++)
    mySerial.write(cmmd1[i]);                             
  delay(200);
}
