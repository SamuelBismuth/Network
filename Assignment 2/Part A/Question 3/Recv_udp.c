// Includes

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

/*
This file receives a message from the Send_udp file
Udp files don't need to know from who the message is send.
*/

int main(int argc, char *argv[]) {

	int socket_fd; // Initialize the socket.
	socklen_t fsize;
	struct sockaddr_in  s_in, from; // Initialize the structure variables.
	struct { char head; u_long  body; char tail;} msg; // Initialize the structure message with three parameters : the characters head and tail, and a u_long body.

	socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // Create a new socket with tree argument, AF_INET : the address domain, SOCK_DGRAM : the type of the socket : udp.
	// if socket_fd < 0 (== -1) problem on the lecture of the socket.

	memset((char *) &s_in,0, sizeof(s_in));  // Clear s_in, some architecture may run without this line, but some other won't.

	/* s_in is a structure, we need here to initialize :
     .sin_family must be short, so we cast the AF_INET into a short number
     .s_addr must set as the ip address
     the ip address is "htonl(INADDR_ANY);", that mean that we do not care about the ip address, so all the  address could send a message.
     .s_port must be set as the ip port.
	 */
	s_in.sin_family = (short)AF_INET;
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
	s_in.sin_port = htons((u_short)0x3333);

	bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in)); // bind associate the socket with the structure s_in.

	while(1) { // Infinite loop (waiting to receive a message).
		fsize = sizeof(from);
		/*
		 socket_fd : the socket.
		 &msg : the message we need to read.
		 sizeof(msg) : the size of the message.
		 (struct sockaddr *)&from : the structure from.
		 &fsize : the size of the structure from.
		 */
		recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize); // Use of the function recvfrom with 6 parameters.
		printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); // Print the message.
		fflush(stdout);
	}
	return 0; // the method is a return (int) statement.
}



