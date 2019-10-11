#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h> 
#include <unistd.h>
#include<string.h> 
#include<strings.h>
#include <arpa/inet.h>

//#define buffsize  150
void main()
{
int b,sockfd,sin_size,con,n,len;
//char buff[256];
char ch[50],ch1[50];
//int op1,op2,result;
if((sockfd=socket(AF_INET,SOCK_STREAM,0))>0)
printf("socket created sucessfully\n");
//printf("%d\n", sockfd);
struct sockaddr_in servaddr;

servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
servaddr.sin_port=6006;

sin_size = sizeof(struct sockaddr_in);
if((con=connect(sockfd,(struct sockaddr *) &servaddr, sin_size))==0); //initiate a connection on a socket
printf("connect sucessful\n");


int n1=0,n2=0;
	
while(1)
{
printf("\nEnter:");
gets(ch);

write(sockfd,&ch,sizeof(ch));

read(sockfd,ch1,sizeof(ch1)); 
printf("\nServer says:%s",ch1); 
n1=strcmp(ch,"quit");
n2=strcmp(ch1,"quit");

	if(n1==0 || n2==0)
		break;

}
close(sockfd);
}


