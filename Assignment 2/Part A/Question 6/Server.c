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
	int socket_fd, cc, fsize;
	struct sockaddr_in  dest, from; 
	bool flag = false;
	bool first = true;

	socket_fd = socket (AF_INET, SOCK_DGRAM, 0); 

	memset((char *) &dest,0, sizeof(dest));  
	dest.sin_family = (short) AF_INET; 
	dest.sin_port = htons((u_short)0x3333);  

	const char * initialisation = "ServerIsConnected";

	while (first) {
		sendto(socket_fd,initialisation,strlen(initialisation),0,(struct sockaddr *)&dest,sizeof(dest));
		for (int i = 0; i < 1000; i++) for (int j = 0; j < 1000; j++);
		char router_on[100];
		fsize = sizeof(from); 
		if (recv(socket_fd, router_on, 0, 100) != -1) break;   
	}

	while (1) {
		char my_message[100];
		if(flag) {
			puts("please input a message :");
			fgets(my_message, 100, stdin);
			sendto(socket_fd,my_message,strlen(my_message),0,(struct sockaddr *)&dest,sizeof(dest)); 
			flag = false;
		}
		else {
			char reception[100];
			char message[100];
			fsize = sizeof(from); 
			unsigned int offset = (unsigned int) fsize % sizeof(int);
			cc = recvfrom(socket_fd,reception,sizeof(reception),0,(struct sockaddr *)&from,&offset);  
			reception[cc] = '\0';
			if (!first) {
				strncpy (message, reception + strcspn(reception, "\n"), cc);
				puts("Answer :");
				puts(message);
				if (strstr(message, "exit") != NULL) exit(EXIT_SUCCESS);	
				fflush(stdout);	
				flag = true;
				first = true;
			}
			else {
				puts("Answer :");
				puts(reception);
				fflush(stdout);	
				flag = true;
			}
		}
		first = false;
	}

	return 0; 
}


