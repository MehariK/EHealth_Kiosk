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
long duration;
uint8_t distance;

volatile uint8_t sumValueIN;
volatile uint8_t valueIN[1];
volatile uint8_t junkValueOUT[]= {0x01};
volatile uint8_t valueOUT[] = {0x08};

inline void  measureHeight(){

      //request for measuring height by RPi command interpreted by 
      //addition of consecutive values in a buffer to be used as a command
      if(sumValueIN == 10){
        Serial.println(sumValueIN);
       // distance = readHeightSensor();
      }

      //request by RPi to transmit the data read from the Height Sensor
      if(sumValueIN == 0x14) {
           valueOUT[0] = 0x45;
           sendSensorReadingToRaspiCommand(valueOUT[0]);
            //Send recorded height measurement back to RasPi
              Serial.println("Sending");
      }
}

//#define CS_BIT  2

//char buf [100];
//volatile byte pos;
//volatile boolean process_it;


void setup (void)
{
  Serial.begin (115200);   // debugging

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
  //if (digitalRead (CS) == LOW){
  //optimized code using register values to read CS pin status
               
              //  if ((PINB & _BV(PB2)) == B00000100){
                 // delayMicroseconds(.255);
                       // if ((_BV(PB2)) == B00000100){
                
                                // SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
                              readRequestRaspiCommand(valueIN[0], junkValueOUT[0]);
                              sumValueIN = valueIN[0];
                            //  Serial.println(valueIN[0]);
                             // Serial.println(valueIN[1]);
                            //  Serial.println(valueIN[2]);
                            //  Serial.println(sumValueIN);
                              measureHeight();
                             //  printRaspiCommand(12,valueIN);
                            //   SPI.endTrasnsaction();
                     //   }
            //  } //if ends here
              
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

void readRequestRaspiCommand(uint8_t * valueIN, uint8_t * junkValueOUT){

  //digitalWrite(CS, LOW);
  //SPI.transfer(address);

  //for(int i=0; i<numBytes; i++){
    //valueIN[i] = SPI.transfer(junkValueOUT[0]);
    valueIN[0] = spi_tranceiver(junkValueOUT[0]);
   // Serial.println(char(valueIN[i]));
   //  }
 
}

void sendSensorReadingToRaspiCommand(uint8_t * valueOUT){

  //digitalWrite(CS, LOW);
  //SPI.transfer(address);

 // for(int i=0; i<numBytes; i++){
    //valueIN[i] = SPI.transfer(valueOUT[i]);
    spi_tranceiver(valueOUT[0]);
   // Serial.println(char(valueIN[i]));
    // }
 
  valueOUT[0] = 0x1;

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
    
    return duration*0.034/2;

    
    // Prints the distance on the Serial Monitor
    //Serial.print("Distance: ");
    //Serial.println(distance);

  
}



    

