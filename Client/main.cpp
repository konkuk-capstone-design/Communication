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

int main()
{
	int clnt_sock;
	struct sockaddr_in serv_addr;
	char message[1024] = { 0x00, };
	printf("helloworld0\n");
	clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (clnt_sock == -1)
		printf("socket error\n");
	printf("helloworld1\n");
	char SERVER_IP[] = "192.168.200.109";
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv_addr.sin_port = htons(PORT);
	printf("helloworld2\n");
	if (connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		printf("connect error\n");
	printf("helloworld3\n");
	if (read(clnt_sock, message, sizeof(message) - 1) == -1)
		printf("read error\n");
	printf("helloworld4\n");
	printf("Message from server :%s\n", message);

	close(clnt_sock);
	return 0;
}