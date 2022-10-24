// 이 명령에서는 wiringPiSetupSys 설정의 GPIO 내보내기가 사용됩니다.
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
		digitalWrite(LED, HIGH);  // 설정
		delay(500); // ms
		digitalWrite(LED, LOW);	  // 해제
		delay(500);
	}
	return 0;
}