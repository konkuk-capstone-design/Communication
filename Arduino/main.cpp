#include <stdio.h>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string>

// application reads from the specified serial port and reports the collected data
int _tmain(int argc, _TCHAR* argv[])
{
	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("\\\\.\\COM8");    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected\n");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
	char outData[256] = "wd";
	int dataLength = 2;
	int readResult = 0;

	while (SP->IsConnected())
	{
		SP->WriteData(outData, dataLength);
		readResult = SP->ReadData(incomingData, dataLength);
		incomingData[readResult] = 0;

		printf("%c %c\n", incomingData[0], incomingData[1]);

		Sleep(500);
	}
	return 0;
}