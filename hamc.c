#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


int main()

{

        int sock, bytes_recieved;  
      char send_data[1024],recv_data[1024];
        struct hostent *host;
        struct sockaddr_in server_addr;  

        host = gethostbyname("127.0.0.1");

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;     
        server_addr.sin_port = htons(9500);   
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        bzero(&(server_addr.sin_zero),8); 

        if (connect(sock, (struct sockaddr *)&server_addr,
                    sizeof(struct sockaddr)) == -1) 
        {
            perror("Connect");
            exit(1);
        }

      
        char data[10];
    char dataatrec[10],c,c1,c2,c3,i;
 
    printf("\nEnter 4 bits of data one by one\n");
    scanf("%s",&data[0]);
    scanf("%s",&data[1]);
    scanf("%s",&data[2]);
    scanf("%s",&data[4]);
	
         
       data[6]=data[0]^data[2]^data[4];
    data[5]=data[0]^data[1]^data[4];
    data[3]=data[0]^data[1]^data[2];
 
              
 
               printf("\n Encoded Hamming Code is = %s " , data);
      
	   
printf("\n\nEnter received data bits one by one\n");
    for(i=0;i<7;i++)
        scanf("%s",&data[i]);
          
        send(sock,data,2000, 0); 
       
       
       return 0;
}


