#include <bcm2835.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if (!bcm2835_init()) {
		printf("oops, could not init bcm2835\n");
		return 1;
	}

	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_2048);    // ~ 122 KHz
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                      // The default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default

	//uint8_t mosi[10] = { 0x60, 0x00 };
	uint8_t mosi[] = {
        0x48, 0x45, 0x4C, 0x4C, 0x4F,
        0x20, 
        0x57, 0x4F, 0x52, 0x4C, 0x44,
        0x0A 
	};
	uint8_t miso[12] = { 0 };
	bcm2835_spi_transfernb(mosi, miso, 12);
	//printf("Analogue level from SPI: %04x\n", miso[1] + ((miso[0] & 3) << 8));
	for (char ret = 0; ret < 12; ret++) {
		//if (!(ret % 6))
		//	puts("");
		//printf("%.2X ", rx[ret]);
		printf("Character = %c\n", miso[ret] );
	}
	bcm2835_spi_end();
	bcm2835_close();
	return 0;
}