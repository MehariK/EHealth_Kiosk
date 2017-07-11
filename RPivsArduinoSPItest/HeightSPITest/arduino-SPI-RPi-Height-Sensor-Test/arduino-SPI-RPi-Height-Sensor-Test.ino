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
volatile uint8_t valueIN[1]= {0x0F};
uint8_t junkValueOUT[1]= {0x01};
volatile uint8_t valueOUT[1] = {0xAA};
volatile uint8_t rpiThx[1]= {0x4};


//for height measurement

int countDataCollected = 0;
int countSmoothDataCollected = 0;
int maxDataCollected = 100;
int maxSmoothedDataCollected = 25;
int calculatedAverageHeight;
float heightMeasured[100];

//smoothed average height array
float smoothedAverageHeight[25];

//smoothed median height array
float smoothedMedianHeight[25];


//#define CS_BIT  2

//char buf [100];
//volatile byte pos;
//volatile boolean process_it;


void setup (void)
{
  Serial.begin (57600);   // debugging


  //Set the clock to communicate with RPi at 4MHz
 //SPI.setClockDivider(SPI_CLOCK_DIV4);

 spi_init_slave();

   pinMode(TRIGPIN, OUTPUT); // Sets the trigPin as an Output
   pinMode(ECHOPIN, INPUT); // Sets the echoPin as an Input

   Serial.println("Hello this is for test");

    
    
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
void loop ()
{
  //Serial.println("SPCR register content");
  // Serial.println(SPCR,BIN);
   Serial.println("Just entered loop");
 //valueIN[0] = spi_tranceiver_junk_send();
          SPI.transfer(valueIN,1);
         Serial.println("valueIN[0]");
         Serial.println(valueIN[0]);

                      unsigned long start = micros();               
                     
                      
                      if(valueIN[0]==0x0A){


                         for (int countDataCollected; countDataCollected < maxDataCollected; countDataCollected++)
                                      {
                                        
                                        heightMeasured[countDataCollected] = readHeightSensor();
                                                                  
                                      }



                                      ///////////////////////////////////////////////////////////

                                                   if(countDataCollected == maxDataCollected)
                                                        {
                                                        float measuredAverageHeight = GetAverage(heightMeasured, maxDataCollected);
                                                        float nearRealMedianHeight =  GetMedian(heightMeasured, maxDataCollected);
                                                        calculatedAverageHeight = 210.00 - measuredAverageHeight;
                                                         
                                                         
                                                         //Serial.println(measuredAverageHeight); 
                                                        // printDistance(distancesMeasured, maxDataCollected);
                                                      
                                                        // printVolt(voltMeasured, maxDataCollected);
                                                        /*
                                                        Serial.println("Your Calculated Average Height is:");
                                                        Serial.println(calculatedAverageHeight);
                                                        delay (500);
                                                        Serial.println("Your Calculated Median Height is:");
                                                        Serial.println(nearRealMedianHeight);
                                                         Serial.println("**********Next Person****************");
                                                        delay(3000);
                                                        */
                                                         
                                                        }


                                     /////////////////////////////////////////////////////////////////////
                         
                         valueOUT[0] = calculatedAverageHeight;
                         // spi_tranceiver(valueOUT);
                      //SPDR = readHeightSensor();
                      // SPDR = readHeightSensor();
                       // 
                        Serial.println("Just finished reading height from the bathroom scale");

                        Serial.println("to read data from height measurement sensor took  # this minutes:");
                              unsigned long end_first = micros();
                              unsigned long delta_first = end_first - start;
                              Serial.println(delta_first); 
                               *valueIN = 0x0F;  
                      }
                      //request by RPi to transmit the data read from the Height Sensor
                      else if(valueIN[0]==0x14){
                           Serial.println("to export data after delay of this minutes:");
                              unsigned long end = micros();
                              unsigned long delta = end - start;
                              Serial.println(delta);

                                     SPI.transfer(valueOUT,1);
                              
                        Serial.println("Transmitted Data");
                         *valueIN = 0xFF;
                      }else{

                        Serial.println("No match");
                      }
                      //*valueIN = 0xFF;

                      //Just exited loop    
        Serial.println("XXXXXXXXXXXXXXited the loop");    
                     
}  // end of loop

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

float GetAverage(float daArray[], int iSize)
{
  float sum = 0.0;
  for (int i = 0; i < iSize; ++i)
  {
    sum =  daArray[i] + sum;
  }
  float average = sum / (iSize - 1);
  return average;
}
    


