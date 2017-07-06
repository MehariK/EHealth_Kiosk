
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

#include "HX711.h"
//The following includes are for SPI
#include <SPI.h>
#define MISO 12
#define CS 10
//This is the GPIO used to orchestrate order between RPi SPI and Aruino SPI
#define SPIswitch 8


//The folowing variables are for SPI communication to RPi
volatile uint8_t valueIN[1]= {0};
uint8_t junkValueOUT[1]= {0x01};
volatile uint8_t valueOUT[1]= {0xAA};
volatile uint8_t rpiThx[1]= {0x4};
//up to here

//#define calibration_factor -26240.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define calibration_factor -7770.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DOUT  3
#define CLK  2


volatile float weight = 0;

HX711 scale(DOUT, CLK);

void setup() {
  Serial.begin(115200);


//This is the setting for the SPI swap between RPi and Arduino
  pinMode(SPIswitch,OUTPUT);
  digitalWrite(SPIswitch, HIGH);

SPI.setClockDivider(SPI_CLOCK_DIV4);
  
 //initialize SPI slave mode
  spi_init_slave();
 // Serial.println("HX711 scale demo");

 //Set the clock to communicate with RPi at 4MHz
  

  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();	//Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");
}


void loop() {
// digitalWrite(CLK, HIGH);
digitalWrite(SPIswitch, LOW);
   //scale.power_up();
              for(int j =0; j < 1000; j++){}
Serial.print("Reading: ");
//Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
 
   weight = scale.get_units();
  digitalWrite(SPIswitch, HIGH);
 // digitalWrite(CLK, LOW);
   //Continue receiving from the RPi even if this sensor is not activated yet
    //if(digitalRead(DOUT) == HIGH){
    //for(int j =0; j < 1000; j++){}
     
      for(int j =0; j < 1000; j++){}
    valueIN[0] = spi_tranceiver_junk_send();

     for(int j =0; j < 1000; j++){}
  //  }

   /*
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" kg"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
  */

                      Serial.print("valueIN[0]");
                      Serial.println(valueIN[0]);
                      
                      //SPDR = height;
                      if(valueIN[0]==0x0A){
                        
                        //convert float to binary first
                        SPDR = weight;
                        
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
                      Serial.print("weight");
                      Serial.println(weight);
    
             for(int j =0; j < 1000; j++){}
           
}


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

//This is an attempt to implement the code for reading from the HX711 
/*
long read() {
  // wait for the chip to become ready
  while (!is_ready()) {
    // Will do nothing on Arduino but prevent resets of ESP8266 (Watchdog Issue)
    yield();
  }

  unsigned long value = 0;
  uint8_t data[3] = { 0 };
  uint8_t filler = 0x00;

  // pulse the clock pin 24 times to read the data
  data[2] = shiftIn(DOUT, CLK, MSBFIRST);
  data[1] = shiftIn(DOUT, CLK, MSBFIRST);
  data[0] = shiftIn(DOUT, CLK, MSBFIRST);

  // set the channel and the gain factor for the next reading using the clock pin
  for (unsigned int i = 0; i < 1; i++) {
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }

  // Replicate the most significant bit to pad out a 32-bit signed integer
  if (data[2] & 0x80) {
    filler = 0xFF;
  } else {
    filler = 0x00;
  }

  // Construct a 32-bit signed integer
  value = ( static_cast<unsigned long>(filler) << 24
      | static_cast<unsigned long>(data[2]) << 16
      | static_cast<unsigned long>(data[1]) << 8
      | static_cast<unsigned long>(data[0]) );

  return static_cast<long>(value);
}

long read_average(byte times) {
  long sum = 0;
  for (byte i = 0; i < times; i++) {
    sum += read();
    yield();
  }
  return sum / times;
}

double get_value(byte times) {
  return read_average(times) - 0;
}

float HX711::get_units(byte times) {
  return get_value(times) / SCALE;
}
*/
