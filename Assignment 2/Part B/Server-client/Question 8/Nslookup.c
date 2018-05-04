#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
	struct addrinfo* res; // I the structure res.
	char* hostname; // Initialize the string (array of char) of the hostname.
	char* hostaddr; // Initialize the string (array of char) of the hostadrr.
	struct sockaddr_in* saddr; // Initialize the structure saddr.

	if (argc != 2) { // Check if the arguments the user put the good number of arguments i.e : 2 argument, the space, and the hostname.
		perror("Usage: hostnamelookup <hostname>\n");
		exit(1); // If the user didn't put 2 arguments, we close the program.
	}

	hostname = argv[1]; // same use as scanf, we take the hostname the user put.

	if (0 != getaddrinfo(hostname, NULL, NULL, &res)) { // Check if the hostname really exists. (if getaddrinfo return something else than 0).
		fprintf(stderr, "Error in resolving hostname %s\n", hostname);
		exit(1); // If the user put a wrong hostname, we close the program.
	}

	// the structure res has know all the information we need (thanks to getdaddringo).

	saddr = (struct sockaddr_in*)res->ai_addr; // saddr get the sock adress of the res, i.e, the hostname.
	hostaddr = inet_ntoa(saddr->sin_addr); // hostadrr needs to focus of the sin_addr, which is what we are interinsting by.

	printf("Address for %s is %s\n", hostname, hostaddr); // Print the hostname.
	exit(0); // We don't need a return statment because of the exit(0).
}

