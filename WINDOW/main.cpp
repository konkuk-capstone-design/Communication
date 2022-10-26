#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <string>

#include <WinSock2.h>
#include <fstream>
#pragma comment(lib,"ws2_32")
#define PORT 4578
#define PACKET_SIZE 1024


void SENDKEY(SOCKET hClient, char* key, int size);
void RECVKEY(SOCKET hClient, char* key, int size);
int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  // ���� ������� IPV4, ���������� ����, TCP �������� �ǹ�

	SOCKADDR_IN tListenAddr = {};
	tListenAddr.sin_family = AF_INET;    // ������
	tListenAddr.sin_port = htons(PORT);  // PORT �ּ�
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP �ּ� (s_addr�� IPV4�� �ǹ�)

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));  // ���Ͽ� �ּ������� ����
	listen(hListen, SOMAXCONN);   // ������ ���� ��� ���·� ����, �ι�° ���ڴ� �Ѳ����� ��û ������ �ִ� ���ӽ��� ��

	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	SOCKET hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize); // ���� ������� ����, accept�� Ŭ���̾�Ʈ�� �ּ����� ����ü�� �ּҰ� �� ��, �ι�° ���ڷ� ���� ����ü�� ũ�⸦ �����ص� ������ �ּ�

	char cBuffer[PACKET_SIZE] = {};
	recv(hClient, cBuffer, PACKET_SIZE, 0);  // ��� �������κ��� ������ ������ �޾��ִ� ����
	printf("Recv Msg : %s\n", cBuffer);

	char cMsg[] = "Server Send";
	send(hClient, cMsg, strlen(cMsg), 0);  // ������ �޼����� Ŭ���̾�Ʈ���� ����

	char key[2];
	while (1)
	{
		key[0] = key[1] = 'n';
		SENDKEY(hClient, key, 2);

	}

	closesocket(hListen);
	closesocket(hClient);

	WSACleanup();

	return 0;
}
void SENDKEY(SOCKET hClient, char* key, int size)
{
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		key[0] = key[1] = 'q';

		send(hClient, key, 2, 0);
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

		send(hClient, key, size, 0);
	}
	fflush(stdin);
}
void RECVKEY(SOCKET hClient, char* key, int size)
{
	recv(hClient, key, size, 0);
}