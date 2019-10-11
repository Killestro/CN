#include <sys/types.h> //Basic System Data types
#include <sys/socket.h>  // Structure needed by socket
#include <netinet/in.h>	//sockaddr_in() and other internet definitions
#include <arpa/inet.h>	//inet(3) functions
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


int main()
{
        int sock, connected, bytes_recieved , true = 1,i;  
       char send_data [1024], recv_data[1024],buffer[20];       
int c1,c2,c3,c;
        struct sockaddr_in server_addr,client_addr;    
        int sin_size;
        
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

/* stetscokopt() is used to set the options that affect socket. sock is a socket descriptor, SOL_SOCKET is the level that specifies the code in the system that interprets option(general socket code or protocols specific code),SO_REUSEADDR is the option name that allow local address to reuse, &true is optval that is pointer to a variable from which the new value of option is fetched bt setsocket(), sizeof(in) is the size of this variable*/
        
if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
            perror("Setsockopt");
            exit(1);
        }
        
        server_addr.sin_family = AF_INET;         
        server_addr.sin_port = htons(9500);     
        server_addr.sin_addr.s_addr = INADDR_ANY; 
        bzero(&(server_addr.sin_zero),8); /* function having to arguments one is pointer to the buffer 
						and other is size of buffer sin_zero[8]*/

        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
                                                                       == -1) {
            perror("Unable to bind");
            exit(1);
        }

        if (listen(sock, 5) == -1) {
            perror("Listen");
            exit(1);
        }
		
	printf("\n TCP Server Waiting for client");
        fflush(stdout);


      

            sin_size = sizeof(struct sockaddr_in);

            connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

            printf("\n I got a connection from (%s , %d)",
                   inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		/*inet_ntoa converts an IPv4 address from dotted decimal string to its 32 bit binary network byte ordered value */

           
	        bytes_recieved = recv(connected,recv_data,1024,0);
	
	        recv_data[bytes_recieved] = '\0';
             
              
               printf("\n Recieved Encoded data is = %s " , recv_data);
              c1=recv_data[6]^recv_data[4]^recv_data[2]^recv_data [0];
    c2=recv_data[5]^recv_data[4]^recv_data[1]^recv_data[0];
    c3=recv_data[3]^recv_data[2]^recv_data[1]^recv_data[0];
    c=c3*4+c2*2+c1 ;
		
		
	        if(c==0) {
        printf("\nNo error while transmission of data\n");
    }
    else {
        printf("\nError on position %d",c);
        
     
        
        printf("\nCorrect message is\n");
 
        
      if(recv_data[7-c]==0)
            recv_data[7-c]='1';
        else
            recv_data[7-c]='0';
        
        for (i=0;i<7;i++) {
            printf("%c",recv_data[i]);
           
        }
         printf("\n");
         
    }
		
	
      close(sock);
      return 0;
} 

 
