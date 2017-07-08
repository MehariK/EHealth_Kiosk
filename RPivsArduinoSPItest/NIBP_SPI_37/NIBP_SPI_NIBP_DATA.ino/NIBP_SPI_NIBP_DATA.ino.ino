
#include <SPI.h>
#include <SoftwareSerial.h>
#define MISO 12
#define CS 10


//SPI variables

volatile uint8_t sumValueIN;
volatile uint8_t valueIN[1]= {0x0F};
uint8_t junkValueOUT[1]= {0x01};
volatile uint8_t valueOUT[1] = {0xAA};
volatile uint8_t rpiThx[1]= {0x4};  
//char* BP_DATA_Location;  
 
//char val[11];
char *BP_DATA;

 char* BP_READ_VALUE;


 SoftwareSerial mySerial(8,9); //Rx, Tx

//end of SPI variables

byte cmd_request_open_comm_port[]= {
  0x02, 0x43, 0x50, 0x43, 0x30, 0x35, 0x3B
};

byte cmd_request_close_comm_port[]= {
  0x02, 0x43, 0x50, 0x43, 0x30, 0x34, 0xC6
};

int ACK_packet_length = 6;

byte ACK_packet[]= {
  0x01, 0x50, 0x43, 0x37, 0x30, 0x06
};

int NAK_packet_length = 6;
byte NAK_packet[]= {
  0x01, 0x50, 0x43, 0x37, 0x30, 0x15
};

//SoftwareSerial Serial2(8,9); //RX, TXs

char *decodeMessage( unsigned char *mes, int n );

char binToHex( char in );

void read_BP_data_after_each_measurement(char* bag, int vault_length);

int decodeTwoBytesHEX( unsigned char *p );


int issue_command_to_BPM( int com1, int com2 ){
  //Serial2.println( "check0" );
  int ack_received = 0;
  static char cmd_frame[8]= { 0x02, 0x43, 0x50, 0x43, 0x00, 0x00, 0x00 };
    int sum=0;
  cmd_frame[4] = com1;
  cmd_frame[5] = com2;
    for( int i=1; i<6; i++ ) {
      sum += cmd_frame[i];
    }
    cmd_frame[6] = sum & 0xff;

  for( int trial=0; trial<3; trial++ ) {
    Serial.write( cmd_frame, 7 ); // send command to BPM

    // wait for acknowledge from BPM
    char dataReading[100] =  {};
//Serial2.println( "check1" );
    while( Serial.available() == 0 );
//Serial2.println( "check2" );
    delay( 10 );
    int num_bytes = Serial.available();
    Serial.readBytes(dataReading,num_bytes); // read acknowledge packet
    
    // check acknowlege packet
    if( dataReading[5] == 0x06 /*ACK*/ ) {
      ack_received = 1;
      break;
    } else if( dataReading[5] != 0x15 /*NAK*/) {
      // error happened!      
    }
  }
  return( ack_received );
}


void issue_command_and_check_ack( int com1, int com2, char *success, char *failure ) {
  int code = issue_command_to_BPM( com1, com2 );
  if( code != 0 ) { //Serial2.println( success );
  } else { //Serial2.println( failure );
    }
}



boolean
receive_command_from_BPM( unsigned char *dataReading, long buf_size/*, unsigned char **payload, int *payload_length*/ ) {
    boolean success=false;
    int data_length;
    long packet_length;
    for( int trial=0; trial<3; trial++ ) {  
      int num_bytes=0;
      packet_length = 100000;

      do {
        while( Serial.available() == 0 );
        dataReading[num_bytes++] = Serial.read();
        if( num_bytes >= buf_size ) {
         // Serial2.println( "buffer overflow!" );
        }
        if( num_bytes >= 8 ) {
          data_length = 0;
          for( int i=0; i<4; i++ ) {
            data_length *= 16;
            int digit = dataReading[4+i];
            if( digit >= 'A' && digit <= 'F' ) data_length += digit - 'A' + 10;
            else if( digit >= 'a' && digit <= 'f' ) data_length += digit - 'a' + 10;
            else data_length += digit - '0';
          }
//        Serial2.println( decodeMessage( dataReading+4, 4 ) );
          packet_length = data_length + 10;
        }
/*      {
        char message[100];
        sprintf( message, "num_bytes=%d, data_legnth=%d, packet_length=%d", num_bytes, data_length, packet_length );
        Serial2.println( message );
      }*/
      }  while( num_bytes < packet_length );
      //Serial2.println( packet_length );
    
      // check CheckSum
      {
        int sum=0;
        for( int i=1; i<packet_length-1; i++ ) {
          sum += dataReading[i];
        }
        sum &= 0xff;
        if( sum == dataReading[num_bytes-1] ) { // check sum OK
          Serial.write( ACK_packet, ACK_packet_length );
         // Serial2.println( "ACK sent" );

          success = true;
          break;    // break the loop

       } else {
          Serial.write( NAK_packet, NAK_packet_length );
         // Serial2.println( "NAK sent" );        
        }
      } 
    }
       
    // print data field
    char *out = decodeMessage( dataReading+9, data_length );

    //Serial2.println(out);

   /*
    *payload = dataReading+9;
    *payload_length = data_length;
     */
  
    return( success );
}

