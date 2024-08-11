#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
/* fork wait */
#include <sys/wait.h>
/* socket */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
/* shared memory */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
/* MMYL make you laugh */
#include "mmyl/mmyl.h"

#define SERVER_PORT 8888
#define MAX_CONNECT 5
#define BUF_SIZE 1024
#define FILE_PATH "mmyl/joke/create.py"

int recvProcess(char *buffer, struct mmylPacket *packet, struct packetHandler *handler);

int sendProcess(struct mmylPacket *packet, struct packetHandler *handler);

char *getToken();

int main(int argc, char *argv[]){
	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("socket create error\n");
		exit(-1);
	}	

	struct sockaddr_in cli_addr, serve_addr;

	memset((char *) &serve_addr, 0, sizeof(serve_addr));
	serve_addr.sin_family = AF_INET;
	serve_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serve_addr.sin_port = htons(SERVER_PORT);

	if(bind(sockfd, (struct sockaddr *) &serve_addr, sizeof(serve_addr)) < 0){
		printf("bind error\n");
		exit(-1);
	}

	listen(sockfd, MAX_CONNECT);

	int pid;
	for(;;){
		int clilen = sizeof(cli_addr);
		int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

		if(newsockfd < 0){
			printf("accept error\n");
			exit(-1);
		}	

		if((pid = fork()) < 0){
			printf("fork fail\n");
			exit(-1);
		}else if(pid == 0){
			/* child process */

			/* Received data handler */
			char *recv_buffer = (char *)malloc(BUF_SIZE);
			memset(recv_buffer, 0, BUF_SIZE);
			recv(newsockfd, recv_buffer, BUF_SIZE, 0);

			struct mmylPacket recv_packet;
			struct packetHandler handler;

			if(recvProcess(recv_buffer, &recv_packet, &handler))
				exit(-1);

			/* Sending data handler */
			struct mmylPacket send_packet;
			initPacket(&send_packet);

			if(sendProcess(&send_packet, &handler))
				exit(-1);			

			/* token as memory name */
			char *token = getToken();
			int newPid = fork();

			if(newPid < 0){
				printf("child fork fail\n");
				exit(-1);
			}else if(newPid == 0){
				/* self-child process get joke */
				execlp(FILE_PATH, FILE_PATH, handler.jokeType, token, (char *)NULL);
			}
			
			/* shared memory get jokes */
			char *addr;
			int fd;
                        if((fd = shm_open(token, O_CREAT | O_RDWR, 0666)) < 0){
                                printf("memory open error\n");
                                exit(-1);
                        }

			if(ftruncate(fd, BUF_SIZE) == -1){
				printf("fstat error\n");
				exit(-1);
			}

			addr = mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
			if(addr == MAP_FAILED){
				printf("map fail\n");
				exit(-1);
			}

			/* wait for joke creation */
			wait(NULL);
			setBody(&send_packet, handler.mainType, handler.subType, addr, strlen(addr));	
			
			char *send_buffer = toString(&send_packet);
			send(newsockfd, send_buffer, strlen(send_buffer), 0);

			munmap(addr, BUF_SIZE);
			shm_unlink(addr);
			close(fd);

			close(sockfd);
			abort();
			exit(0);
		}

		close(newsockfd);
	}
}

int recvProcess(char *buffer, struct mmylPacket *packet, struct packetHandler *handler){
	if(parsePacket(buffer, packet, handler)){
        	printf("parse error\n");
                return -1;
        }

	return 0;
}

int sendProcess(struct mmylPacket *packet, struct packetHandler *handler){
	setHeader(packet, JOKE, handler->jokeType);
        setHeader(packet, CONNECT, "closed");

	return 0;
}

char *getToken(){
	unsigned int length = (rand() % 19) + 1;
	char *output = (char *)malloc(length);

	for(size_t i = 0; i < length; ++i){
		/* ASCII code 32 ~ 126 */
		*(output + i) = (rand() % 95) + 32;
	}

	return output;
}
