// Protocoale de comunicatii
// Laborator 9 - DNS
// dns.c

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int usage(char* name)
{
	printf("Usage:\n\t%s -n <NAME>\n\t%s -a <IP>\n", name, name);
	return 1;
}

// Receives a name and prints IP addresses
void get_ip(char* name)
{
	int ret;
	struct addrinfo hints, *result, *p;

	// TODO: set hints
	hints.ai_socktype = 0;
	hints.ai_protocol = 0;
	hints.ai_addrlen = strlen(name);
	hints.ai_canonname = NULL;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE | AI_CANONNAME;

	// TODO: get addresses

	int ans = getaddrinfo(name, NULL, &hints, &result);

	if (ans != 0) {
		perror("Eroare");
		return;
	}
	// TODO: iterate through addresses and print them

	p = result;
	while (p) {
		char *str = malloc(sizeof(BUFSIZ));

		if (p->ai_family == AF_INET) {
			struct sockaddr_in* s = (struct sockaddr_in*) (p->ai_addr);
			inet_ntop(AF_INET, &s->sin_addr, str, BUFSIZ);
			printf("%s  %d  %s  %d \n", str, s->sin_port, p->ai_canonname, p->ai_socktype);
		} else if (p->ai_family == AF_INET6) {
			struct sockaddr_in6* s = (struct sockaddr_in6*) (p->ai_addr);
			inet_ntop(AF_INET, &s->sin6_addr, str, BUFSIZ);
			printf("%s  %d  %s  %d \n", str, s->sin6_port, p->ai_canonname, p->ai_socktype);
		}

		p = p->ai_next;
	}

	// TODO: free allocated data

	p = result;
	while (p) {
		struct addrinfo *toDelete = p;

		p = p->ai_next;
		free(toDelete);
	}
}

// Receives an address and prints the associated name and service
void get_name(char* ip)
{
	int ret;
	struct sockaddr_in addr;
	char host[1024];
	char service[20];

	// TODO: fill in address data
	addr.sin_family = AF_INET;
	inet_aton(ip, &addr.sin_addr);

	// TODO: get name and service

	int ans = getnameinfo(&addr, sizeof(struct sockaddr), host, BUFSIZ, service, BUFSIZ, 0);

	if (ans != 0) {
		perror("Eroare");
		return;
	}

	// TODO: print name and service
	printf("%s  %s\n", host, service);
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		return usage(argv[0]);
	}

	if (strncmp(argv[1], "-n", 2) == 0) {
		get_ip(argv[2]);
	} else if (strncmp(argv[1], "-a", 2) == 0) {
		get_name(argv[2]);
	} else {
		return usage(argv[0]);
	}

	return 0;
}
