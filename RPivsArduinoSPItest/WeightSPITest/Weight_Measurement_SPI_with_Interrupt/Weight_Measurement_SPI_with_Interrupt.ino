#include "HX711.h"
//The following includes are for SPI
#include <SPI.h>
#define MISO 12
#define CS 10

//The following variables immediately below are for SPI communication 

volatile uint8_t valueIN[1]= {0};
uint8_t junkValueOUT[1]= {0x01};
volatile uint8_t valueOUT[1]= {0xAA};
volatile uint8_t rpiThx[1]= {0x4};


// The following variables and constants are for weight measurement

//#define calibration_factor -26240.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define calibration_factor -18350.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define DOUT  3
#define CLK  2
volatile float weight = 0;

HX711 scale(DOUT, CLK);



void setup() {
  
Serial.begin(115200);

 //initialize SPI slave mode
 spi_init_slave();

  //Set the clock to communicate with RPi at 4MHz
  SPI.setClockDivider(SPI_CLOCK_DIV4);

  SPI.attachInterrupt();
 
 // Serial.println("HX711 scale demo");

  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();  //Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");


}

void loop() {
  SPI.transfer(valueOUT[0]);
 
  weight = scale.get_units();
  Serial.println(weight);
 // Serial.println(scale.get_scale());
}

ISR (SPI_STC_vect){
  valueIN[0]=SPDR;

                      Serial.print("valueIN[0]");
                      Serial.println(valueIN[0]);
                      
                      //SPDR = height;
                      if(valueIN[0]==0x0A){
                        
                        //convert float to binary first
                        //SPDR = weight;
                        
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



