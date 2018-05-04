#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	int socket_fd, cc;
	socklen_t fsize;
	struct sockaddr_in  dest, from;
	bool flag = true;
	bool first = true;

	socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

	memset((char *) &dest,0, sizeof(dest));
	dest.sin_family = (short) AF_INET;
	dest.sin_port = htons((u_short)0x3333);

	const char * initialisation = "ClientIsConnected";

	while (first) {
		sendto(socket_fd,initialisation,strlen(initialisation),0,(struct sockaddr *)&dest,sizeof(dest));
		for (int i = 0; i < 1000; i++) for (int j = 0; j < 1000; j++);
		char router_on[100];
		fsize = sizeof(from);
		if (recv(socket_fd, router_on, 0, 100) != -1) break;
	}

	while (1) {
		char my_message[100];
		char number[10];
		if(flag) {
			puts("please input a message :");
			fgets(my_message, 100, stdin);
			puts("Please input a number between 0 and 1 :");
			fgets(number, 10, stdin);
			char my_message_final[100];
			strcat(my_message_final, my_message);
			strcat(my_message_final, number);
			sendto(socket_fd,my_message_final,strlen(my_message_final),0,(struct sockaddr *)&dest,sizeof(dest));
			flag = false;
		}
		else {
			char reception[100];
			fsize = sizeof(from);
			cc = recvfrom(socket_fd,reception,sizeof(reception),0,(struct sockaddr *)&from,&fsize);
			reception[cc] = '\0';
			puts("Answer :");
			puts(reception);
			fflush(stdout);
			if (strstr(reception, "exit") != NULL) exit(EXIT_SUCCESS);
			flag = true;
		}
	}

	return 0;
}


