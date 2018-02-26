#include "lib/uthash/uthash.h"
#include "lib/util.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define MYPORT 			atoi(argv[1])
#define MESSAGE_SIZE 	50
#define BACKLOG			20// No. of pending connections you are willing to handle.

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

FILE* fpLog;
FILE* fpDate;

int sockfd, newfd, ret_bind, theiraddr_size, recv_bytes, send_bytes, yes = 1, i = 0;
struct sockaddr_in myaddr, theiraddr;
char* msg;
char c;


int main(int argc, char *argv[])
{
    daemonize();
    
	/* Allocating memory */
	msg = malloc(200 * sizeof(char));

    fpLog = fopen("/home/DebugLog.txt","a");
       
	if(argc != 2) 
    {
		fprintf(fpLog,"\nUsage: <executable> <server_port_number>\n");
		exit(1);
	}
	/* ------------------- Initialize connection ------------------*/

	/* Get a socket file descriptor */
    sockfd = socket(PF_INET, SOCK_STREAM, 0);

	if(sockfd == -1)
		perror("socket");

	/* Enable reuse of the port number immediately after program
	 * termination.
	*/
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		perror("setsockopt");

	
	myaddr.sin_family = AF_INET; 					/* Host byte order */
	myaddr.sin_port = htons(MYPORT);				/* Network byte order */
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY); 	/* Get my IP address */
	memset(&(myaddr.sin_zero), '\0', 8); 			/* Zero it out */
		
	/* Bind it to your IP address and a suitable port number. */
	ret_bind = bind(sockfd, (struct sockaddr *)&myaddr, sizeof(struct sockaddr));
	
	if(ret_bind == -1)
		perror("bind");
    else 
    	fprintf(fpLog,"\nbind() worked fine "); 
	
		
	/* Try listening to incoming connection requests.
	 * And set a limit on the size of the connection queue.
	 */
	if(listen(sockfd, BACKLOG) == -1)
		perror("listen");
	
	
	theiraddr_size = sizeof(struct sockaddr_in);
	
		
	while(1) {
		/* Accept the incoming connection request. */
		newfd = accept(sockfd, (struct sockaddr *)&theiraddr, &theiraddr_size);

		if(newfd == -1)
			perror("accept");
	    else
	        fprintf(fpLog,"\nThe connection has been accepted ");	
		
			
		/* If you want only one connection, close sockfd. */
		//close(sockfd);	
		/* ------------ End of connection initialization ------------- */


		/* --------------------- Start communicating ------------------ */	
		const char* response = "Thank you for request!";
		while(1)
		{
			/* You expect the client to send you something first. */
			recv_bytes = recv(newfd, (void *)msg, sizeof(msg), 0);
			
		    fprintf(fpLog,"\nUnfortunately, this might be the last fprintf ");
			if(recv_bytes != -1) 
		    {
				if(recv_bytes > 0) 
		        {
					msg[recv_bytes] = '\0';
					fprintf(fpLog,"\nThe message received from the client is: %s\n", msg);
					fprintf(fpLog,"recv_bytes = %d",recv_bytes);
				} 
		        else if (recv_bytes == 0)
		        {
					fprintf(fpLog,"\nThe client closed the connection!\n");						
		 			fprintf(fpLog,"recv_bytes = %d",recv_bytes);	 		
		 			//fclose(fpLog);
		 			break;	
				}
			} 
			else 
				perror("recv");	
		    /* THIS WAS THE END OF THE RECEIVING PART */

			/* SENDING PART */
			 fprintf(fpLog,"\n\n\nBeginning of the code - simple send of string");
		 //    msg = malloc(200 * sizeof(char));
			    
			// fpDate = fopen("/home/DateOutput.txt","r");
		 //    while(1)
		 //    {
		 //        c = getc(fpDate);

		 //        if(c == EOF)
			//  	{
		 //           *(msg + i) = '\0';
			//    		break;	
			//  	}

		 //        *(msg + i++) = c;
		 //    }		
			// fclose(fpDate);
		    send_bytes = send(newfd, (void*)response, strlen(response), 0);//send_bytes = send(newfd, (void*)msg, strlen(msg), 0);
		    fprintf(fpLog,"\nMessage sent to client : [%s]",msg);
		    fprintf(fpLog,"\nNumber of bytes sent = %d",send_bytes);
		}
	}
	/* ---------------- Release all resources ----------------- */
	
	close(newfd);		
    fclose(fpLog);	
	return 0;
}
