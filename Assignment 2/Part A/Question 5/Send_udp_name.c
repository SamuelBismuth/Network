#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) { 
	int socket_fd;
	struct sockaddr_in  dest; 
	struct hostent *hostptr;
	const char* my_name = "Samuel";

	socket_fd = socket (AF_INET, SOCK_DGRAM, 0); 
	memset((char *) &dest,0, sizeof(dest));  
	hostptr = gethostbyname("127.0.0.1");

	dest.sin_family = (short) AF_INET; 
	memcpy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
	dest.sin_port = htons((u_short)0x3333);  

	sendto(socket_fd,my_name,strlen(my_name),0,(struct sockaddr *)&dest,sizeof(dest)); 

	char reception[200];
	recvfrom(socket_fd,reception,sizeof(reception),0,NULL,NULL);  
	reception[sizeof(reception)] = '\0';
	printf("%s\n", reception);
	fflush(stdout);
	close(socket_fd);

	return 0; 
}



