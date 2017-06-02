
#include <SPI.h>
#include <String.h>

#include <SoftwareSerial.h>

#define MISO 12
#define CS 10
//#define CS_BIT  2

//char buf [100];
//volatile byte pos;


//variabe size of buffer read in
volatile uint8_t serialBufferUsed = 0;

/*
Switch(ASCII_STRING){
  
          //48  
      case:30
          return 48;
      case:31
          return 49;
      case:32
          return 50;
      case:33
          return 51;
      case:34
          retrun 52;
      case:35
          return 53;
      case:36
          return 54;
      case:37
          return 55;
      case:38
          return 56;
      case:39 
          return 57;   
49  
31
50  
32
51  33
52  34
53  35
54  36
55  37
56  38
57  39


}

*/



#define SX 0x02
#define C 0x43
#define ACK 0x06
#define NAK 0x15
#define PC uint16_t (0x50<<8)|0x43
#define NIBP 0x37|0x30
#define End_of_Transmission_Request 0x30|0x34
#define Open_Communication_Port 0x30|0x35
#define Inquire_BP_and_Pulse_Data 0x31|0x30
#define Inquire_Present_Event 0x31|0x31
#define Clear_All_BP_and_Pulse_Data 0x31|0x32
#define Inquire_Time_and_Date 0x31|0x33
#define Start_Blood_Pressure_Measuring 0x34|0x30
#define Enquire_Device_ID 0x37|0x30

//void readNIBP_MeasurementData(uint8_t * request_open_comm_port, uint8_t * measurement_data, uint8_t * wakeUp);

//control data format
uint8_t ACK_Response_from_NIBP_to_PC[] = {
  0x01, 0x37, 0x30, 0x50, 0x43,
  0x06
};

uint8_t NAK_Response_from_PC_to_NIBP[] = {
  0x01, 0x50, 0x43, 0x37, 0x30,
  0x15
};

byte wakeUp[] = {0x55};

uint8_t cmd_request_open_comm_port[] = {
  0x02, 0x43, 0x50, 0x43, 0x30,
  0x35, 0x3B
};

uint8_t cmd_request_measurement_data[] = {
  0x02, 0x43, 0x50, 0x43, 0x31,
  0x30, 0x37
};

/*measurement_data[30] --- holds SYS,DIA,PUL, YEAR, MONTH, DAY, HOUR, MIN among others in th following order
  |SYS-DIA|DIA|PUL|30+30|30+30|Year|Month|Day|Hour|Min|30+30| */

 


     

  /* measurement_data[11] and measurement_data[12] --- | DIA|
   measurement_data[12] and measurement_data[14] --- |PUL|
   measurement_data[15] and measurement_data[16] --- |30 + 30|
   measurement_data[17] and measurement_data[18] --- |30 + 30|
   measurement_data[19} and measurement_data[20] --- |Year|
   measurement_data[21] and measurement_data[22] --- |Month|
   measurement_data[23] and measurement_data[24] --- |Day|
   measurement_data[25] and measurement_data[26] --- |Hour|
   measurement_data[27] and measurement_data[28] --- |Min|
   measurement_data[29] and measurement_data[30] --- |30+30|
   */


//measurement data
//zero based array of course.
/*
   measurement_data[0] --- holds command type
   measurement_data[1] --- holds "D" fixed 0x44
   measurement_data[2]  and
   measurement_data[3] --- holds data sent from value
   measurement_data[4] and
   measurement_data[5] and
   measurement_data[6] and
   measurement_data[7] --- holds data_lenght value
   measurement_data[8] --- holds "0" fixed value
   measurement_data[9] to
   measurement_data[30] --- holds SYS,DIA,PUL, YEAR, MONTH, DAY, HOUR, MIN among others in th following order
   |SYS-DIA|DIA|PUL|30+30|30+30|Year|Month|Day|Hour|Min|30+30|
   each section is represented in two bytes i.e
   measurement_data[9] and measurement_data[10] --- |SYS-DIA|
   measurement_data[11] and measurement_data[12] --- | DIA|
   measurement_data[12] and measurement_data[14] --- |PUL|
   measurement_data[15] and measurement_data[16] --- |30 + 30|
   measurement_data[17] and measurement_data[18] --- |30 + 30|
   measurement_data[19} and measurement_data[20] --- |Year|
   measurement_data[21] and measurement_data[22] --- |Month|
   measurement_data[23] and measurement_data[24] --- |Day|
   measurement_data[25] and measurement_data[26] --- |Hour|
   measurement_data[27] and measurement_data[28] --- |Min|
   measurement_data[29] and measurement_data[30] --- |30+30|
   measurement_data[31] holds 1 byte checksum

*/
volatile uint8_t measurement_data[32] = { 0 };
volatile uint8_t ack_message[6]= { 0 };
//volatile uint8_t valueIN[12];



//SoftwareSerial mySerial(10, 11); // RX, TX

