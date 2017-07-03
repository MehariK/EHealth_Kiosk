// cited from https://sparkfun.com/tutorial/ADXL/ADXL345_Basic.pde 
// and further commented based on ADXL345 Datasheet and SPI protocal timing

//Add the SPI library so we can communicate with the ADXL345 sensor
#include <SPI.h>

//Assign the Chip Select signal to pin 10.
int CS=10;

//This is a list of some of the registers available on the ADXL345.
//To learn more about these and the rest of the registers on the ADXL345, read the datasheet!
char POWER_CTL = 0x2D;	//Power Control Register
char DATA_FORMAT = 0x31;
char DATAX0 = 0x32;	//X-Axis Data 0
char DATAX1 = 0x33;	//X-Axis Data 1
char DATAY0 = 0x34;	//Y-Axis Data 0
char DATAY1 = 0x35;	//Y-Axis Data 1
char DATAZ0 = 0x36;	//Z-Axis Data 0
char DATAZ1 = 0x37;	//Z-Axis Data 1

//This buffer will hold values read from the ADXL345 registers.
char values[10];
//These variables will be used to hold the x,y and z axis accelerometer values.
int x,y,z;

void setup(){ 
  //Initiate an SPI communication instance.
  SPI.begin();
  //Configure the SPI connection for the ADXL345.
  SPI.setDataMode(SPI_MODE3);
  //Create a serial connection to display the data on the terminal.
  Serial.begin(9600);

  //Set up the Chip Select pin to be an output from the Arduino.
  pinMode(CS, OUTPUT);
  //Before communication starts, the Chip Select pin needs to be set high.
  digitalWrite(CS, HIGH);

  //Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
  writeRegister(DATA_FORMAT, 0x01);
  //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
  writeRegister(POWER_CTL, 0x08);  //Measurement mode  
}

void loop(){
  //Reading 6 bytes of data starting at register DATAX0 will retrieve the x,y and z acceleration values from the ADXL345.
  //The results of the read operation will get stored to the values[] buffer.
  readRegister(DATAX0, 6, values);

  //The ADXL345 gives 10-bit acceleration values, but they are stored as bytes (8-bits). To get the full value, two bytes must be combined for each axis.
  //The X value is stored in values[0] and values[1].
  x = ((int)values[1]<<8)|(int)values[0];
  //The Y value is stored in values[2] and values[3].
  y = ((int)values[3]<<8)|(int)values[2];
  //The Z value is stored in values[4] and values[5].
  z = ((int)values[5]<<8)|(int)values[4];

  //Print the results to the terminal.
  Serial.print(x, DEC);
  Serial.print(',');
  Serial.print(y, DEC);
  Serial.print(',');
  Serial.println(z, DEC);      
  delay(10); 
}

//This function will write a value to a register on the ADXL345.
 //Parameters:
//  char registerAddress - The register to write a value to
//  char value - The value to be written to the specified register.
// The bits to spedify write or read operation on slave device, and single
// or multiple register reading in reading operation, are included in the 
// registeraddress.

// As defined by protocal, the MSB of the address indicates write/read, and the following bit indicate single/multiple reading. Cause ADXL345 support Byte operation, 
// The MSB is the 8th bit, the following bit is the 7th bit. (note: if indicating multiple bits reading, ADXL345 will increasing the register address automatically to the

// next successive register like a pointer.
 
void writeRegister(char registerAddress, char value){
  //Set Chip Select pin low to signal the beginning of an SPI packet.
  digitalWrite(CS, LOW);
  //Transfer the register address over SPI.
  // All ADXL345 register address are smaller than 0100 0000, all are like 00** ****, so that any register address of ADXL345 can be used directly for register write operation
  // without any change on the MSB and its following bit, but the case for reading operation is the opposite.
  SPI.transfer(registerAddress);
  //Transfer the desired register value over SPI.
  // ignored the reture of SPI.transfer in writer operation
  SPI.transfer(value);
  //Set the Chip Select pin high to signal the end of an SPI packet.
  digitalWrite(CS, HIGH);
}

//This function will read a certain number of registers starting from a specified address and store their values in a buffer.
//Parameters:
//  char registerAddress - The register addresse to start the read sequence from.
//  int numBytes - The number of registers that should be read.
//  char * values - A pointer to a buffer where the results of the operation should be stored.
void readRegister(char registerAddress, int numBytes, char * values){
  //Since we're performing a read operation, the most significant bit (bit 7(counting starts from bit 0)) of the register address should be set high.
  char address = 0x80 | registerAddress;
  //If we're doing a multi-byte read, bit 6 (counting starts from bit 0)needs to be set high as well.
  if(numBytes > 1)address = addres s | 0x40;

  //Set the Chip select pin low to start an SPI packet.
  digitalWrite(CS, LOW);
  //Transfer the starting register address that needs to be read.
  SPI.transfer(address);
  //Continue to read registers until we've read the number specified, storing the results to the input buffer.
  //SPI is full duplex transsmission. Every signle time SPI.transfer can and can only transmit 8 bits. the input parameter will be delived to slave, and it's return will be 
  // the data that master device and get from the slave device. The data returned during master device's write operation makes no sense, hence always been ignored by not 
  // doing any assignment operation.
  for(int i=0; i<numBytes; i++){
    values[i] = SPI.transfer(0x00);
  }
  //Set the Chips Select pin high to end the SPI packet.
  digitalWrite(CS, HIGH);
}