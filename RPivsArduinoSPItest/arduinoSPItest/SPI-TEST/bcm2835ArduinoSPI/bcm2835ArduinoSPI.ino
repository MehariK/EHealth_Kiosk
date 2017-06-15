
// Written by Nick Gammon
// February 2011
/**
 * Send arbitrary number of bits at whatever clock rate (tested at 500 KHZ and 500 HZ).
 * This script will capture the SPI bytes, when a '\n' is recieved it will then output
 * the captured byte stream via the serial.
 */

#include <SPI.h>
#define MISO 12

char buf [100];
volatile byte pos;
volatile boolean process_it;

void setup (void)
{
  Serial.begin (115200);   // debugging

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  // get ready for an interrupt 
  pos = 0;   // buffer empty
  process_it = true;

 SPI.setClockDivider(SPI_CLOCK_DIV128);


  //SPI.SPISettings(14000000, MSBFIRST, SPI_MODE0);

  // now turn on interrupts
  SPI.attachInterrupt();

}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
//byte c = SPDR;  // grab byte from SPI Data Register
  //char *rx[] = {"\0x48", "\0x48"}; 

  uint8_t tx[] = {
      0x40, 0x21, 0x22, 0x23, 0x24,
      0x20, 
       0x4D, 0x65, 0x68, 0x61, 0x72, 0x0A 
  };

  
  Serial.println(sizeof(tx));
  // 0x57, 0x4F, 0x52, 0x4C, 0x44,0x0A 
  //SPDR = rx;
 SPI.transfer(tx, 12); 
  printf("Character = %c\n", SPDR );
  for (char ret = 0; ret < 12; ret++) {
    //if (!(ret % 6))
    //  puts("");
    //SPDR = rx[ret];
    printf("Character = %c\n", SPDR );
  }
  // add to buffer if room
  /*if (pos < sizeof buf)
    {
    buf [pos++] = c;
    Serial.println(pos);
    // example: newline means time to process buffer
    if (c == '\n')
      process_it = true;
      
    }  // end of room available
    */
}  // end of interrupt routine SPI_STC_vect

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  if (process_it)
    {
    buf [pos] = 0;  
    //Serial.println (buf);
    pos = 0;
    process_it = false;
   
    }  // end of flag set
    //Serial.println(pos );
    
}  // end of loop
