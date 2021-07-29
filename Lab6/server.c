/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h> /* pentru open(), exit() */
#include <fcntl.h> /* O_RDWR */
#include <stdlib.h>

#include "helpers.h"


void usage(char*file)
{
	fprintf(stderr,"Usage: %s server_port file\n",file);
	exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc,char**argv)
{
	int fd;

	if (argc!=3)
		usage(argv[0]);
	
	struct sockaddr_in my_sockaddr, from_station ;
	char buf[BUFLEN];


	/*Deschidere socket*/
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("Eroare la crearea socket-ului");
		close(sock);
		return -1;
	}
	
	/*Setare struct sockaddr_in pentru a asculta pe portul respectiv */
	from_station.sin_port = htons(atoi(argv[1]));
	from_station.sin_family = AF_INET;
	from_station.sin_addr.s_addr = INADDR_ANY;

	
	/* Legare proprietati de socket */
	int b = bind(sock, (struct sockaddr*) &from_station, sizeof(from_station));

	if (b < 0) {
		perror("Eroare la bind");
		close(sock);
		return -1;
	}
	
	
	/* Deschidere fisier pentru scriere */
	DIE((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");
	
	/*
	*  cat_timp  mai_pot_citi
	*		citeste din socket
	*		pune in fisier
	*/
	socklen_t sock_len = sizeof(my_sockaddr);
	int r;
	int copiat;

	while ((r = recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr*) &my_sockaddr, &sock_len)) >= 0) {
		if (strcmp(buf, "FINISHED") == 0) {
			break;
		}

		copiat = write(fd, buf, strlen(buf));

		if (copiat < 0) {
			perror("Eroare la scriere");
			close(sock);
			close(fd);
			return -1;
		}

	}

	if (r < 0) {
		perror("Eroare la primire");
		close(sock);
		close(fd);
		return -1;
	}


	/*Inchidere socket*/	
	close(sock);
	
	/*Inchidere fisier*/
	close(fd);

	return 0;
}