//Start of code for reading BP measurement data only

char*
receive_BP_measurement_data_from_BPM( unsigned char *dataReading, long buf_size/*, unsigned char **payload, int *payload_length*/ ) {
    boolean success=false;
    int data_length;
    long packet_length;
    for( int trial=0; trial<3; trial++ ) {  
      int num_bytes=0;
      packet_length = 100000;

      do {
        while( Serial.available() == 0 );
        dataReading[num_bytes++] = Serial.read();
        if( num_bytes >= buf_size ) {
          //Serial2.println( "buffer overflow!" );
        }
        if( num_bytes >= 8 ) {
          data_length = 0;
          for( int i=0; i<4; i++ ) {
            data_length *= 16;
            int digit = dataReading[4+i];
            if( digit >= 'A' && digit <= 'F' ) data_length += digit - 'A' + 10;
            else if( digit >= 'a' && digit <= 'f' ) data_length += digit - 'a' + 10;
            else data_length += digit - '0';
          }
//        Serial2.println( decodeMessage( dataReading+4, 4 ) );
          packet_length = data_length + 10;
        }
/*      {
        char message[100];
        sprintf( message, "num_bytes=%d, data_legnth=%d, packet_length=%d", num_bytes, data_length, packet_length );
        Serial2.println( message );
      }*/
      }  while( num_bytes < packet_length );
     // Serial2.println( packet_length );
    
      // check CheckSum
      {
        int sum=0;
        for( int i=1; i<packet_length-1; i++ ) {
          sum += dataReading[i];
        }
        sum &= 0xff;
        if( sum == dataReading[num_bytes-1] ) { // check sum OK
          Serial.write( ACK_packet, ACK_packet_length );
         // Serial2.println( "ACK sent" );

          success = true;
          break;    // break the loop

       } else {
          Serial.write( NAK_packet, NAK_packet_length );
         // Serial2.println( "NAK sent" );        
        }
      } 
    }
       
    // print data field
   char *out = dataReading+9;
   

   //Serial2.println(out);

     //read_BP_data_after_each_measurement(out, data_length);

     //test code
                    int num_bytes=data_length;
            
            //Serial.readBytes( buf, num_bytes );
           // Serial2.print("Number of bytes: ");
           // Serial2.println(num_bytes);
           // out[num_bytes] = '\0';
            //Serial2.println( out );
              
           static char val[11];
            for( int i=0; i<11; i++ ) {
              val[i] = decodeTwoBytesHEX( out+(i*2) );
            }
            char SYS = val[0]+val[1];
            char DIA = val[1];
            char PUL = val[2];
            int YEAR = val[5]+1900;
            char MONTH = val[6];
            char DAY = val[7];
            char HOUR = val[8];
            char MIN = val[9];
            char SECOND = val[10];

            char BP_READING_FINAL[3]={SYS, DIA, PUL};
           sprintf( out, "%d %d %d %d %d %d %d %d %d %d %d", val[0], val[1], val[2], val[3], val[4], val[5], val[6], val[7], val[8], val[9], val[10] );
          mySerial.println( out );
         sprintf( out, "SYS=%d DIA=%d PUL=%d YEAR=%d MONTH=%d DAY=%d HOUR=%d MINUTES=%d", SYS, DIA, PUL, YEAR, MONTH, DAY, HOUR, MIN );
         mySerial.println( out );

     //end of test code
/*
    *payload = dataReading+9;
    *payload_length = data_length;
     */
    //Serial2.println(out);


     //terminate communication mode and return to standby mode
           // Serial.write(cmd_request_close_comm_port,7);

           mySerial.println((int)val[0] ); 
            mySerial.println( (int)val[1] ); 
        mySerial.println( val );    
        mySerial.println((int)*(val));
         mySerial.println((int)*(val+1));

      /*   for(int x = 0; x<12; x++){
                          sprintf( *(val+x), "%d");
                          mySerial.println( *(val+x));
                         }
       */                  
    return BP_READING_FINAL;
}



