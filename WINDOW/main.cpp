#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <string>

#include <WinSock2.h>
#include <fstream>
#pragma comment(lib,"ws2_32")
#define PORT 4578
#define PACKET_SIZE 1024


int BLOBSIZE(std::string weight);
int SENDKEY(SOCKET hClient);
int RECVKEY(SOCKET hSocket, char key[2]);


int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  // 인자 순서대로 IPV4, 연결지향형 소켓, TCP 프로토콜 의미

	SOCKADDR_IN tListenAddr = {};
	tListenAddr.sin_family = AF_INET;    // 고정값
	tListenAddr.sin_port = htons(PORT);  // PORT 주소
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP 주소 (s_addr은 IPV4를 의미)

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));  // 소켓에 주소정보를 전달
	listen(hListen, SOMAXCONN);   // 소켓을 접속 대기 상태로 만듦, 두번째 인자는 한꺼번에 요청 가능한 최대 접속승인 수

	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	SOCKET hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize); // 인자 순서대로 소켓, accept할 클라이언트측 주소정보 구조체의 주소가 들어갈 곳, 두번째 인자로 넣은 구조체의 크기를 저장해둔 변수의 주소

	char cBuffer[PACKET_SIZE] = {};
	recv(hClient, cBuffer, PACKET_SIZE, 0);  // 대상 소켓으로부터 보내온 정보를 받아주는 역할
	printf("Recv Msg : %s\n", cBuffer);

	char cMsg[] = "Server Send";
	send(hClient, cMsg, strlen(cMsg), 0);  // 서버가 메세지를 클라이언트측에 전달

	while (1)
	{
		if (SENDKEY(hClient))
			break;
	}

	closesocket(hListen);
	closesocket(hClient);

	WSACleanup();

	return 0;
}
int BLOBSIZE(std::string weight)
{
	int iter = 0;
	int tmp = 0;
	int check = 0;
	int result = 0;

	while (1)
	{
		if (check)
		{
			if (check == 1)
				result += (weight[iter] - '0') * 100;
			else if (check == 2)
				result += (weight[iter] - '0') * 10;
			else if (check == 3)
				result += (weight[iter] - '0');
			else if (check == 4)
				break;

			check++;
		}

		if (weight[iter] == '_')
			if (++tmp == 2)
				check = 1;

		iter++;
	}

	return result;
}
int SENDKEY(SOCKET hSocket)
{
	char key[2] = { 'n','n' };

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		key[0] = key[1] = 'q';

		send(hSocket, key, 2, 0);
		return 1;
	}
	else
	{
		if (GetAsyncKeyState('W') & 0x8000)
			key[0] = 'w';
		else if (GetAsyncKeyState('S') & 0x8000)
			key[0] = 's';

		if (GetAsyncKeyState('A') & 0x8000)
			key[1] = 'a';
		else if (GetAsyncKeyState('D') & 0x8000)
			key[1] = 'd';


		send(hSocket, key, 2, 0);
		return 0;
	}
}
int RECVKEY(SOCKET hSocket, char key[2])
{
	key[0] = key[1] = 'q';
	recv(hSocket, key, 2, 0);

	if (key[0] == 'q')
		return 1;
	else
	{
		if (key[0] != 'n' || key[1] != 'n')
			printf("\n");

		if (key[0] == 'w' || key[0] == 's')
			printf("%c ", key[0]);

		if (key[1] == 'a' || key[1] == 'd')
			printf("%c", key[1]);

		return 0;
	}
}