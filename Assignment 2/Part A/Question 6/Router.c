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
#include <stdbool.h>
#include <stdlib.h>

char *substr(char *src,int pos,int len) { 
	char *dest=NULL;                        
	if (len > 0) {                            
		dest = (char *) malloc(len+1);        
		strncat(dest,src+pos,len);            
	}                                       
	return dest;                            
}

int main(int argc, char *argv[]) {
	int socket_fd, cc; 
	socklen_t fsize;
	struct sockaddr_in  s_in, from, server, client; 
	bool first = true;
	bool flag_client = true;
	bool flag_server = true;

	socket_fd = socket (AF_INET, SOCK_DGRAM, 0); 

	memset((char *) &s_in,0, sizeof(s_in)); 
	s_in.sin_family = (short)AF_INET; 
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);   
	s_in.sin_port = htons((u_short)0x3333);

	bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));

	const char * on = "IamOn";

	while (first) {
		char first_message[100];
		fsize = sizeof(from); 
		cc = recvfrom(socket_fd,first_message,sizeof(first_message),0,(struct sockaddr *)&from,&fsize);  
		first_message[cc] = '\0';
		if (strcmp(first_message, "ClientIsConnected") == 0 && flag_client) {
			client = from;
			sendto(socket_fd,on,strlen(on),0,(struct sockaddr *)&client,sizeof(client));
			flag_client = false;
		}
		if (strcmp(first_message, "ServerIsConnected") == 0 && flag_server) {
			server = from;
			sendto(socket_fd,on,strlen(on),0,(struct sockaddr *)&server,sizeof(server));
			flag_server = false;
		}
		if (flag_server == 0 && flag_client == 0) first = false;
	}

	puts("Everyone connected !");

	while(1) {
		char reception[100];
		memset(reception, 0, sizeof(reception));
		fflush(stdout);
		fsize = sizeof(from); 
		cc = recvfrom(socket_fd,reception,sizeof(reception),0,(struct sockaddr *)&from,&fsize);  
		reception[cc] = '\0';
		if (strstr(reception, "exit") != NULL) {
			sendto(socket_fd,reception,strlen(reception),0,(struct sockaddr *)&client,sizeof(client));
			sendto(socket_fd,reception,strlen(reception),0,(struct sockaddr *)&server,sizeof(server));
			exit(EXIT_SUCCESS);	
		}
		if (server.sin_port == from.sin_port) sendto(socket_fd,reception,strlen(reception),0,(struct sockaddr *)&client,sizeof(client)); 
		else if (client.sin_port == from.sin_port) {
			double random_number = (double) rand() / (double) RAND_MAX ;
			char message[100];
			char number[10];
			strncpy (message, reception, strcspn(reception, "0") - 1);
			message[strcspn(reception, "0") - 1] = '\0';
			strncpy (number, reception + strcspn(reception, "0") - 1, cc);
			number[cc] = '\0';
			double number_choosed = atof(number);
			printf("Choose %f, random %f\n", number_choosed, random_number);
			fflush(stdout);	
			if (random_number < number_choosed)
				sendto(socket_fd,message,strlen(message),0,(struct sockaddr *)&server,sizeof(server));
			else {
				const char* error_message = "The random number is higher than the chosen number.";
				sendto(socket_fd,error_message,strlen(error_message),0,(struct sockaddr *)&server,sizeof(server));
			}
		}
	}

	return 0; 
}

