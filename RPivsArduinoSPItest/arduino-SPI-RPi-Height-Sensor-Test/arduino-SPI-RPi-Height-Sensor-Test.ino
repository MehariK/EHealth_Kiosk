    /*
    * Ultrasonic Sensor HC-SR04 and Arduino Tutorial
    *
    * Crated by Dejan Nedelkovski,
    * www.HowToMechatronics.com
    *
    */
 
#include <SPI.h>
#define MISO 12
#define CS 10

//pin numbers dedicated for Sensors (HC-SR04)
#define TRIGPIN 7
#define ECHOPIN 8

// defines variables
volatile long duration;
volatile uint8_t distance;
volatile uint8_t height;

volatile uint8_t sumValueIN;
volatile uint8_t valueIN[1]= {0};
uint8_t junkValueOUT[1]= {0x01};
volatile uint8_t valueOUT[1] = {0xAA};
volatile uint8_t rpiThx[1]= {0x4};



//#define CS_BIT  2

//char buf [100];
//volatile byte pos;
//volatile boolean process_it;


void setup (void)
{
  Serial.begin (115200);   // debugging


  //Set the clock to communicate with RPi at 4MHz
 SPI.setClockDivider(SPI_CLOCK_DIV4);

  spi_init_slave();

   pinMode(TRIGPIN, OUTPUT); // Sets the trigPin as an Output
   pinMode(ECHOPIN, INPUT); // Sets the echoPin as an Input

   
  // have to send on master in, *slave out*
 
 // Set MISO output, all others input 
 //DDR_SPI = (1<<DD_MISO);

  
  //the following line is inserted to troubleshoot the bus contention by multiple slave devices
  
  //pinMode(MISO, INPUT_PULLUP);
 //pinMode(CS, INPUT_PULLUP);
 

 // enable pull up on the CS
 //PORTB |= B00000100;
// PORTB |= _BV(PB2);
  
  // turn on SPI in slave mode
  //SPCR |= _BV(SPE);

//I have uncommented the following implementation to troubleshoot multi slave caused bus issue
 // SPI.beginTransaction(SPISettings(20000000, MSBFIRST, SPI_MODE0));
  

  // now turn on interrupts
  // SPI.attachInterrupt();

}  // end of setup



// main loop - wait for flag set in interrupt routine
void loop (void)
{
  //Serial.println("SPCR register content");
  // Serial.println(SPCR,BIN);
  valueIN[0] = spi_tranceiver_junk_send();
    //if (digitalRead(CS)==LOW){
  //optimized code using register values to read CS pin status
               
              //  if ((PINB & _BV(PB2)) == B00000100){
                 // delayMicroseconds(.255);
                       // if ((_BV(PB2)) == B00000100){
                
                                // SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
                              //readRequestRaspiCommand(valueIN);
                             //valueIN[0] = spi_tranceiver_junk_send();
                             // sumValueIN = valueIN;
                            //  Serial.println(valueIN[0]);
                             // Serial.println(valueIN[1]);
                            //  Serial.println(valueIN[2]);
                            //  Serial.println(sumValueIN);
                            // Serial.println(valueIN[0]);
                             // measureHeight(valueIN, valueOUT);
                             //  printRaspiCommand(12,valueIN);
                            //   SPI.endTrasnsaction();
                      // Measured Height function copied here

                      //request for measuring height by RPi command interpreted by 
                      //addition of consecutive values in a buffer to be used as a command
                      Serial.print("valueIN[0]");
                      Serial.println(valueIN[0]);
                      height = readHeightSensor();
                      //SPDR = height;
                      if(valueIN[0]==0x0A){
                         SPDR = height;
                         //*valueOUT = height;
                         // spi_tranceiver(valueOUT);
                      //SPDR = readHeightSensor();
                      // SPDR = readHeightSensor();
                       // 
                        Serial.println("10");
                       // distance = readHeightSensor();
                      }
                      //request by RPi to transmit the data read from the Height Sensor
                      else if(valueIN[0]==0x14){
                           //SPDR = height;
                          // *junkValueOUT = 0xFA;
                           //sendSensorReadingToRaspiCommand(valueOUT);
                          // spi_tranceiver(junkValueOUT);
                           //readRequestRaspiCommand(rpiThx, valueOUT);
                            //Send recorded height measurement back to RasPi
                             Serial.println("20");
                      }
                      *valueIN = 0xFF;
                     // int x = readHeightSensor();
                      Serial.print("height");
                      Serial.println(height);

      //measured height function ends here

                            
                     //   }
            //  } //if ends here
   // }  //if        
    delayMicroseconds(1000);
}  // end of loop

// Initialize SPI Slave Device
void spi_init_slave (void)
{
     //pinMode(MISO,OUTPUT); 
      //*********************************************//
      //set the CS to input mode
     //DDRB &= B11111011;
     DDRB &= ~(_BV(PB2));

     //*********************************************//
      // enable pull up on the CS
    //PORTB |= B00000100;
     PORTB |= _BV(PB2);
    
    DDRB = (1<<PB4);     //MISO as OUTPUT
    SPCR = (1<<SPE);   //Enable SPI
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


void  measureHeight(uint8_t * valueIN, uint8_t * valueOUT){

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
}



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
    Serial.print(char(commandIN[i]));
  }
    Serial.print("\n");
}

//Function to send and receive data for both master and slave
//unsigned char spi_tranceiver (unsigned char data)
uint8_t spi_tranceiver (uint8_t * valueOUT)
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
    //SPDR = 0xFF;
 
    //Wait until transmission complete
    while(!(SPSR & (1<<SPIF) ));
    
    //set MISO LOW
    //DDRB &= ~(_BV(PB4)); 
    
    // Return received data
    return(SPDR);  
}

uint8_t readHeightSensor(){

// Clears the trigPin
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHOPIN, HIGH);
    // Calculating the distance
    //distance= duration*0.034/2;
    
    return (duration*0.034/2);

    
    // Prints the distance on the Serial Monitor
    //Serial.print("Distance: ");
    //Serial.println(distance);

  
}



    


