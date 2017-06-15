
#include <SPI.h>
#include <SoftwareSerial.h>
#include <avr/interrupt.h>
#include <avr/io.h>
 
volatile int i=0;
volatile byte buffer[200];
volatile uint8_t StrRxFlag=0;




#define MISO 12
#define CS 10
//#define CS_BIT  2

//UART Settings

#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUDRATE * 16UL))) - 1)

//UART settings ends here


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

boolean isNewLine=1;

//void readNIBP_MeasurementData(uint8_t * request_open_comm_port, uint8_t * measurement_data, uint8_t * wakeUp);

//control data format
byte ACK_Response_from_NIBP_to_PC[] = {
  0x01, 0x37, 0x30, 0x50, 0x43, 0x06
};

byte NAK_Response_from_PC_to_NIBP[] = {
  0x01, 0x50, 0x43, 0x37, 0x30, 0x15
};

byte wakeUp[] = {0x55};

byte cmd_request_open_comm_port[] = {
  0x02, 0x43, 0x50, 0x43, 0x30, 0x35, 0x3B
};

byte cmd_request_measurement_data[] = {
  0x02, 0x43, 0x50, 0x43, 0x31, 0x30, 0x37
};

byte cmd_clock_inquiry[] = { 
  0x02, 0x43, 0x50, 0x43, 0x31, 0x33, 0x3A
};

byte cmd_ack_receipt[] = {
  0x01, 0x50, 0x43, 0x37, 0x30, 0x06
};

byte cmd_request_deviceID[] = {
  0x02, 0x44, 0x50, 0x43, 0x37, 0x30, 0x3E
};

byte cmd_request_measureBP[] = {
  0x02, 0x43, 0x50, 0x43, 0x34, 0x30, 0x3A
};

