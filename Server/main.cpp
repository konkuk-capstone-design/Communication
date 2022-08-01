#pragma comment(lib,"ws2_32")

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>		
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>

#define PORT 4578
#define PACKET_SIZE 1024

using namespace cv;
//int SENDKEY(SOCKET hClient);
//int RECVKEY(SOCKET hSocket, char key[2]);
//void SENDMAT(Mat image, SOCKET hClient);
//Mat RECVMAT(SOCKET hSocket);

int main()
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		printf("socket error\n");
	printf("helloworld0\n");
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(PORT);

	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		printf("bind error\n")

	if (listen(serv_sock, 5) == -1)
		printf("listen error\n");

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1)
		printf("accept error\n");

	char msg[] = "Server Send!\n";
	write(clnt_sock, msg, sizeof(msg));

	close(clnt_sock);
	close(serv_sock);

	return 0;

}
/*
int SENDKEY(int clnt_sock)
{
	char key[2] = { 'n','n' };

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		key[0] = key[1] = 'q';

		write(clnt_sock, key, sizeof(key));
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
	
	send(hSocket, rows, 4, 0);
	send(hSocket, cols, 4, 0);
	send(hSocket, buffer, size, 0);
	
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
	
	for (int i = 0; i < size; i += recvbytes)
	{
		if ((recvbytes = recv(hSocket, buffer + i, row * col * 3 - i, 0)) == -1)
			break;
	}
	
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
	return image;
}
*/
