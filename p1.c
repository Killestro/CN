#include<netinet/in.h>//contains constants and structures needed for internet domain addresses
#include<errno.h>//it consists of macros for defining and documenting errors & it is used along with perror() function
#include<netdb.h>//consists of predefined network database operations
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include<sys/socket.h>//consists of main socket headers and also it defines sockaddr
#include<arpa/inet.h>//definitions of internet operations
#include<sys/ioctl.h>//mainly used to control the Inputs and outputs i.e.to interact with the hardware and the user
#include<sys/time.h>
#include<sys/types.h>//contains a number of basic derived types that should be used whenever appropriate
#include<unistd.h>//standard symbolic constants and types
 
void ProcessPacket(unsigned char* , int); // packet processing
void print_ip_header(unsigned char* , int); //printing ip header 
void print_tcp_packet(unsigned char * , int );// printing tcp header
void print_udp_packet(unsigned char * , int );//printing udp header

 
FILE *logfile;//pointer of type file ... declaration needed for communication between the program and file
struct sockaddr_in source,dest; // sockaddr_in specifies a local or remote endpoint address to which to connect a socket.
/*struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET, AF_INET6
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};
*/
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j; 
 
int main()
{
    int saddr_size , data_size;
    struct sockaddr saddr;
	/*struct sockaddr {
    unsigned short    sa_family;    // address family, AF_xxx
    char              sa_data[14];  // 14 bytes of protocol address
	};*/

         
    unsigned char *buffer = (unsigned char *) malloc(65536); //for storing the message /packet in buffer
     
    logfile=fopen("log.txt","w");// either creating a new file or if file with the same name exists then erasing the contents of the old file and creating a new one
    if(logfile==NULL) 
    {
        printf("Unable to create log.txt file.");
    }
    printf("Starting...\n");
     
    int sock_raw = socket( AF_PACKET , SOCK_RAW , htons(ETH_P_ALL)) ;/*socket function has 3 parameters... first one is the socket family,second one is the type specification of the new socket, When protocol is set to htons(ETH_P_ALL), then all protocols are received.  All incoming packets of that protocol type will be passed to the socket before they are passed to the protocolsimplemented in the kernel.If no error occurs, socket returns a descriptor referencing the new socket. Otherwise, a value of INVALID_SOCKET is returned. 0 or 1 are also valid return values  */	
   
     
    if(sock_raw < 0)
    {
        //Print the error with proper message
        perror("Socket Error");
        return 1;
    }
    while(1)
    {
        saddr_size = sizeof saddr;
        //Receive a packet
        data_size = recvfrom(sock_raw , buffer , 65536 , 0 , &saddr , (socklen_t*)&saddr_size);
	/*The recvfrom() function shall receive a message from a connection-mode or connectionless-mode socket. It is normally used with connectionless-mode sockets . parameters:sock_raw specifies the parameters,buffer points to the where the message is stored,65536 specifies the length in bytes of the buffer, 4th paramter are the flags specify the type of message reception, saddr is a null pointer, or points to a sockaddr structure in which the sending address is to be stored,saddr_size specifies the length of the sockaddr structure pointed to by the address argument.The recvfrom() function shall return the length of the message written to the buffer */
// on successful completion ,revcfrom() will return the length in bytes. if no messages are available to be received , it will return zero otherwise incase of error it will return -1
        if(data_size <0 )
        {
            printf("Recvfrom error , failed to get packets\n");
            return 1;
        }
        //Now process the packet
        ProcessPacket(buffer , data_size);
    }
    close(sock_raw);//close connection
    printf("Finished");
    return 0;
}
 
void ProcessPacket(unsigned char* buffer, int size)
{
    //Get the IP Header part of this packet , excluding the ethernet header
    struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    ++total;
    switch (iph->protocol) //Check the Protocol and do accordingly...
    {
	// the case number are the values of the protocols stored in ip header... i.e. 6 corresponds to TCP protocol,17 for UDP protocol and so on
        case 1:  //ICMP Protocol
            ++icmp;
           // print_icmp_packet( buffer , size);
            break;
         
        case 2:  //IGMP Protocol
            ++igmp;
            break;
         
        case 6:  //TCP Protocol
            ++tcp;
            print_tcp_packet(buffer , size);
            break;
         
        case 17: //UDP Protocol
            ++udp;
            print_udp_packet(buffer , size);
            break;
         
        default: //Some Other Protocol like ARP etc.
            ++others;
            break;
    }
    printf("TCP : %d   UDP : %d   ICMP : %d   IGMP : %d   Others : %d   Total : %d\r", tcp , udp , icmp , igmp , others , total);
}
 
