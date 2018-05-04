#include <sys/types.h>
#include <unistd.h>    
#include <strings.h>
#include <arpa/inet.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#define SIM_LENGTH 10 
#define PORT 1337 

char* nslookup(char* hostname) {
	struct addrinfo* res; // Initialize the structure res.
	char* hostaddr; // Initialize the string (array of char) of the hostadrr.
	struct sockaddr_in* saddr; // Initialize the structure saddr.

	if (0 != getaddrinfo(hostname, NULL, NULL, &res)) { // Check if the hostname really exists. (if getaddrinfo return something else than 0). 
		fprintf(stderr, "Error in resolving hostname %s\n", hostname);
		exit(1); // If the user put a wrong hostname, we close the program.
	}

	// the structure res has know all the information we need (thanks to getdaddringo).

	saddr = (struct sockaddr_in*)res->ai_addr; // saddr get the sock adress of the res, i.e, the hostname.
	hostaddr = inet_ntoa(saddr->sin_addr); // hostadrr needs to focus of the sin_addr, which is what we are interesting by.

	return hostaddr;
}

int main(int argc, char* argv[]) { 
	int sock; 
	struct sockaddr_in cli_name; 
	char* hostname;
	int count;
	int value; 

	if (argc != 2) { // Check if the arguments the user put the good number of arguments i.e : 2 argument, the space, and the hostname.
		perror("Usage: hostnamelookup <hostname>\n");
		exit(1); // If the user didn't put 2 arguments, we close the program.
	}

	hostname = argv[1]; // same use as scanf, we take the hostname the user put.

	printf("Client is alive and establishing socket connection.\n");


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror ("Error opening channel");
		close(sock);
		exit(1);
	}


	memset((char *)&cli_name,0, sizeof(cli_name)); 
	cli_name.sin_family = AF_INET; 
	cli_name.sin_addr.s_addr = inet_addr(nslookup(hostname)); 
	cli_name.sin_port = htons(PORT);


	if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0) {
		perror ("Error establishing communications");
		close(sock);
		exit(1);
	}


	for (count = 1; count <= SIM_LENGTH; count++) {
		read(sock, &value, 4);
		printf("Client has received %d from socket.\n", value);
	}

	printf("Exiting now.\n");

	close(sock); 
	exit(0); 

} 

