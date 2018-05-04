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

// Function nslookup : param hostname, return ip.
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
	hostaddr = inet_ntoa(saddr->sin_addr); // hostadrr needs to focus of the sin_addr, which is what we are interinsting by.

	return hostaddr;
}

int main(int argc, char* argv[]) {
	int sock;
	struct sockaddr_in cli_name;
	char* urlString;
	int recv;
	char hostname[100];
	int port = 80;
	char path[100];
	char request[500];
	char buffer[1024];

	if (argc != 2) { // Check if the arguments the user put the good number of arguments i.e : 2 argument, the space, and the hostname.
		perror("Usage: url <url>\n");
		exit(1); // If the user didn't put 2 arguments, we close the program.
	}

	urlString = argv[1]; // Same use as scanf, we take the url the user put.

	// Parse the url.
	if (sscanf(urlString, "http://%99[^:]:%i%199[^\n]", hostname, &port, path) == 3);
	else if (sscanf(urlString, "http://%99[^/]%199[^\n]", hostname, path) == 2 );

	printf("hostname = \"%s\"\n", hostname);
	printf("path = \"%s\"\n", path);

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
	cli_name.sin_port = htons(port);

	if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0) {
		perror ("Error establishing communications");
		close(sock);
		exit(1);
	}

	bzero(request, sizeof(request));
	strcpy(request, "GET ") ;
	strcat(request, urlString);
	strcat(request, " HTTP/1.0\nHOST: ");
	strcat(request, hostname);
	strcat(request, "\n\n");
	puts(request);

	write(sock, request, sizeof(request));

	recv = read(sock, buffer, sizeof(buffer));
	while (recv > 0) {
		write(1, buffer, recv);
		recv = read(sock, buffer, sizeof(buffer));
	}

	printf("Exiting now.\n");

	close(sock);
	exit(0);

}

