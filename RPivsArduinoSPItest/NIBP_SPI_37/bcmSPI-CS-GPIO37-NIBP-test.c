#include <bcm2835.h>
#include <stdio.h>

// Chip select on GPIO pin  GPIO3 
#define NIBP_CS  RPI_V2_GPIO_P1_37

int main(int argc, char **argv) {
	if (!bcm2835_init()) {
		printf("oops, could not init bcm2835\n");
		return 1;
	}
	
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_2048);    // ~ o KHz
   	bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);                      // The default
   //   bcm2835_spi_setChipSelectPolarity(nibp_CS, LOW);      // the default

	//set the nibp_CS to be an output
	//bcm2835_gpio_fsel(NIBP_CS, BCM2835_GPIO_FSEL_OUTP);

	//bcm2835_gpio_write(NIBP_CS,HIGH);

	//uint8_t mosi[10] = { 0x60, 0x00 };
	uint8_t mosi_Start_Read_NIBP_Process[1] = {0x0A};
	uint8_t miso_trigger_NIBP_Measurement[1] = {0x0};
	uint8_t miso_receive_ack_for_first_call_for_data[1] ;
	uint8_t miso_copy_NIBP_Data[3];
	uint8_t mosi_trigger_NIBP_Data[11] = {0x14,0x14,0x14};
	uint8_t mosi_first_call_for_data[1] = {0x14};

	// ///////// This is the first part of the request  to the Height Sensor/////////////
	//										   //
	////////////////////////////////////////////////////////////////////////////////////

	
	
	// The  CS GPIO will be asserted at least 3 core clock cycles 
	// before the msb of the first byte of the transefer

	
	bcm2835_gpio_write(NIBP_CS,LOW);

	bcm2835_delayMicroseconds(.777);

	//create some delay to help match the clock and the cs
	//bcm2835_delay(10);

	//for (char ret = 0; ret < 12; ret++) {
		//transfer here
		miso_trigger_NIBP_Measurement[0] = bcm2835_spi_transfer(mosi_Start_Read_NIBP_Process[0]);
	//}


	//set custom chip select to high after finishing transfer here

	//the CS line will be de-asserted no earlier than 1 core clock cycle 
	//after the trailing edge of teh final clock pulse

	bcm2835_delayMicroseconds(.244);

	bcm2835_gpio_write(NIBP_CS,HIGH);

	
//	bcm2835_spi_end();
	

	/////////// This is the second part of the call to the  Sensor for real data////////
	//										  //
	////////////////////////////////////////////////////////////////////////////////////

	//bcm2835_delayMicroseconds(3000);
	//printf("Arduino sent the following ack for the trigger: "); 
	//printf("%d",miso_trigger_NIBP_Measurement[0]);
	//printf("\n");
	if((int)miso_trigger_NIBP_Measurement[0]==0x0F){

	delay(130000);

	//for(int i=0; i< 20000; i++){}


	//second part continoues after  the delay as put in the above line

	//custom chip select set to low here
  /*
	
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4096);    // ~ o KHz
   	bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);                      // The default
   //   bcm2835_spi_setChipSelectPolarity(nibp_CS, LOW);      // the default
*/



	
	//custom chip select set to low here

	bcm2835_gpio_write(NIBP_CS,LOW);

	// The  CS GPIO will be asserted at least 3 core clock cycles 
	// before the msb of the first byte of the transefer

	bcm2835_delayMicroseconds(.777);

	//create some delay to help match the clock and the cs
	//bcm2835_delay(10);

	miso_receive_ack_for_first_call_for_data[0] = bcm2835_spi_transfer(mosi_first_call_for_data[0]);

	bcm2835_delayMicroseconds(.244);	

	bcm2835_gpio_write(NIBP_CS,HIGH);

	// this is the attempt to read the BP Measurement data
	//bcm2835_delayMicroseconds(7000);
	delay(15);



	bcm2835_gpio_write(NIBP_CS,LOW);

	bcm2835_delayMicroseconds(.777);

	//for (char ret = 0; ret < 12; ret++) {
		//transfer here

		bcm2835_spi_transfernb(mosi_trigger_NIBP_Data,miso_copy_NIBP_Data,3);
	//}


	//set custom chip select to high after finishing transfer here

	//the CS line will be de-asserted no earlier than 1 core clock cycle 
	//after the trailing edge of teh final clock pulse

	bcm2835_delayMicroseconds(.244);

	bcm2835_gpio_write(NIBP_CS,HIGH);

	//printf("Analogue level from SPI: %04x\n", miso[1] + ((miso[0] & 3) << 8));
	//for (char pri = 0; pri < 12; pri++) {
		//if (!(ret % 6))
		//	puts("");
		//printf("%.2X ", rx[ret]);
		
	sprintf(miso_copy_NIBP_Data, "%d,%d,%d", miso_copy_NIBP_Data[0],miso_copy_NIBP_Data[1],miso_copy_NIBP_Data[2]);
	printf(miso_copy_NIBP_Data);
	printf("Systolic Reading is: %d\n",miso_copy_NIBP_Data[0]);
	printf("Diastolic Reading is: %d\n",miso_copy_NIBP_Data[1]);
	printf("Pulse Reading is: %d\n", miso_copy_NIBP_Data[2]);
	//}
	}//END OF IF CHECKING FOR  miso_trigger_NIBP_Measurement[0] RESULT FOR 0X15
	
	bcm2835_spi_end();
	bcm2835_close();	
	return 0;
}
