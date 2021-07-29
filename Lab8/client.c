#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "helpers.h"

void usage(char *file)
{
	fprintf(stderr, "Usage: %s server_address server_port\n", file);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, n, ret;
	struct sockaddr_in serv_addr;
	char buffer[BUFLEN];
	fd_set readFds;
	fd_set tmpFds;

	int fdMax;

	FD_ZERO(&tmpFds);
	FD_ZERO(&readFds);

	if (argc < 3) {
		usage(argv[0]);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "socket");

	FD_SET(sockfd, &readFds);
	fdMax = sockfd;
	FD_SET(0, &readFds);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	ret = inet_aton(argv[1], &serv_addr.sin_addr);
	DIE(ret == 0, "inet_aton");

	ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	DIE(ret < 0, "connect");

	while (1) {
		tmpFds = readFds; 

		ret = select(fdMax + 1, &tmpFds, NULL, NULL, NULL);
		DIE(ret < 0, "Nu putem selecta un socket");

		if (FD_ISSET(0, &tmpFds)) {
			memset(buffer, 0, BUFLEN);
			fgets(buffer, BUFLEN - 1, stdin);

			if (strncmp(buffer, "exit", 4) == 0) {
				break;
			}

			n = send(sockfd, buffer, strlen(buffer), 0);
			DIE(n < 0, "SEND FAILED");
		} else {
			memset(buffer, 0, BUFLEN);
			recv(sockfd, buffer, BUFLEN, 0);
			printf("%s\n", buffer);
		}
	}

	close(sockfd);

	return 0;
}