// end of code for reading BP measurement data

char *
decodeMessage( unsigned char *mes, int n ) {
  static char out[100];
  int i;
  for( i=0; i<n; i++ ) {
     out[i*2] = binToHex( (mes[i]>>4)&0xf );
     out[i*2+1] = binToHex( mes[i]&0xf );
  }
  out[2*i] = '\0';
  return( out );
}

int
decodeTwoBytesHEX( unsigned char *p ) {
  int val=0;
  for( int i=0; i<2; i++, p++ ) {
    val *= 16;
    if( *p >= 'A' && *p <= 'F' ) val += *p - 'A';
    else if( *p >= 'a' && *p <= 'f' ) val += *p - 'a';
    else val += *p - '0';
  }
  return( val );
}

void
read_message_after_measurement() {
  while( Serial.available()==0 );
  delay( 20 );
  int num_bytes=Serial.available();
  char buf[100];
  Serial.readBytes( buf, num_bytes );
  buf[num_bytes] = '\0';
 // Serial2.println( buf );
    
  int val[5];
  for( int i=0; i<5; i++ ) {
    val[i] = decodeTwoBytesHEX( buf+i*2 );
  }
  int SYS = val[1]+val[2];
  int DIA = val[2];
  int PUL = val[3];
  sprintf( buf, "%d %d %d %d %d", val[0], val[1], val[2], val[3], val[4] );
 // Serial2.println( buf );
  sprintf( buf, "SYS=%d DIA=%d PUL=%d", SYS, DIA, PUL );
 // Serial2.println( buf );
}

//Start of code to read BP measurement data

void
read_BP_data_after_each_measurement(char* bag, int vault_length) {
  char * vault = bag;
  //void * memcpy ( void * destination, const void * source, size_t num );
 // int x = 0;
 // x = memcpy (vault, bag,vault_length*2+1);
 /* if(x==0) 
  Serial2.println("Copy OK"); 
  else 
  Serial2.println("Copy error!");
  */
  //while( Serial.available()==0 );
  //delay( 20 );
  
  int num_bytes=vault_length;
  
  //Serial.readBytes( buf, num_bytes );
 // Serial2.print("Number of bytes: ");
  // Serial2.println(num_bytes);
  vault[num_bytes] = '\0';
  //Serial2.println( vault );
    
  int val[11];
  for( int i=0; i<11; i++ ) {
    val[i] = decodeTwoBytesHEX( vault+(i*2) );
  }
  int SYS = val[0]+val[1];
  int DIA = val[1];
  int PUL = val[2];
  int YEAR = val[5]+1900;
  int MONTH = val[6];
  int DAY = val[7];
  int HOUR = val[8];
  int MIN = val[9];
  int SECOND = val[10];
  
  //sprintf( vault, "%d %d %d %d %d %d %d %d %d %d", val[0], val[1], val[2], val[3], val[4], val[5], val[6], val[7], val[8], val[9], val[10] );
 // Serial2.println( vault );
  sprintf( vault, "SYS=%d DIA=%d PUL=%d YEAR=%d MONTH=%d DAY=%d HOUR=%d MIN=%d", SYS, DIA, PUL, YEAR, MONTH, DAY, HOUR, MIN );
 // Serial2.println( vault );
}


//end of code to read BP measurement data

#if 0
void read_device_id() {
  // read device id
  issue_command_and_check_ack( 0x31, 0x33, /* inquire device ID */
             "issue inquire device ID", "error in issueing inquire device ID" );


   boolean flag=receive_command_from_BPM( dataReading, BUF_SIZE );
  // Serial2.println( flag );
}
#endif


char
binToHex( char in ) {
  if( in > 9 ) {
    return( in-10+'a');
  } else {
    return( in+'0');
  }
}



void setup() {


  // put your setup code here, to run once:
            Serial.begin(9600,SERIAL_8N2);

             mySerial.begin(57600);

  
  //SPI Settings go here

  mySerial.println("Hello this is for test");


 //Set the clock to communicate with RPi at 4MHz
// SPI.setClockDivider(SPI_CLOCK_DIV4);

 spi_init_slave();


 

//SPI Settings ends here
  
}



