// Written by Nick Gammon
// February 2011


#include <SPI.h>
#define MISO 12


//char buf [100];
//volatile byte pos;
//volatile boolean process_it;

volatile uint8_t valueIN[12];
uint8_t valueOUT[] = {
      0x37, 0x38, 0x39, 0x30, 0x2E,
      0x20, 
       0x31, 0x32, 0x33, 0x34, 0x35, 0x36
};

void setup (void)
{
  Serial.begin (115200);   // debugging

   spi_init_slave();

        //set the CS to input mode
       //DDRB &= B11111011;
       DDRB &= ~(_BV(PB2));

  SPI.usingInterrupt(255);

  // now turn on interrupts
  SPI.attachInterrupt();
  

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{

        readWriteRaspiCommand(12, valueIN, valueOUT);
}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  
}  // end of loop

// Initialize SPI Slave Device
void spi_init_slave (void)
{
     //pinMode(MISO,OUTPUT);
    
    DDRB = (1<<PB4);    //MISO as OUTPUT
    SPCR = (1<<SPE);   //Enable SPI
}

void readWriteRaspiCommand(int numBytes, uint8_t * valueIN, uint8_t * valueOUT){

  //digitalWrite(CS, LOW);
  //SPI.transfer(address);

  for(int i=0; i<numBytes; i++){
    //valueIN[i] = SPI.transfer(valueOUT[i]);
    valueIN[i] = spi_tranceiver(valueOUT[i]);
   // Serial.println(char(valueIN[i]));
     }
 // digitalWrite(CS, HIGH);

}

void printRaspiCommand(int lengthOfBytes, uint8_t * commandIN){

   for(int i=0; i<lengthOfBytes; i++){
    Serial.print(char(commandIN[i]));
  }
    Serial.print("\n");
}

//Function to send and receive data for both master and slave
//unsigned char spi_tranceiver (unsigned char data)
uint8_t spi_tranceiver (unsigned char data)
{
   // set MISO HIGH
    //DDRB |= _BV(PB4);
    // Load data into the buffer
    SPDR = data;
 
    //Wait until transmission complete
    while(!(SPSR & (1<<SPIF) ));
    
    //set MISO LOW
    //DDRB &= ~(_BV(PB4)); 
    
    // Return received data
    return(SPDR);
    
}
    