void print_ethernet_header(unsigned char* Buffer, int Size)
{
    struct ethhdr *eth = (struct ethhdr *)Buffer; //creating an ethernet header for buffer of size 'Size'
     
    fprintf(logfile , "\n");
    fprintf(logfile , "Ethernet Header\n");

    fprintf(logfile , "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
// destination ethernet addrress is stored in h_dest.Now that we have the network packets in our buffer, we will get information about the Ethernet header. The Ethernet header contains the physical address of the source and destination, or the MAC address and protocol of the receiving packet. 

    fprintf(logfile , "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );

    fprintf(logfile , "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);//h_proto gives information about the next layer.for eg If you get 0x800 (ETH_P_IP), it means that the next header is the IP header.
}
 
void print_ip_header(unsigned char* Buffer, int Size)
{
    print_ethernet_header(Buffer , Size);
   
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );//build the ip header
    iphdrlen =iph->ihl*4;//4*32 bit words in header
     
    memset(&source, 0, sizeof(source));// memset() is used to fill a block a memory with a particular value. 1st parameter is  pointer to the block of memory to fill, 0(the value) is the value to be set. The value is passed as an int, but the function fills the block of memory using the unsigned char conversion of this value,3rd parameter is the number of bytes to be set
    source.sin_addr.s_addr = iph->saddr;//set source IP address
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;//set destination IP address	
     
    fprintf(logfile , "\n");
    fprintf(logfile , "IP Header\n");
    fprintf(logfile , "   |-IP Version        : %d\n",(unsigned int)iph->version);// ip version
    fprintf(logfile , "   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);//header length
    fprintf(logfile , "   |-Type Of Service   : %d\n",(unsigned int)iph->tos);// type of service
    fprintf(logfile , "   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));//total length of packet
    fprintf(logfile , "   |-Identification    : %d\n",ntohs(iph->id));//
    //fprintf(logfile , "   |-Reserved ZERO Field   : %d\n",(unsigned int)iphdr->ip_reserved_zero);
    //fprintf(logfile , "   |-Dont Fragment Field   : %d\n",(unsigned int)iphdr->ip_dont_fragment);
    //fprintf(logfile , "   |-More Fragment Field   : %d\n",(unsigned int)iphdr->ip_more_fragment);
    fprintf(logfile , "   |-TTL      : %d\n",(unsigned int)iph->ttl);//time to live 
    fprintf(logfile , "   |-Protocol : %d\n",(unsigned int)iph->protocol);//protocol in IP 
    fprintf(logfile , "   |-Checksum : %d\n",ntohs(iph->check));// 16 bit check sum of IP header.
    fprintf(logfile , "   |-Source IP        : %s\n",inet_ntoa(source.sin_addr));//source address
    fprintf(logfile , "   |-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));//destination address

/* inet_ntoa: The inet_ntoa() function converts the Internet host address in, given in network byte order, to a string in IPv4 dotted-decimal notation. The string is returned in a statically allocated buffer, which subsequent calls will overwrite. 
*/
/*ntohs: converts unsigned short integer given by iph->check into network byte order*/
//network byte order: sequential order in which the bytes are arranged to form large numerical values when stored or sent over a link. the most significant byyte is stored or sent first(has the lowest address) and subequently the packets are stored or sent in a decreasing order
}
 
void print_tcp_packet(unsigned char* Buffer, int Size)
{
    unsigned short iphdrlen;
     
   struct iphdr *iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );//build the ip header
  iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));//build the tcp header
             
    int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;//The header length field (doff in the structure) is the total length of the header, including options
     
    fprintf(logfile , "\n\n***********************TCP Packet*************************\n");  
         
    print_ip_header(Buffer,Size);
        

//The explanation for all attribute is given at https://www.cs.auckland.ac.nz/~nevil/ruby-libtrace/TCP.html 
    fprintf(logfile , "\n");
    fprintf(logfile , "TCP Header\n");
    fprintf(logfile , "   |-Source Port      : %u\n",ntohs(tcph->source));
    fprintf(logfile , "   |-Destination Port : %u\n",ntohs(tcph->dest));
    fprintf(logfile , "   |-Sequence Number    : %u\n",ntohl(tcph->seq));
    fprintf(logfile , "   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    fprintf(logfile , "   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);// tcph->doff: for size
    //fprintf(logfile , "   |-CWR Flag : %d\n",(unsigned int)tcph->cwr);
    //fprintf(logfile , "   |-ECN Flag : %d\n",(unsigned int)tcph->ece);
    fprintf(logfile , "   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
    fprintf(logfile , "   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
    fprintf(logfile , "   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
    fprintf(logfile , "   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
    fprintf(logfile , "   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
    fprintf(logfile , "   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
    fprintf(logfile , "   |-Window         : %d\n",ntohs(tcph->window));
    fprintf(logfile , "   |-Checksum       : %d\n",ntohs(tcph->check));
    fprintf(logfile , "   |-Urgent Pointer : %d\n",tcph->urg_ptr);
    fprintf(logfile , "\n");
   /*ntohs: converts unsigned short integer given by iph->check into network byte order*/
//network byte order: sequential order in which the bytes are arranged to form large numerical values when stored or sent over a link. the most significant byyte is stored or sent first(has the lowest address) and subequently the packets are stored or sent in a decreasing order    
  //  fprintf(logfile , "IP Header\n");
   // fprintf(logfile , "TCP Header\n");
    fprintf(logfile , "\n###########################################################");
}
 
void print_udp_packet(unsigned char *Buffer , int Size)
{
     
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer +  sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4;
     
    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof udph;
     
    fprintf(logfile , "\n\n***********************UDP Packet*************************\n");
     
    print_ip_header(Buffer,Size);           
     //The explanation for all attribute is given at https://www.cs.auckland.ac.nz/~nevil/ruby-libtrace/UDP.html
    fprintf(logfile , "\nUDP Header\n");
    fprintf(logfile , "   |-Source Port      : %d\n" , ntohs(udph->source));
    fprintf(logfile , "   |-Destination Port : %d\n" , ntohs(udph->dest));
    fprintf(logfile , "   |-UDP Length       : %d\n" , ntohs(udph->len));
    fprintf(logfile , "   |-UDP Checksum     : %d\n" , ntohs(udph->check));
/*ntohs: converts unsigned short integer given by iph->check into network byte order*/
     
    fprintf(logfile , "\n");
    //fprintf(logfile , "IP Header\n");
  
         
    //fprintf(logfile , "UDP Header\n");
  
    fprintf(logfile , "\n###########################################################");
}




/*

You need net connection on when running this code and browse something on the internet..
alvis@ubuntu:~/Desktop$ gcc p1.c
alvis@ubuntu:~/Desktop$ sudo ./a.out
Starting...
TCP : 616   UDP : 63   ICMP : 0   IGMP : 0   Others : 0   Total : 679

*/


/* NOTE:: WHY DO WE HAVE TO USE HTONS()
If you've got a 16-bit int, you use htons() on data you're about to send, and ntohs()
on data you've received; if you've got a 32-bit int, you use htonl()/ntohl()...  (Sadly,
there's no standard 64-bit versions yet, so if you've got a 64-bit int, you kind of have
to roll your own...)  If you've got anything other than a raw int of some kind, you don't
use them...  Ie: single characters or strings of multiple characters never need any
such trickery done to them...  And, you only need to do it on ints that actually are
transmitted across the network...  (This includes ones you actually send yourself, as
well as ones sent for you by the TCP/IP stack in packet headers and such...  Eg: raw
IPs, port numbers, etc...)

The reason this is necessary is due to different machines having different native ideas
about how to layout integer values in memory...  Technically, if you were sending to a
host that was the same architecture as you (or, at least the same endianness), you
wouldn't have to bother converting to/from network order on any ints you send,
since both ends would agree on how to interpret them...  But, if you're coding
something intended to work generically, communicating with any machine on the
Net, you have to allow for the fact that that machine may not agree with you about
how ints are stored, so you'll both need to talk to each other in network order, and
do your own conversions to/from your native host order as necessary...
*/
 
