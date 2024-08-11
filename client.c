#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "mmyl/mmyl.h"

#define SERVER_PORT 8888
#define SERVER_HOST "192.168.1.103"
#define BUF_SIZE 1024

int main(int argc, char *argv[]){
	int sockfd;
	struct sockaddr_in serve_addr;

	memset((char *) &serve_addr, 0 ,sizeof(serve_addr));
	serve_addr.sin_family = AF_INET;
	serve_addr.sin_addr.s_addr = inet_addr(SERVER_HOST);
	serve_addr.sin_port = htons(SERVER_PORT);

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("socket create error\n");
		exit(-1);
	}	

	if(connect(sockfd, (struct sockaddr *) &serve_addr, sizeof(serve_addr)) < 0){
		printf("connect error\n");
		exit(-1);
	}

	struct mmylPacket send_packet;
	initPacket(&send_packet);
	setHeader(&send_packet, ACCEPT, "text/plain");
	setHeader(&send_packet, CONNECT, "closed");
	setHeader(&send_packet, JOKE,  OBSERVE);

	char *buffer = toString(&send_packet);

	send(sockfd, buffer, strlen(buffer), 0);

	buffer = (char *)realloc(buffer, BUF_SIZE);
	recv(sockfd, buffer, BUF_SIZE, 0);

	struct mmylPacket recv_packet;
	struct packetHandler handler;

	if(parsePacket(buffer, &recv_packet, &handler)){
		printf("parse error\n");
		exit(-1);
	}	

	printf("%s", recv_packet.body);

	close(sockfd);
	
	return 0;
}
