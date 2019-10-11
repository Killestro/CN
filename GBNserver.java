import java.io.*;
import java.net.*;
import java.util.*;
class testserver
{
public static void main(String args[])throws IOException
{
System.out.println("...........Server..........");
System.out.println("Waiting for connection....");
InetAddress addr=InetAddress.getByName("Localhost");

ServerSocket ss=new ServerSocket(1500);
// creates a server socket bound to the specified port for communication.

Socket client=new Socket();

client=ss.accept();
/*The server invokes the accept() method of the ServerSocket class.
This method waits until a client connects to the server on the given port.*/

BufferedInputStream in=new BufferedInputStream(client.getInputStream()); // to get the data from client side
DataOutputStream out=new DataOutputStream(client.getOutputStream());	// to get the data from client side

System.out.println("Received request for sending frames");
int p=in.read();		//reads the no of frames from the client to be sent 

boolean f[]=new boolean[p];	
/* array maintained for resending of frames...having f[true]=frame sent!	f[false]=frame not sent..retransmission required */

int pc=in.read();		//reading the choice as entered by the client
System.out.println("Sending....");

if(pc==0)  				 //no error
{
for(int i=0;i<p;++i)
{
System.out.println("sending frame number "+i);
out.write(i);
out.flush();
System.out.println("Waiting for acknowledgement");
try
{
Thread.sleep(7000);			   // thread to sleep for 7000 milliseconds
}
catch(Exception e){}

int a=in.read();
System.out.println("received acknowledgement for frame "+i+" as "+a);
}
out.flush();
}
else							//error
{
for(int i=0;i<p;++i)
{
if(i==2)
{
System.out.println("sending frame no "+i);
// not actually sending frame no:2 to the client for error sake 
}
else
{
System.out.println("sending frame no "+i);
out.write(i);
out.flush();
System.out.println("Waiting for acknowledgement ");
try
{
Thread.sleep(7000);
}
catch(Exception e){}

int a=in.read();

if(a!=255)	//this condition is true for a=0,1 but for a= -1 it is false.....dunno d reason
{
System.out.println("received ack for frame no: "+i+" as "+a);
f[i]=true;
}
}// end of inner else
}// end of for


for(int a=0;a<p;++a)		//  check which frames have not been ack
{
if(f[a]==false)
{
System.out.println("Resending frame "+a);
out.write(a);
out.flush();
System.out.println("Waiting for ack ");
try
{
Thread.sleep(5000);
}
catch(Exception e){}

int b=in.read();
System.out.println("received ack for frame no: "+a+" as "+b);
f[a]=true;
}
}
out.flush();
}// end of else which is for error 

in.close();
out.close();
client.close();
ss.close();
System.out.println("Quiting");

}// end main method
}// end main class