
/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This example demonstrates basic scale output. See the calibration sketch to get the calibration_factor for your
 specific load cell setup.
 
 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 
 The HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
 based load cell which should allow a user to measure everything from a few grams to tens of tons.

 Arduino pin 2 -> HX711 CLK
 3 -> DAT
 5V -> VCC
 GND -> GND
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 
*/

#include <PinChangeInt.h>
#include <eHealth.h>

//The following includes are for SPI

#include <SPI.h>
#define MISO 12
#define CS 10
//This is the GPIO used to orchestrate order between RPi SPI and Aruino SPI
//#define SPIswitch 8

//! It stores the  beats per minute value.
volatile int BPM;
    
    //! It stores blood oxigen saturation value.
volatile int SPO2;


//The folowing variables are for SPI communication to RPi
volatile uint8_t valueIN[1]= {0};
uint8_t junkValueOUT[1]= {0x01};
volatile uint8_t valueOUT[1]= {0xAA};
volatile uint8_t rpiThx[1]= {0x4};
//up to here


void setup() {
  Serial.begin(115200);

SPI.setClockDivider(SPI_CLOCK_DIV4);
//This is the setting for the SPI swap between RPi and Arduino
 
initPulsioximeter();
 PCintPort::attachInterrupt(4, readPulsioximeter, RISING);


  
 //initialize SPI slave mode
  spi_init_slave();
 // Serial.println("HX711 scale demo");

 //Set the clock to communicate with RPi at 4MHz
  

   Serial.println("Readings:");
}


void loop() {
// digitalWrite(CLK, HIGH);
//digitalWrite(SPIswitch, LOW);
   //scale.power_up();
             // for(int j =0; j < 1000; j++){}

//Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
 
//readPulsioximeter();
  
 // digitalWrite(CLK, LOW);
   //Continue receiving from the RPi even if this sensor is not activated yet
    //if(digitalRead(DOUT) == HIGH){
    //for(int j =0; j < 1000; j++){}
     
  //    for(int j =0; j < 1000; j++){}
   // valueIN[0] = spi_tranceiver_junk_send();

   //  for(int j =0; j < 1000; j++){}
  //  }

   /*
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" kg"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
  */

                    //  Serial.print("valueIN[0]");
                     // Serial.println(valueIN[0]);
                      
                      //SPDR = height;
                      if(valueIN[0]==0x0A){
                        
                        //convert float to binary first
                       // SPDR = weight;
                    
                    //    Serial.println("10");
                       // distance = readHeightSensor();
                      }
                      //request by RPi to transmit the data read from the Height Sensor
                      else if(valueIN[0]==0x14){ 
                           
                       //      Serial.println("20");
                      }
                    //  *valueIN = 0xFF;
                     // int x = readHeightSensor();
                      Serial.print("SPO2");
                      Serial.println(SPO2);
                      Serial.print("Pulse");
                      Serial.println(BPM);
                      
           delay(500);
            // for(int j =0; j < 1000; j++){}
           
}//end loop


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

void initPulsioximeter(void) 
  {
    // Configuring digital pins like INPUTS
    pinMode(11, INPUT);   pinMode(10, INPUT);
    pinMode(9, INPUT);   pinMode(8, INPUT);
    pinMode( 7, INPUT);   pinMode( 6, INPUT);
    pinMode( 5, INPUT);   pinMode(4, INPUT);
    // attach a PinChange Interrupt to our pin on the rising edge
  }

  //!******************************************************************************
  //!   Name: getOxygenSaturation()                     *
  //!   Description: Returns the oxygen saturation in blood in percent.     *
  //!   Param : void                              *
  //!   Returns: int with the oxygen saturation value             *
  //!   Example: int SPO2 = eHealth.getOxygenSaturation();            *
  //!******************************************************************************

  int getOxygenSaturation(void)
  {
    return SPO2;
  }

  //!******************************************************************************
  //!   Name: getBPM()                            *
  //!   Description: Returns the heart beats per minute.            *
  //!   Param : void                              *
  //!   Returns: int with the beats per minute                  *
  //!   Example: int BPM = eHealth.getBPM();                  *
  //!******************************************************************************

  int getBPM(void)
  {
    return BPM;
  }

  //!******************************************************************************
  //!   Name: readPulsioximeter()                       *
  //!   Description: It reads a value from pulsioximeter sensor.        *
  //!   Param : void                              *
  //!   Returns: void                             *
  //!   Example: readPulsioximeter();                     *
  //!******************************************************************************
 
  void readPulsioximeter(void)
  {
    uint8_t digito[41];

    uint8_t A = 0;
    uint8_t B = 0;
    uint8_t C = 0;
    uint8_t D = 0;
    uint8_t E = 0;
    uint8_t F = 0;
    uint8_t G = 0;

    for (int i = 0; i<41 ; i++) { // read all the led's of the module
      A = !digitalRead(11);
      B = !digitalRead(10);
      C = !digitalRead(9);
      D = !digitalRead(8);
      E = !digitalRead(7);
      F = !digitalRead(6);
      G = !digitalRead(5);
      
      digito[i] = segToNumber(A, B, C ,D ,E, F,G);    
      delayMicroseconds(300); //300 microseconds      
      
    }

      SPO2 = 10 * digito[25] + digito[20];
      BPM  = 100 * digito[19] + 10 * digito[2] + digito[0];
    
              //Serial.print("Pulse");
                     // Serial.println(BPM);           
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


/*******************************************************************************************************/

  //! Converts from 7 segments to number.

  uint8_t segToNumber(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E, uint8_t F, uint8_t G )
  {
    if ((A == 1) && (B == 1) && (C == 1) && (D == 0) && (E == 1) && (F == 1) && (G == 1)) {
      return 0;
     
    } else if ((A == 0) && (B == 1) && (C == 0) && (D == 0) && (E == 1) && (F == 0) && (G == 0)) {  
      return 1;
    
    } else if ((A == 1) && (B == 1) && (C == 0) && (D == 1) && (E == 0) && (F == 1) && (G == 1)) { 
      return 2;
    
    } else if ((A == 1) && (B == 1) && (C == 0) && (D == 1) && (E == 1) && (F == 0) && (G == 1)) { 
      return 3;
    
    } else if ((A == 0) && (B == 1) && (C == 1) && (D == 1) && (E == 1) && (F == 0) && (G == 0)) { 
      return 4;
    
    } else if ((A == 1) && (B == 0) && (C == 1) && (D == 1) && (E == 1) && (F == 0) && (G == 1)) { 
      return 5;
    
    } else if ((A == 1) && (B == 0) && (C == 1) && (D == 1) && (E == 1) && (F == 1) && (G == 1)) { 
      return 6;
    
    } else if ((A == 1) && (B == 1) && (C == 0) && (D == 0) && (E == 1) && (F == 0) && (G == 0)) {
      return 7;  
    
    } else if ((A == 1) && (B == 1) && (C == 1) && (D == 1) && (E == 1) && (F == 1) && (G == 1)) { 
      return 8;
    
    } else if ((A == 1) && (B == 1) && (C == 1) && (D == 1) && (E == 1) && (F == 0) && (G == 1)) { 
      return 9;
      
    } else  {
      return 0;
    }
  }

/*******************************************************************************************************/

