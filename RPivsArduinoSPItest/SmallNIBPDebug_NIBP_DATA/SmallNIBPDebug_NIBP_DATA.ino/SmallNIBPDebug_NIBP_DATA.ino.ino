#include <SoftwareSerial.h>

byte cmd_request_open_comm_port[]= {
  0x02, 0x43, 0x50, 0x43, 0x30, 0x35, 0x3B
};

int ACK_packet_length = 6;
byte ACK_packet[]= {
  0x01, 0x50, 0x43, 0x37, 0x30, 0x06
};

int NAK_packet_length = 6;
byte NAK_packet[]= {
  0x01, 0x50, 0x43, 0x37, 0x30, 0x15
};


SoftwareSerial Serial2(8,9); //RX, TXs

char *decodeMessage( unsigned char *mes, int n );

char binToHex( char in );

void read_BP_data_after_each_measurement(char* bag, int vault_length);

int decodeTwoBytesHEX( unsigned char *p );


int issue_command_to_BPM( int com1, int com2 ){
  Serial2.println( "check0" );
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
Serial2.println( "check1" );
    while( Serial.available() == 0 );
Serial2.println( "check2" );
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
  if( code != 0 ) { Serial2.println( success );
  } else { Serial2.println( failure );}
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
          Serial2.println( "buffer overflow!" );
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
      Serial2.println( packet_length );
    
      // check CheckSum
      {
        int sum=0;
        for( int i=1; i<packet_length-1; i++ ) {
          sum += dataReading[i];
        }
        sum &= 0xff;
        if( sum == dataReading[num_bytes-1] ) { // check sum OK
          Serial.write( ACK_packet, ACK_packet_length );
          Serial2.println( "ACK sent" );

          success = true;
          break;    // break the loop

       } else {
          Serial.write( NAK_packet, NAK_packet_length );
          Serial2.println( "NAK sent" );        
        }
      } 
    }
       
    // print data field
    char *out = decodeMessage( dataReading+9, data_length );

    Serial2.println(out);

   /*
    *payload = dataReading+9;
    *payload_length = data_length;
     */
  
    return( success );
}

//Start of code for reading BP measurement data only

boolean
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
          Serial2.println( "buffer overflow!" );
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
      Serial2.println( packet_length );
    
      // check CheckSum
      {
        int sum=0;
        for( int i=1; i<packet_length-1; i++ ) {
          sum += dataReading[i];
        }
        sum &= 0xff;
        if( sum == dataReading[num_bytes-1] ) { // check sum OK
          Serial.write( ACK_packet, ACK_packet_length );
          Serial2.println( "ACK sent" );

          success = true;
          break;    // break the loop

       } else {
          Serial.write( NAK_packet, NAK_packet_length );
          Serial2.println( "NAK sent" );        
        }
      } 
    }
       
    // print data field
   char *out = decodeMessage( dataReading+9, data_length );

   Serial2.println(out);

     //read_BP_data_after_each_measurement(out, data_length);

     //test code
                    int num_bytes=data_length;
            
            //Serial.readBytes( buf, num_bytes );
            Serial2.print("Number of bytes: ");
            Serial2.println(num_bytes);
           // out[num_bytes] = '\0';
            Serial2.println( out );
              
            int val[11];
            for( int i=0; i<11; i++ ) {
              val[i] = decodeTwoBytesHEX( out+(i*2) );
            }
            int SYS = val[0]+val[1];
            int DIA = val[1];
            int PUL = val[2];
            int YEAR = val[5];
            int MONTH = val[6];
            int DAY = val[7];
            int HOUR = val[8];
            int MIN = val[9];
            int SECOND = val[10];
            
            sprintf( out, "%d %d %d %d %d %d %d %d %d %d", val[0], val[1], val[2], val[3], val[4], val[5], val[6], val[7], val[8], val[9], val[10] );
            Serial2.println( out );
            sprintf( out, "SYS=%d DIA=%d PUL=%d YEAR=%d MONTH=%d DAY=%d HOUR=%d MIN=%d", SYS, DIA, PUL, YEAR, MONTH, DAY, HOUR, MIN );
            Serial2.println( out );

     //end of test code
/*
    *payload = dataReading+9;
    *payload_length = data_length;
     */
    //Serial2.println(out);
    return( success );
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
  Serial2.println( buf );
    
  int val[5];
  for( int i=0; i<5; i++ ) {
    val[i] = decodeTwoBytesHEX( buf+i*2 );
  }
  int SYS = val[1]+val[2];
  int DIA = val[2];
  int PUL = val[3];
  sprintf( buf, "%d %d %d %d %d", val[0], val[1], val[2], val[3], val[4] );
  Serial2.println( buf );
  sprintf( buf, "SYS=%d DIA=%d PUL=%d", SYS, DIA, PUL );
  Serial2.println( buf );
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
  Serial2.print("Number of bytes: ");
  Serial2.println(num_bytes);
  vault[num_bytes] = '\0';
  Serial2.println( vault );
    
  int val[11];
  for( int i=0; i<11; i++ ) {
    val[i] = decodeTwoBytesHEX( vault+(i*2) );
  }
  int SYS = val[0]+val[1];
  int DIA = val[1];
  int PUL = val[2];
  int YEAR = val[5];
  int MONTH = val[6];
  int DAY = val[7];
  int HOUR = val[8];
  int MIN = val[9];
  int SECOND = val[10];
  
  sprintf( vault, "%d %d %d %d %d %d %d %d %d %d", val[0], val[1], val[2], val[3], val[4], val[5], val[6], val[7], val[8], val[9], val[10] );
  Serial2.println( vault );
  sprintf( vault, "SYS=%d DIA=%d PUL=%d YEAR=%d MONTH=%d DAY=%d HOUR=%d MIN=%d", SYS, DIA, PUL, YEAR, MONTH, DAY, HOUR, MIN );
  Serial2.println( vault );
}


//end of code to read BP measurement data

#if 0
void read_device_id() {
  // read device id
  issue_command_and_check_ack( 0x31, 0x33, /* inquire device ID */
             "issue inquire device ID", "error in issueing inquire device ID" );


   boolean flag=receive_command_from_BPM( dataReading, BUF_SIZE );
   Serial2.println( flag );
}
#endif


void setup() {
  boolean flag;
  Serial2.begin(57600);

  // put your setup code here, to run once:
  Serial.begin(9600,SERIAL_8N2);

  Serial.write(0);

  
  delay(100);
 // Serial.write(0x5);

  // request open command
  issue_command_and_check_ack( 0x30, 0x35, /* open connect1ion */ "Port opened!", "Error in open port!" );
  
  delay( 1000 );

  // delete measured data stored in memory after copying it
   Serial2.println( "Now delete measured data already retrieved from memory" );
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
  Serial2.println( "Now issue read measured data" );
  issue_command_and_check_ack( 0x31, 0x30, /* inquire BP & pulse data */
             "issue data request", "error in issueing data request" );

  flag=receive_BP_measurement_data_from_BPM( dataReading, BUF_SIZE );
  Serial2.println( flag );
}


char
binToHex( char in ) {
  if( in > 9 ) {
    return( in-10+'a');
  } else {
    return( in+'0');
  }
}






void loop() {
 // put your main code here, to run repeatedly:
  if( Serial.available()) {
    delay( 20 );
    int num_bytes=Serial.available();
    char buf[100];
    Serial.readBytes( buf, num_bytes );
    Serial2.println( decodeMessage( buf, num_bytes ) );
  }
}