void loop() {
 // put your main code here, to run repeatedly:
  /*if( Serial.available()) {
    delay( 20 );
    int num_bytes=Serial.available();
    char buf[100];
    Serial.readBytes( buf, num_bytes );
    //Serial2.println( decodeMessage( buf, num_bytes ) );
  }*/
  mySerial.println("Just entered loop");
 //valueIN[0] = spi_tranceiver_junk_send();
 SPI.transfer(valueIN,1);
         mySerial.println("valueIN[0]");
         mySerial.println(valueIN[0]);

         unsigned long start = micros();
                       
                       if(valueIN[0]==0x0A){


                              
                                
                        //read blood pressure data here
                       
                        BP_DATA = read_NIBP_Data_From_ANDNIBP();

                        mySerial.println("Following up val");
                         mySerial.println( BP_DATA );   
                         
                        // height = readHeightSensor();
                         //SPDR = BP_DATA;
                         //*valueOUT = height;
                         // spi_tranceiver(valueOUT);
                      //SPDR = readHeightSensor();
                      // SPDR = readHeightSensor();
                       // 
                        mySerial.println("Finished Measuring ");
                       // distance = readHeightSensor();

                          mySerial.println("to read data from NIBP took this minutes:");
                              unsigned long end_first = micros();
                              unsigned long delta_first = end_first - start;
                              mySerial.println(delta_first);     
                       
                      } //request by RPi to transmit the data read from the Height Sensor
                      else if(valueIN[0]==0x14){
                           //SPDR = height;
                          // *junkValueOUT = 0xFA;
                           //sendSensorReadingToRaspiCommand(valueOUT);


                           mySerial.println("to export data after delay of this minutes:");
                              unsigned long end = micros();
                              unsigned long delta = end - start;
                              mySerial.println(delta);

                
                 //sprintf( BP_DATA, "%d %d %d %d %d %d %d %d %d %d %d", BP_DATA[0], BP_DATA[1], BP_DATA[2], BP_DATA[3], BP_DATA[4], BP_DATA[5], BP_DATA[6], BP_DATA[7], BP_DATA[8], BP_DATA[9], BP_DATA[10]);
                        /* for(int x = 0; x<12; x++){
                          //sprintf( *(BP_DATA+x), "%d");
                          mySerial.println((int)*(BP_DATA+x));
                         }*/
                            SPI.transfer(BP_DATA,3);
                           
                         /*for(int j = 0; j<12; j++){
                             //spi_tranceiver((int)BP_DATA[j]);
                             SPI.transfer(BP_DATA[j],1);
                          }*/
                           //readRequestRaspiCommand(rpiThx, valueOUT);
                            //Send recorded height measurement back to RasPi
                             mySerial.println("Transmited Data");
                      }else{

                        mySerial.println("No match");
                      }
                      
                      valueIN[0] = 0x0F;
                     // int x = readHeightSensor();
                     // Serial.print("BP Measurement Data");
                     // Serial.println(BP_DATA);

        //Just exited loop    
        mySerial.println("XXXXXXXXXXXXXXited the loop");         

}


// Initialize SPI Slave Device
void spi_init_slave ()
{
    
      //*********************************************//
      //set the CS to input mode
     //DDRB &= B11111011;
     //DDRB &= ~(_BV(PB2));

     //*********************************************//
      // enable pull up on the CS
    //PORTB |= B00000100;
     PORTB |= _BV(PB2);
    
  

  DDRB = DDRB | _BV(PB4);     //MISO as OUTPUT

  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
    
}



void readRequestRaspiCommand(uint8_t * valueIN){

  //digitalWrite(CS, LOW);
  //SPI.transfer(address);

  //for(int i=0; i<numBytes; i++){
    //valueIN[i] = SPI.transfer(junkValueOUT[0]);
    //valueIN[0] = spi_tranceiver(junkValueOUT[0]);
   // valueIN[0] = spi_tranceiver_junk_send();
   // Serial.println(char(valueIN[i]));
   //  }
 
}


