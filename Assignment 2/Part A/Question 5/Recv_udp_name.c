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

int main(int argc, char *argv[]) {
	int socket_fd, cc;
	socklen_t fsize; 
	struct sockaddr_in  s_in, from; 

	socket_fd = socket (AF_INET, SOCK_DGRAM, 0); 

	memset((char *) &s_in,0, sizeof(s_in)); 
	s_in.sin_family = (short)AF_INET; 
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);   
	s_in.sin_port = htons((u_short)0x3333);

	bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));

	while(1) { 
		char reception[100];
		fsize = sizeof(from); 
		cc = recvfrom(socket_fd,reception,sizeof(reception),0,(struct sockaddr *)&from,&fsize);  
		reception[cc] = '\0';
		printf("%s\n", reception);
		fflush(stdout);

		const char* my_name = "Bismuth";
		sendto(socket_fd,my_name,strlen(my_name),0,(struct sockaddr *)&from,sizeof(from)); 

	}
	close(socket_fd);

	return 0; 
}



