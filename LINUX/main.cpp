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

#include <stdint.h>
#include <errno.h>

#include <fstream>

#define PORT 4578
#define PACKET_SIZE 1024

void RECVKEY(char key[2], int size, int socket);
int fd = 0;
int main()
{
	char SERVER_TIP[30];
	printf("IP: ");
	scanf("%s", SERVER_TIP);
	while (1)
	{
		if (SERVER_TIP[fd] == '\0')
		{
			fd++;
			break;
		}
		else
			fd++;
	}
	char* SERVER_IP = (char*)calloc(fd, sizeof(char));
	for (int i = 0; i < fd; i++)
		SERVER_IP[i] = SERVER_TIP[i];

	int clnt_sock;
	struct sockaddr_in serv_addr;
	char buffer[PACKET_SIZE];
	char message[] = "Client Send";
	clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (clnt_sock == -1)
		printf("socket error\n");
	memset(&serv_addr, 0, sizeof(char) * fd);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv_addr.sin_port = htons(PORT);
	if (connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		printf("connect error\n");
	if (write(clnt_sock, message, sizeof(message)) == -1)
		printf("write error\n");
	if (read(clnt_sock, buffer, PACKET_SIZE) == -1)
		printf("read error\n");
	printf("Message from server :%s\n", buffer);

	fd = 0;
	char key[2];
	int te = 0;
	while (1)
	{
		te++;
		RECVKEY(key, 2, clnt_sock);
		printf("%d: %c %c\n",te, key[0], key[1]);
		if (key[0] == 'q')
			break;
		else if ((key[0] == 'n') && (key[1] == 'n'))
		{

		}
		else
		{

		}
	}
	close(clnt_sock);

	return 0;
}
void RECVKEY(char key[2], int size, int socket)
{
	read(socket, key, size);
}