void setup (void)
{
  Serial.begin (9600, SERIAL_8N2);  // debugging
  //mySerial.begin(57600);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
    }*/

  //mySerial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port

   uint8_t requested_data[32] = {0};
  String  hex_SYSTOLIC_DIASTOLIC = (String(int(requested_data[9])*1)) + HexValLSB(int(requested_data[10]));
  String  hex_DIASTOLIC = (String(int(requested_data[11])*1)) + HexValLSB(int(requested_data[12]));
  String  hex_PULSE = (String(int(requested_data[13])* 1)) + HexValLSB(int(requested_data[14]));
  String  hex_YEAR = (String(int(requested_data[19]) * 1)) + HexValLSB(int(requested_data[20]));
  String  hex_MONTH = (String(int(requested_data[21])* 1)) + HexValLSB(int(requested_data[22]));
  String  hex_DAY = (String(int(requested_data[23])* 1)) + HexValLSB(int(requested_data[24]));
  String  hex_HOUR = (String(int(requested_data[25]) * 1)) + HexValLSB(int(requested_data[26]));
  String  hex_MINUTES = (String(int(requested_data[27]) * 1)) + HexValLSB(int(requested_data[28]));


  // set the data rate for the SoftwareSerial port

   
  char  dec_SYSTOLIC_DIASTOLIC = ((int(requested_data[9])*16)) + (char(requested_data[10]));
  char  dec_DIASTOLIC = ((char(requested_data[11])*16)) + (char(requested_data[12]));
  char  dec_PULSE = ((char(requested_data[13])* 16)) + (char(requested_data[14]));
  char  dec_YEAR = ((char(requested_data[19]) * 16)) + (char(requested_data[20]));
  char  dec_MONTH = ((char(requested_data[21])* 16)) + (char(requested_data[22]));
  char  dec_DAY = ((char(requested_data[23])* 16)) + (char(requested_data[24]));
  char  dec_HOUR = ((char(requested_data[25]) * 16)) + (char(requested_data[26]));
  char  dec_MINUTES = ((char(requested_data[27]) * 16)) + (char(requested_data[28]));
  char  dec_SYSTOLIC = dec_SYSTOLIC_DIASTOLIC + dec_DIASTOLIC;

}  // end of setup



// main loop - wait for flag set in interrupt routine
void loop (void)
{

  readNIBP_MeasurementData(cmd_request_open_comm_port, measurement_data, wakeUp);
  //for (int x = 0; x<serialBufferUsed; x++){
  // Serial.println(measurement_data[7]);
  //}

}

void readNIBP_MeasurementData(uint8_t * cmd_request_open_comm_port, uint8_t * measurement_data, uint8_t * wakeUp) {

  //initiate communication mode
  Serial.write(10);

  delay(100);

  //request to open communication port
  //for (int i =0; i<7; i++){
  Serial.write(cmd_request_open_comm_port, 7);
  // Serial.flush();
  //}
  delay(100);

  serialBufferUsed = Serial.available();

  if (Serial.available() > 0) {
    for (int j = 0; j < serialBufferUsed; j++) {
         ack_message[j] = Serial.read();

      //Serial.readBytes(measurement_data,32);
      //Serial.println(readSize);
    }
  }

  if (Serial.available() > 0) {
    for (int j = 0; j < serialBufferUsed; j++) {
      measurement_data[j] = Serial.read();

      //Serial.readBytes(measurement_data,32);
      //Serial.println(readSize);
    }
  }
}


/**
   Calculate the CCITT-CRC16 of the supplied frame.
*/
static uint16_t calculate_crc(uint16_t initial_crc, const unsigned char *buffer, unsigned int length)
{
  uint16_t crc = initial_crc;
  if (buffer != NULL)
  {
    for (unsigned int index = 0; index < length; index++)
    {
      crc = (uint16_t)((unsigned char)(crc >> 8) | (uint16_t)(crc << 8));
      crc ^= buffer [index];
      crc ^= (unsigned char)(crc & 0xff) >> 4;
      crc ^= (uint16_t)((uint16_t)(crc << 8) << 4);
      crc ^= (uint16_t)((uint16_t)((crc & 0xff) << 4) << 1);
    }
  }
  return (crc);
}

char *strdup_asciify(const unsigned char *p, unsigned int len)
{
  char *s, *ret;

  s = malloc(len + 1);
  if (NULL == s)
    return NULL;

  ret = s;

  for (int i=0; i<len; i++) {
    if (isprint(p[i]))
      *s++ = p[i];
    else
      *s++ = '.';
  }

  *s = '\0';

  return ret;
}

char *xstrdup_asciify(const unsigned char *p, unsigned int len)
{
  char *s = strdup_asciify(p, len);
  if (NULL == s)
//    fatal("Out of memory");
  return s;
}

char *strdup_hexdump(const unsigned char *p, unsigned int len)
{
  const char lookup[] = "0123456789abcdef";
  char *s, *ret;

  /* For every 4 bytes in len we will require 9 characters to display it.
   * We also need a terminating byte and to account for rounding errors.
   */
  s = malloc(((len * 9) / 4) + 2);
  if (NULL == s)
    return NULL;

  ret = s;

  for (int i=0; i<len; i++) {
    if (i && 0 == (i % 4))
      *s++ = ' ';

    *s++ = lookup[p[i] >> 4];
    *s++ = lookup[p[i] & 0x0f];
  }

  *s = '\0';

  return ret;
}

char *xstrdup_hexdump(const unsigned char *p, unsigned int len)
{
  char *s = strdup_hexdump(p, len);
  if (NULL == s)
//    fatal("Out of memory");
  return s;
}


String HexValLSB(int x){
  switch (x){
      case 0:
        return String('0');
      case 1:
        return String('1');
      case 2:
        return String('2');
      case 3:
        return String('3');
      case 4:
        return String('4');
      case 5:
        return String('5');
      case 6:
        return String('6');
      case 7:
        return String('7');
      case 8:
        return String('8');
      case 9:
        return String('9');
      case 10:
        return String('A');
      case 11:
        return String('B');
      case 12:
        return String('C');
      case 13:
        return String('D');
      case 14:
        return String('E');
      case 15:
        return String('F');
  } 

} 

