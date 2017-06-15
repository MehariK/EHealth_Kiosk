
 
#include <SPI.h>
#define MISO 12
#define CS 10

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
   
  // have to send on master in, *slave out*
  pinMode(MISO,OUTPUT);
 // Set MISO output, all others input 
 //DDR_SPI = (1<<DD_MISO);

  
  //the following line is inserted to troubleshoot the bus contention by multiple slave devices
  
  //pinMode(MISO, INPUT_PULLUP);
 pinMode(CS, INPUT_PULLUP);
  
  // turn on SPI in slave mode
  //SPCR |= _BV(SPE);

 //SPI.setClockDivider(SPI_CLOCK_DIV128);

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
  if (digitalRead (CS) == LOW){
    // SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  readWriteRaspiCommand(12, valueIN, valueOUT);

   //printRaspiCommand(12,valueIN);
//   SPI.endTrasnsaction();
  } 
}  // end of loop


// Initialize SPI Slave Device
void spi_init_slave (void)
{
   // DDRB = (1<<6);     //MISO as OUTPUT
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
    // Load data into the buffer
    SPDR = data;
 
    //Wait until transmission complete
    while(!(SPSR & (1<<SPIF) ));
 
    // Return received data
    return(SPDR);
}



