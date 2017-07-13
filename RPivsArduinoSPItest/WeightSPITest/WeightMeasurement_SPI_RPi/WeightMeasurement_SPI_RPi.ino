
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
volatile uint8_t valueIN[1]= {0x0F};
uint8_t junkValueOUT[1]= {0x01};
byte valueOUT[10]={0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06};
volatile uint8_t rpiThx[1]= {0x4};
//up to here

//#define calibration_factor -26240.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define calibration_factor -1900.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DOUT  3
#define CLK  2

 //SoftwareSerial mySerial(8,9); //Rx, Tx

//Variables for weight measurement


int numberOfDataCollected = 0;
int countSmoothDataCollected = 0;
int maxDataCollected = 10;
int maxSmoothedDataCollected = 5;


float weightMeasured[10];
 float measuredAverageWeight;
//smoothed average height array
float smoothedAverageWeight[5];

//smoothed median height array
float smoothedMedianWeight[5];

volatile float weight = 0;

HX711 scale(DOUT, CLK);

void setup() {
   // put your setup code here, to run once:
            Serial.begin(57600);

             //mySerial.begin(57600);

  
  //SPI Settings go here

  Serial.println("Hello this is for test");


 //Set the clock to communicate with RPi at 4MHz
// SPI.setClockDivider(SPI_CLOCK_DIV4);

 spi_init_slave();


  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();	//Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");
}


void loop() {
// digitalWrite(CLK, HIGH);
//digitalWrite(SPIswitch, LOW);
   //scale.power_up();
             // for(int j =0; j < 1000; j++){}
      Serial.print("Reading: ");
       Serial.println("Just entered loop");
        //valueIN[0] = spi_tranceiver_junk_send();
        SPI.transfer(valueIN,1);
         Serial.println("valueIN[0]");
         Serial.println(valueIN[0]);
         

         unsigned long start = micros();
  //  }

   /*
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" kg"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
  */

                     
                      
                      //SPDR = height;
                      if(valueIN[0]==0x0A){

                                      for (int countDataCollected = 0; countDataCollected < maxDataCollected; countDataCollected++)
                                      {
                                       
                                        weightMeasured[countDataCollected] = scale.get_units();
                                               numberOfDataCollected =  countDataCollected;
                                                   ++numberOfDataCollected;                        
                                      }


                                        for (int countDataCollected = 0; countDataCollected < maxDataCollected; countDataCollected++){
                                          Serial.println( weightMeasured[countDataCollected]);
                                        }


                                      ///////////////////////////////////////////////////////////

                                                   if(numberOfDataCollected == maxDataCollected)
                                                        {
                                                        measuredAverageWeight = GetAverage(weightMeasured, maxDataCollected);
                                                       // float nearRealMedianWeight =  GetMedian(weightMeasured, maxDataCollected);
                                                        //float  calculatedAverageWeight = 210.00 - measuredAverageWeight;
                                                         
                                                         
                                                         //Serial.println(measuredAverageHeight); 
                                                        // printDistance(distancesMeasured, maxDataCollected);
                                                      
                                                        // printVolt(voltMeasured, maxDataCollected);
                                                        /*
                                                        Serial.println("Your Calculated Average Height is:");
                                                        Serial.println(calculatedAverageHeight);
                                                        delay (500);
                                                        Serial.println("Your Calculated Median Height is:");
                                                        Serial.println(nearRealMedianHeight);
                                                         Serial.println("****************************Next Person*******************************************");
                                                        delay(3000);
                                                        */
                                                         
                                                        }


                                     /////////////////////////////////////////////////////////////////////
                             // valueOUT[0] = measuredAverageWeight * 100;  
                             int valueToSend = 68.84 * 100; 
                           //  valueOUT[0] = 0x48;  
                                   // valueOUT = 'ABCDEF\n';

                           sprintf(valueOUT, "%X", valueToSend); 
                           //valueOUT[4] = '\0';

                                //Serial.println(valueOUT);
                            
                                   
                        Serial.println("Finished measuring weight");
                        Serial.println(valueOUT[0]);
                        Serial.println((int)valueOUT[0]);
                       // distance = readHeightSensor();
                        
                        Serial.println("to read data from bathroom scale took this # milliseconds:");
                              unsigned long end_first = micros();
                              unsigned long delta_first = end_first - start;
                              Serial.println(delta_first);     
                       
                      }
                      //request by RPi to transmit the data read from the Height Sensor
                      else if(valueIN[0]==0x14){ 

                        Serial.println("to export weight measurement data takes this # milliseconds:");
                              unsigned long end = micros();
                              unsigned long delta = end - start;
                              Serial.println(delta);

                              SPI.transfer(valueOUT,10);
                              
                           //SPDR = height;
                          // *junkValueOUT = 0xFA;
                           //sendSensorReadingToRaspiCommand(valueOUT);
                          // spi_tranceiver(junkValueOUT);
                           //readRequestRaspiCommand(rpiT[hx, valueOUT);
                            //Send recorded height measurement back to RasPi
                             Serial.println("Transmitted data");
                      }else{
                        Serial.println("No match");
                      }
                      
                      *valueIN = 0x0F;
                     // int x = readHeightSensor();
                      //Serial.print("weight");
                      //Serial.println(weight);
                //reset numberOfDataCollected
                numberOfDataCollected = 0;
            Serial.println("Value In Resetted to");
              Serial.println(valueIN[0]);    
           //Just exited loop    
        Serial.println("XXXXXXXXXXXXXXited the loop");         
           
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

//Function to send and receive data for both master and slave
//unsigned char spi_tranceiver (unsigned char data)
float spi_tranceiver (float * valueOUT)
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

float spi_tranceiver_junk_send ()
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


float GetMedian(float daArray[], int iSize) {
  // Allocate an array of the same size and sort it.
  float* dpSorted = new float[iSize];
  //    int arraySize = daArray.length();
  for (int i = 0; i < iSize; ++i) {
    dpSorted[i] = daArray[i];
  }
  for (int i = iSize - 1; i > 0; --i) {
    for (int j = 0; j < i; ++j) {
      if (dpSorted[j] > dpSorted[j + 1]) {
        float dTemp = dpSorted[j];
        dpSorted[j] = dpSorted[j + 1];
        dpSorted[j + 1] = dTemp;
      }
    }
  }

  // Middle or average of middle values in the sorted array.
  float dMedian = 0.0;
  if ((iSize % 2) == 0) {
    dMedian = (dpSorted[iSize / 2] + dpSorted[(iSize / 2) - 1]) / 2.0;
  } else {
    dMedian = dpSorted[iSize / 2];
  }
  delete [] dpSorted;
  return dMedian;
}

float GetAverage(float *daArray, int iSize)
{
  float sum = 0.0;
  for (int i = 0; i < iSize; ++i)
  {
    sum =  daArray[i] + sum;
  }
  float average = sum / (iSize - 1);
  return average;
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
  float data[3] = { 0 };
  float filler = 0x00;

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