/*void  measureHeight(uint8_t * valueIN, uint8_t * valueOUT){

      //request for measuring height by RPi command interpreted by 
      //addition of consecutive values in a buffer to be used as a command
      if(*valueIN == 0x0A){
      //readRequestRaspiCommand(rpiThx, junkValueOUT);
      *junkValueOUT = 0x45;
       // SPDR = 0x45;
        spi_tranceiver(junkValueOUT);
        Serial.println("10");
       // distance = readHeightSensor();
      }
      //request by RPi to transmit the data read from the Height Sensor
      else if(*valueIN == 0x14) {
           //*valueOUT = 0x75;
           //sendSensorReadingToRaspiCommand(valueOUT);
           spi_tranceiver(valueOUT);
           //readRequestRaspiCommand(rpiThx, valueOUT);
            //Send recorded height measurement back to RasPi
             Serial.println("20");
      }
      *valueIN = 0xFF;
}*/



void sendSensorReadingToRaspiCommand(uint8_t * valueOUT){

  //digitalWrite(CS, LOW);
  //SPI.transfer(address);

 // for(int i=0; i<numBytes; i++){
    //valueIN[i] = SPI.transfer(valueOUT[i]);
    spi_tranceiver(valueOUT[0]);
   // Serial.println(char(valueIN[i]));
    // }
 
  //valueOUT[0] = 0x1;

}

void printRaspiCommand(int lengthOfBytes, uint8_t * commandIN){

   for(int i=0; i<lengthOfBytes; i++){
    mySerial.print(char(commandIN[i]));
  }
    mySerial.print("\n");
}

//Function to send and receive data for both master and slave
//unsigned char spi_tranceiver (unsigned char data)
uint8_t spi_tranceiver (volatile uint8_t * valueOUT)
{
   // set MISO HIGH
    //DDRB |= _BV(PB4);
    // Load data into the buffer
    SPDR = *valueOUT;
 
    //Wait until transmission complete
    while(!(SPSR & (1<<SPIF) ));
    
    //set MISO LOW
    //DDRB &= ~(_BV(PB4)); 
    
    // Return received data
    return(SPDR);  
}

uint8_t spi_tranceiver_junk_send ()
{
   // set MISO HIGH
    //DDRB |= _BV(PB4);
    // Load data into the buffer
    SPDR = 0xFF;
 
    //Wait until transmission complete
    while(!(SPSR & (1<<SPIF) ));
    
    //set MISO LOW
    //DDRB &= ~(_BV(PB4)); 
    
    // Return received data
    return(SPDR);  
}

char* read_NIBP_Data_From_ANDNIBP(){
       
             boolean flag;
            //Serial2.begin(57600);
          
                    
            Serial.write(0);
                      
            delay(100);
           // Serial.write(0x5);
          
            // request open command
            issue_command_and_check_ack( 0x30, 0x35, /* open connect1ion */ "Port opened!", "Error in open port!" );
            
            delay( 1000 );
          
            // delete measured data stored in memory after copying it
             //Serial2.println( "Now delete measured data already retrieved from memory" );
             issue_command_and_check_ack( 0x31, 0x32, /* inquire BP & pulse data */
                      "issue data delete request", "error in deleting measurement data stored in memory" );
          
          #define BUF_SIZE 500
            //listen message from BPM and print to Serial2
             unsigned char dataReading[BUF_SIZE];
            
          
            // start blood pressure measurement
            issue_command_and_check_ack( 0x34, 0x30, /* start measurement */ "Measurement started!",
                       "Error in starting measurement!" );
          
            read_message_after_measurement();
          
          
          //delay for  a  duration equivalent to 
          //(time taken while BP measurement) and ( time taken to move to communication mode)
          
           /* for( int i=0; i<20; i++ ) {
              delay( 4000 );
              if( i%10 == 0 ) Serial2.print( "+" );
              else Serial2.print( "-" );
            }
            Serial2.println( "" );
            */
          
            delay(61000);
            
            // read measured data
            //Serial2.println( "Now issue read measured data" );
            issue_command_and_check_ack( 0x31, 0x30, /* inquire BP & pulse data */
                       "issue data request", "error in issueing data request" );
          
            BP_READ_VALUE = receive_BP_measurement_data_from_BPM( dataReading, BUF_SIZE );
            //Serial2.println( flag );
          
           //terminate communication mode and return to standby mode
          //Serial.write(4);
          issue_command_and_check_ack( 0x30, 0x34, /* open connect1ion */ "Port closed!", "Error in closing port!" );
          mySerial.println("Exit");
           return (BP_READ_VALUE);
}


