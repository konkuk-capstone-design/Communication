// �� ��ɿ����� wiringPiSetupSys ������ GPIO �������Ⱑ ���˴ϴ�.
#define	LED	17
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	wiringPiSetupSys();

	pinMode(LED, OUTPUT);

	while (true)
	{
		digitalWrite(LED, HIGH);  // ����
		delay(500); // ms
		digitalWrite(LED, LOW);	  // ����
		delay(500);
	}
	return 0;
}