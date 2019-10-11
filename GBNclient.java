import java.io.*;
import java.net.*;
import java.math.*;
import java.util.*;

class testclient
{

public static void main(String args[])throws IOException
{
InetAddress addr=InetAddress.getByName("Localhost"); 
/*determines the IP address of a host for the given host name*/
System.out.println(addr);

Socket connection=new Socket(addr,1500);
/*creates a socket obj of socket class and connects it to the specified address on the specified port*/

BufferedInputStream in=new BufferedInputStream(connection.getInputStream());// to get the data from server side
DataOutputStream out=new DataOutputStream(connection.getOutputStream()); // to send the data to server side
//creates buffered byte streams 

Scanner scr=new Scanner(System.in);
// this will be used to accept i/p from console


System.out.println(".......Client........");
System.out.println("Connect");
System.out.println("Enter the number of frames to be requested to the server");
int c=scr.nextInt();

out.write(c);
out.flush();

System.out.println("Enter the type of transfer Error=1 , No Error=0");
int choice=scr.nextInt();
out.write(choice);

int check=0;
int i=0;
int j=0;

if(choice==0) 			// no error
{
for(j=0;j<c;++j)
{
i=in.read();
System.out.println("received frame no: "+i);
System.out.println("Sending acknowledgement for frame no: "+i);
out.write(i);
out.flush();
}
out.flush();
}
else					//error
{
for(j=0;j<c;++j)
{
i=in.read();
if(i==check)		//condition becomes false for frame no:3 and onwards .... as i=3(received frame) but check=2
{
System.out.println("received frame no: "+i);
System.out.println("Sending acknowledgement for frame no: "+i);
out.write(i);
++check;
}
else
{
--j;
System.out.println("Discarded frame no: "+i);
System.out.println("Sending NEGATIVE ack");
out.write(-1);
}
out.flush();
}
}//end of else for error

in.close();				//close input stream when done
out.close();			//close output stream when done
System.out.println("Quiting");

}// end of main method
}// end of main class