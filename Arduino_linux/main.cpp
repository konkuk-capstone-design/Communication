// just that the Arduino IDE doesnt compile these files.
//include system librarys
#include <stdio.h> //for printf
#include <stdint.h> //uint8_t definitions
#include <stdlib.h> //for exit(int);
#include <string.h> //for errno
#include <errno.h> //error output

//wiring Pi
#include <wiringPi.h>
#include <wiringSerial.h>

// Find Serial device on Raspberry with ~ls /dev/tty*
// ARDUINO_UNO "/dev/ttyACM0"
// FTDI_PROGRAMMER "/dev/ttyUSB0"
// HARDWARE_UART "/dev/ttyAMA0"
char device[] = "/dev/ttyACM0";
// filedescriptor
int fd;
unsigned long baud = 9600;
unsigned long time = 0;

//prototypes
int main(void);
void loop(void);
void setup(void);

void setup() 
{
    printf("%s \n", "Raspberry Startup!");
    fflush(stdout);

    //get filedescriptor
    if ((fd = serialOpen(device, baud)) < 0) 
    {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        exit(1); //error
    }
    //setup GPIO in wiringPi mode
    if (wiringPiSetup() == -1) 
    {
        fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
        exit(1); //error
    }

}
int flag = 0;
void loop()
{
    // Pong every 3 seconds
    if (millis() - time <= 3000)
    {
        if (flag == 0)
        {
            serialPutchar(fd, 'w');
            flag = 1;
        }
        else
        {
            serialPutchar(fd, 's');
            flag = 0;
        }
        time = millis();
    }

    // read signal
    if (serialDataAvail(fd)) 
    {
        char newChar = serialGetchar(fd);
        printf("%c", newChar);
        fflush(stdout);
    }

}
int main() {
    setup();
    while (1) 
        loop();
    return 0;
}
