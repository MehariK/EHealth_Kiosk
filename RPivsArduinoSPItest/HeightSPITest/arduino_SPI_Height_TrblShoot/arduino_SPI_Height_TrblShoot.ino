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
byte valueToTx[10]={0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06};
volatile uint8_t rpiThx[1]= {0x4};
//up to here


//pin numbers dedicated for Sensors (HC-SR04)
#define TRIGPIN 7
#define ECHOPIN 8


// defines variables
volatile long duration;
volatile uint8_t distance;
volatile uint8_t height;

 //SoftwareSerial mySerial(8,9); //Rx, Tx

//Variables for weight measurement


int numberOfDataCollected = 0;
int countSmoothDataCollected = 0;
int maxDataCollected = 10;
int maxSmoothedDataCollected = 5;


float heightMeasured[10];
 float measuredAverageHeight;
//smoothed average height array
float smoothedAverageHeight[5];

//smoothed median height array
float smoothedMedianHeight[5];

//volatile float height = 0;

//HX711 scale(DOUT, CLK);

void setup() {
   // put your setup code here, to run once:
            Serial.begin(57600);

             //mySerial.begin(57600);

  
  //SPI Settings go here

  Serial.println("Hello this is for test");


 //Set the clock to communicate with RPi at 4MHz
// SPI.setClockDivider(SPI_CLOCK_DIV4);

 spi_init_slave();

  Serial.println("Heat Measuring:");
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
                                       
                                        heightMeasured[countDataCollected] = readHeightSensor();
                                               numberOfDataCollected =  countDataCollected;
                                                   ++numberOfDataCollected;                        
                                      }


                                        for (int countDataCollected = 0; countDataCollected < maxDataCollected; countDataCollected++){
                                          Serial.println( heightMeasured[countDataCollected]);
                                        }


                                      ///////////////////////////////////////////////////////////

                                                   if(numberOfDataCollected == maxDataCollected)
                                                        {
                                                        measuredAverageHeight = GetAverage(heightMeasured, maxDataCollected);
                                                       // float nearRealMedianHeight =  GetMedian(heightMeasured, maxDataCollected);
                                                        //float  calculatedAverageHeight = 210.00 - measuredAverageHeight;
                                                         
                                                         
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
                                   //  if (measuredAverageHeight > 0.00){
                                          int measuredAverageHeight_to_send = measuredAverageHeight * 100; 
                                         // int measuredAverageHeight_to_send = 68.84 * 100; 
                                          sprintf(valueOUT, "%X", measuredAverageHeight_to_send); 
                                    // }
                                     //else{
                                    //   valueOUT[0] = 0; 
                                    // }
                            // int valueToSend = 68.84 * 100; 
                           //  valueOUT[0] = 0x48;  
                                   // valueOUT = 'ABCDEF\n';

                           //sprintf(valueToTx, "%X", valueOUT); 
                           //valueOUT[4] = '\0';

                               //Serial.println(valueOUT);
                            
                                   
                        Serial.println("Finished measuring Height");
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

                        Serial.println("to export Height measurement data takes this # milliseconds:");
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
                      //Serial.print("Height");
                      //Serial.println(Height);
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

