
 #include <unistd.h>
 #include <termios.h>
 #include <errno.h>
 #include <stdio.h>
 #include <fcntl.h> 
 #include <string.h>

char  cmdPC_Disconnect[6] = {0x02, 0x06, 0x08, 0x03, 0xC2, 0x62};

char  cmdPC_SoftwareVersion[9] = {0x02, 0x09, 0x00, 0x05, 0x0D, 0x02, 0x03, 0xDA, 0x71};

int fd;

void creat_ultraMini_Connection(){


struct termios options;


 const char *device = "/dev/ttyUSB0";
 fd = open(device,O_RDWR);
 if(fd== -1){
	printf("failed to open port\n");
 }

 //
 // Get the current configuration of the serial interface
 //
 if(tcgetattr(fd, &options) < 0) {  
   printf("Failed to stablish serial connection on this port\n");
 }

 // force raw mode
 //
 // Input flags - Turn off input processing
 // Some of the flags are not defined in here they are just here for reference
 // convert break to null byte, no CR to NL translation,
 // no NL to CR translation, don't mark parity errors or breaks
 // no input parity check, don't strip high bit off,
 // no XON/XOFF software flow control
 //
 options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);


 //
 // Output flags - Turn off output processing
 //
 // no CR to NL translation, no NL to CR-NL translation,
 // no NL to CR translation, no column 0 CR suppression,
 // no Ctrl-D suppression, no fill characters, no case mapping,
 // no local output processing
 //
 // config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
 //                     ONOCR | ONOEOT| OFILL | OLCUC | OPOST);
 // there is no local output processing

 options.c_oflag &= ~OPOST;


 //
 // No line processing
 //
 // echo off, echo newline off, canonical mode off, 
 // extended input processing off, signal chars off
 //

 options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

 //
 // Turn off character processing
 //
 // clear current char size mask, no parity checking,
 // no output processing, force 8 bit input
 //

 options.c_cflag &= ~(CSIZE | PARENB);
 // local, enable receiver, 8-bit data

        options.c_cflag |= (CLOCAL | CREAD | CS8);


 //
 // One input byte is enough to return from read()
 // Inter-character timer off
 //
 //config.c_cc[VMIN]  = 1;
 // config.c_cc[VTIME] = 0;

 //
 // Communication speed (simple version, using the predefined
 // constants)
 //

 cfsetispeed(&options, B9600);
 cfsetospeed(&options, B9600);
     

 //
 // Finally, apply the configuration
 //
 //if(tcsetattr(fd, TCSAFLUSH, &options) < 0) { ... error handling ... }
int res = tcsetattr(fd, TCSANOW, &options);

	if (0 != res) {

		//ERROR("Cannot configure device parameters (%s)\n", strerror(errno));
                printf("Cannot configure device parameters");
	}


}



int main(){


creat_ultraMini_Connection();

char byte[6]={0};
write(fd,&cmdPC_Disconnect, 6);
    ssize_t size = read(fd, &byte, 6);
    printf("Read byte %c\n", byte);

write(fd,&cmdPC_SoftwareVersion,9);

return 0;
}
