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