byte cmd_set_clockData[] = {
  0x02, 0x44, 0x50, 0x43, 0x30, 0x30, 0x30, 0x41, 0x30, 0x36, 0x36, 0x30, 0x35, 0x31, 0x44, 0x30, 0x46, 0x31, 0x34, 0x3A
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
int countRXC = 0;
volatile byte requested_data[100] = {0};
String NIBP_sendsData = "TEST";


volatile byte measurement_data[100] = { 0 };
volatile String NIBP_measurement;


byte ack_message[10]= { 0 };
char NIBP_Ack[100] = {0};
//volatile uint8_t valueIN[12];

 
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


SoftwareSerial mySerial(8, 9); // RX, TX




String doSomething_Data(byte * cmd_request_doSomething, byte * cmd_ack_receipt, uint8_t cmdSizeBye,String NIBP_sendsData, byte* ack_message) {

  NIBP_sendsData = "Checking ";

  //initiate communication mode
  //Serial.write(10);

  //delay(100);

  //request to open communication port
  //for (int i =0; i<7; i++){
 // Serial.pirnt(cmd_request_doSomething, int(cmdSizeBye));
  Serial.write(cmd_request_doSomething,7);
 
   //USART_putstring(cmd_request_doSomething);

    if(RXC0)countRXC++;
              //Serial.flush();
             delay(150);
  //}

  //serialBufferUsed = Serial.available();
  //mySerial.println("Serial Buffer Used");
  //mySerial.println(serialBufferUsed,HEX);
  //delay(100);

           if(Serial.available()) {
            //for (int j = 0; j < serialBufferUsed; j++) {
                // ack_message[j] = Serial.read();
               //Serial.readBytes(measurement_data,32);
              //Serial.println(readSize);
           // }
        
          char c = Serial.read();
         // char c = char(USART_Receive());
                 
             NIBP_sendsData += c;
                 
        
           
         }
  
                //reset isNewLine

            if(Serial.available()) {
            //for (int j = 0; j < serialBufferUsed; j++) {
                // ack_message[j] = Serial.read();
               //Serial.readBytes(measurement_data,32);
              //Serial.println(readSize);
           // }
        
          byte c = USART_Receive();
                 
                  NIBP_sendsData += char(c);
                
        
           
         }
              
            Serial.write(cmd_ack_receipt, 7);
              //Serial.flush();
              delay(150);
              Serial.write('\n');
              
      if(NIBP_sendsData.length()>0){
       return NIBP_sendsData;
      }else{
       return "";  
      }        
            
}



void setup (void)
{
 Serial.begin (9600, SERIAL_8N2);  // debugging
   //UCSR0A = 1<<RXEN0;
  // USART_Init();
  
  mySerial.begin(57600);
 
 //enable interrupt
 // cli();
 //sei();
 //interrupts();

 //attachInterrupt(0, serialRadEvent, CHANGE);
    
}  // end of setup



// main loop - wait for flag set in interrupt routine
void loop (void)
{
   //char* connAck ;
  //requestConnectionToNIBP(cmd_request_open_comm_port, wakeUp, NIBP_Ack);
   //for(int z=0;z<20; z++){
   //mySerial.print(char(buffer));
   //}
   mySerial.println("\n");
   mySerial.println(countRXC);
  // mySerial.println(connAck);

   mySerial.print("I is now : ");
   mySerial.println(i);

  //set clock
   uint8_t cmdLength_measure = sizeof(cmd_request_measurement_data);

  //oSomething_Data(cmd_request_measureBP, cmd_ack_receipt, cmdLength_measure,NIBP_sendsData, ack_message);

  uint8_t cmdLength = sizeof(cmd_clock_inquiry);
  
  //request device ID
 //String NIBP_clock = doSomething_Data(cmd_clock_inquiry, cmd_ack_receipt,cmdLength,NIBP_sendsData, ack_message);
  //for (int x = 0; x<serialBufferUsed; x++){
  //NIBP_clock = NIBP_clock + "OK!";
  
  mySerial.println("NIBP Data");
  //mySerial.println(NIBP_clock);
  
   mySerial.println("UCSR0A");
   mySerial.println(UCSR0A,BIN);

   mySerial.println("UCSR0B");
   mySerial.println(UCSR0B,BIN);

   mySerial.println("UCSR0C");
   mySerial.println(UCSR0C,BIN);
  //}
  /*mySerial.print("Diastolic Blood Pressure =  ");
  mySerial.println(dec_DIASTOLIC,DEC);
  mySerial.print("Systolic Blood Pressure = ");
  mySerial.println(dec_SYSTOLIC, DEC);
  */
            //mySerial.println( sizeof(cmd_request_open_comm_port),DEC);
            mySerial.println("Ack");
            mySerial.println(sizeof(ack_message),DEC);
            mySerial.println(sizeof(cmd_request_open_comm_port));
    //for (int j = 0; j < 10; j++) {
        
        //mySerial.println(ack_message[j]);
          //experimental
          //mySerial.read();
      //Serial.readBytes(measurement_data,32);
      //Serial.println(readSize);
   // }

  i = 0;
   Serial.write(0x04);
   delay(400000);
}

char* requestConnectionToNIBP(byte * cmd_request_open_comm_port, byte * wakeUp, char* NIBP_Ack) {

  //initiate communication mode
    
    
    //Serial.write(0x00);
    Serial.write(0x55);
       if(RXC0)countRXC++;
  //delay(150);

  //Serial.write(10);

  while (1);



  //request to open communication port
  //for (int i =0; i<7; i++){
  uint8_t sizeCmdByte = sizeof(cmd_request_open_comm_port);
   // Serial.write(cmd_request_open_comm_port,8);
   USART_putstring(cmd_request_open_comm_port);
      if(RXC0)countRXC++;
  // Serial.flush();
  //}
          //Serial.flush();
              delay(150);
  //Serial.write('\n');
         //USART_putstring(10);
         delay(150);
         
  //serialBufferUsed = Serial.available();

          if(Serial.available()) {
            //for (int j = 0; j < serialBufferUsed; j++) {
                // ack_message[j] = Serial.read();
               //Serial.readBytes(measurement_data,32);
              //Serial.println(readSize);
           // }
        
          //char c = Serial.read();
          *NIBP_Ack = USART_Receive();
                 /*if(c=='\n'){
                   NIBP_Ack = "";
                 }else{
                  NIBP_Ack += c;
                 }*/
        
           
         }
        /* if(NIBP_Ack.length()>0){
            return NIBP_Ack;
         }else{
           return "";  
         }  */ 
         return NIBP_Ack;
}


void USART_Init(void)
{
/* Set baud rate */
//UBRR0H = (unsigned char)(baud>>8);
//UBRR0L = (unsigned char)baud;
UBRR0 = BAUD_PRESCALER;// Set the baud rate prescale rate register
/* Enable receiver and transmitter */
UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1 << RXCIE0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (1<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UPM01)|(0<<UPM00);
}

void USART_send( unsigned char data)
{
    //while the transmit buffer is not empty loop
    while(!(UCSR0A & (1<<UDRE0)));
 
    //when the buffer is empty write data to the transmitted
    UDR0 = data;
}

void USART_putstring(char* StringPtr)
// sends the characters from the string one at a time to the USART
{
    while(*StringPtr != 0x00)
    {
        USART_send(*StringPtr);
        StringPtr++;
    }
}


// Receive using low level code

unsigned char USART_Receive( void )
{
/* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)));
/* Get and return received data from buffer */
return UDR0;
}

//Interrupt Service Routine (ISR)

ISR(USART0_RX_vect)
{
    buffer[i]=UDR0;  
    i++;
    //Read USART data register
    /*if(buffer[i++]=='\r')   //check for carriage return terminator and increment buffer index
    {
        // if terminator detected
        StrRxFlag=1;        //Set String received flag
        buffer[i-1]=0x00;   //Set string terminator to 0x00
        i=0;                //Reset buffer index
    }*/
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    //char inChar = (char)Serial.read();
    // add it to the inputString:
    //inputString += inChar;
    buffer[i]=UDR0;
    i++;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    //if (inChar == '\n') {
      //stringComplete = true;
    //}
  }
}
