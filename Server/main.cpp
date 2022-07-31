//#include "stdafx.h"
//#include <conio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"ws2_32")

//#include <unistd.h>			//#include <WinSock2.h>
//#include <arpa/inet.h>		//...
#include <sys/socket.h>		//...
//#include <sys/types.h>

//#include <curses.h>	    //#include <Windows.h>

#include <stdio.h>
#include <iostream>
#include <time.h>

#include <opencv2/opencv.hpp>
#include <fstream>
#include <opencv2/imgproc.hpp>


#define PORT 4578
#define PACKET_SIZE 1024

using namespace cv;

int SENDKEY(SOCKET hClient);
int RECVKEY(SOCKET hSocket, char key[2]);
void SENDMAT(Mat image, SOCKET hClient);
Mat RECVMAT(SOCKET hSocket);
clock_t a, b, c;

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


	Mat image;
	while (1)
	{
		image = RECVMAT(hClient);
		//printf("Receiving Time: %d\n", (b - a));
		//printf("Processing Time: %d\n", (c - b));
		imshow("image", image);
		waitKey(1);

		if (SENDKEY(hClient))
			break;
	}

	closesocket(hClient);
	closesocket(hListen);

	WSACleanup();
	return 0;

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


		if (key[0] != 'n' || key[1] != 'n')
		{
			printf("%c ", key[0]);
			printf("%c\n", key[1]);
		}
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
void SENDMAT(Mat image, SOCKET hSocket)
{
	a = clock();
	char rows[4];
	char cols[4];
	int size = image.rows * image.cols * image.channels();
	strcpy_s(rows, std::to_string(image.rows).c_str());
	strcpy_s(cols, std::to_string(image.cols).c_str());
	char* buffer = new char[size];

	for (int i = 0; i < 4 - std::to_string(image.rows).size(); i++)
		rows[3 - i] = -1;
	for (int i = 0; i < 4 - std::to_string(image.cols).size(); i++)
		cols[3 - i] = -1;
	memcpy(buffer, image.data, size);
	b = clock();
	send(hSocket, rows, 4, 0);
	send(hSocket, cols, 4, 0);
	send(hSocket, buffer, size, 0);
	c = clock();
	delete[]buffer;
}
Mat RECVMAT(SOCKET hSocket)
{
	char rows[4];
	char cols[4];
	char* buffer;
	int size, recvbytes, riter, citer, row, col;
	riter = citer = 1;
	recvbytes = row = col = 0;

	recv(hSocket, rows, 4, 0);
	recv(hSocket, cols, 4, 0);
	for (int i = 0; i < 4; i++)
	{
		if (rows[3 - i] != -1)
		{
			row += (rows[3 - i] - '0') * riter;
			riter *= 10;
		}
		if (cols[3 - i] != -1)
		{
			col += (cols[3 - i] - '0') * citer;
			citer *= 10;
		}
	}

	size = row * col * 3;
	buffer = new char[size];
	a = clock();
	for (int i = 0; i < size; i += recvbytes)
	{
		if ((recvbytes = recv(hSocket, buffer + i, row * col * 3 - i, 0)) == -1)
			break;
	}
	b = clock();
	recvbytes = 0;
	Mat  image = Mat::zeros(row, col, CV_8UC3);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			image.at<cv::Vec3b>(i, j) = cv::Vec3b(buffer[recvbytes + 0], buffer[recvbytes + 1], buffer[recvbytes + 2]);
			recvbytes = recvbytes + 3;
		}
	}

	delete[]buffer;
	c = clock();
	return image;
}
