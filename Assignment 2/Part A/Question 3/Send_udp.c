// Includes.

#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>

/*
This file send a message to the Recv_udp file.
You need to run first Recv_udp to get the message.
 */

int main(int argc, char *argv[]) {

	int socket_fd; // Initialize the socket.
	struct sockaddr_in  dest; // Initialize the structure variable.
	struct hostent *hostptr;
	struct { char head; u_long body; char tail; } msgbuf; // Initialize the structure message with three parameters : the characters head and tail, and a u_long body.

	socket_fd = socket (AF_INET, SOCK_DGRAM, 0); // Create a new socket with tree argument, AF_INET : the address domain, SOCK_DGRAM : the type of the socket : udp.
	// if socket_fd < 0 (==-1) problem on the lecture of the socket

	memset((char *) &dest,0, sizeof(dest));  // Clear s_in, some architecture may run without this line, but some other won't.
	hostptr = gethostbyname(argv[1]);  // hostptr is the ip address : the second place (1) in the array argv because the first(0) is the space.
	dest.sin_family = (short) AF_INET; // .sin_family must be short, so we cast the AF_INET into a short number.
	memcpy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length); // Copies h.length characters from memory area hostptr to memory area dest.sin_addr.
	dest.sin_port = htons((u_short)0x3333);  //.sin_port must be set as the ip port.

	msgbuf.head = '<';
	msgbuf.body = htonl(getpid()); // The message body contains the process id of the id process.
	msgbuf.tail = '>';

	/*
	  socket_fd : the socket.
	  &msgbuf : the message we need to send.
	  sizeof(msgbuf) : the size of the message.
	  (struct sockaddr *)&dest : the structure dest.
	  &fsize : the size of the structure dest.
	 */
	sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,sizeof(dest)); // We use the method sendto with 6 parameters.

	return 0; // the method is a return (int) statement
}



