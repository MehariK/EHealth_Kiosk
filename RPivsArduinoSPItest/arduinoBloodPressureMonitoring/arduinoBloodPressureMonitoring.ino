
#include <SPI.h>

#include <SoftwareSerial.h>

#define MISO 12
#define CS 10
//#define CS_BIT  2

//char buf [100];
//volatile byte pos;


//variabe size of buffer read in
volatile uint8_t serialBufferUsed = 0;

//measurement data
//zero based array of course. 
/*
 * measurement_data[0] --- holds command type
 * measurement_data[1] --- holds "D" fixed 0x44
 * measurement_data[2]  and
 * measurement_data[3] --- holds data sent from value
 * measurement_data[4] and
 * measurement_data[5] and
 * measurement_data[6] and 
 * measurement_data[7] --- holds data_lenght value
 * measurement_data[8] --- holds "0" fixed value
 * measurement_data[9] to
 * measurement_data[30] --- holds SYS,DIA,PUL, YEAR, MONTH, DAY, HOUR, MIN among others in th following order
 * |SYS-DIA|DIA|PUL|30+30|30+30|Year|Month|Day|Hour|Min|30+30|
 * each section is represented in two bytes i.e
 * measurement_data[9] and measurement_data[10] --- |SYS-DIA|
 * measurement_data[11] and measurement_data[12] --- | DIA|
 * measurement_data[12] and measurement_data[14] --- |PUL|
 * measurement_data[15] and measurement_data[16] --- |30 + 30|
 * measurement_data[17] and measurement_data[18] --- |30 + 30|
 * measurement_data[19} and measurement_data[20] --- |Year|
 * measurement_data[21] and measurement_data[22] --- |Month|
 * measurement_data[23] and measurement_data[24] --- |Day|
 * measurement_data[25] and measurement_data[26] --- |Hour|
 * measurement_data[27] and measurement_data[28] --- |Min|
 * measurement_data[29] and measurement_data[30] --- |30+30|
 * measurement_data[31] holds 1 byte checksum
 * 
 */
volatile uint8_t measurement_data[32] = { 0 };

//volatile uint8_t valueIN[12];

byte wakeUp[] = {0x55};

uint8_t request_open_comm_port[] = {
      0x02, 0x43, 0x50, 0x43, 0x30,
      0x35, 0x3B
};

uint8_t request_measurement_data[] = {
      0x02, 0x43, 0x50, 0x43, 0x31,
      0x30, 0x37
};

//SoftwareSerial mySerial(10, 11); // RX, TX

void setup (void)
{
  Serial.begin (9600,SERIAL_8N2);   // debugging

 
  //mySerial.begin(57600);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }*/


  //mySerial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
   

}  // end of setup



// main loop - wait for flag set in interrupt routine
void loop (void)
{

     readNIBP_MeasurementData(request_open_comm_port, measurement_data, wakeUp);
     //for (int x = 0; x<serialBufferUsed; x++){
     // Serial.println(measurement_data[7]);
     //}
  
}

void readNIBP_MeasurementData(uint8_t * request_open_comm_port, uint8_t * measurement_data, uint8_t * wakeUp){

   //initiate communication mode
   Serial.write(10);

   delay(100);

  //request to open communication port
    //for (int i =0; i<7; i++){
      Serial.write(request_open_comm_port,7);
     // Serial.flush();
    //}
    delay(100);
   serialBufferUsed = Serial.available();
    if(Serial.available()>0){
      for (int j = 0; j<32; j++){
        measurement_data[j] = Serial.read();
       //Serial.readBytes(measurement_data,32);
       //Serial.println(readSize);
      }
    }
}
    


