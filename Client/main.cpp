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

int RECVKEY(int clnt_sock, char key[2]);
void SENDMAT(Mat image, int clnt_sock);
int main()
{
	int clnt_sock;
	struct sockaddr_in serv_addr;
	char message[1024] = { 0x00, };
	
	clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (clnt_sock == -1)
		printf("socket error\n");
	
	char SERVER_IP[] = "192.168.85.153";
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv_addr.sin_port = htons(PORT);
	
	if (connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		printf("connect error\n");

	if (read(clnt_sock, message, sizeof(message) - 1) == -1)
		printf("read error\n");

	printf("Message from server :%s\n", message);

	char key[2];
	Mat image;
	VideoCapture capture;
	capture.open(0);
	while (1)
	{
		capture >> image;
		SENDMAT(image, clnt_sock);

		if (RECVKEY(clnt_sock, key))
			break;
	}

	close(clnt_sock);
	return 0;
}
int RECVKEY(int clnt_sock, char key[2])
{
	key[0] = key[1] = 'q';
	read(clnt_sock, key, 2);

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
void SENDMAT(Mat image, int clnt_sock)
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
	

	//write(clnt_sock, msg, sizeof(msg));
	write(clnt_sock, rows, 4);
	write(clnt_sock, cols, 4);
	write(clnt_sock, buffer, size);
	
	delete[]buffer;
}