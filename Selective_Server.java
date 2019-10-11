import java.io.DataInputStream; 
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;

public class Server {
	static ServerSocket Serversocket;
	static DataInputStream dis;
	static DataOutputStream dos;

	public static void main(String[] args) throws SocketException {

		try {
			int a[] = { 30, 40, 50, 60, 70, 80, 90, 100 };//data to send;
			Serversocket = new ServerSocket(8011);//Creating object of ServerSocket to accept the connection from client;
			System.out.println("waiting for connection");
			Socket client = Serversocket.accept();//using the Serversocket object establishing connection;
			dis = new DataInputStream(client.getInputStream());//to get the data from other side of socket i.e receiver side;
			dos = new DataOutputStream(client.getOutputStream());// to send the data to other side of socket i.e sender side;
			System.out.println("The number of packets sent is:" + a.length);
			int y = a.length;//finding the length of array;
			dos.write(y);//sending the length of array on other side of socket i.e receiver side;
			dos.flush();//immediately writes the saved data in the output stream to receiver;

			for (int i = 0; i < a.length; i++) {
				dos.write(a[i]);//sending the array;
				dos.flush();//writes data immediately;
			}

			int k = dis.read();//getting the bit which was not received properly on receiver side;

			dos.write(a[k]);//resending the data which was not received properly on receiver side;
			dos.flush();

		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			try {
				dis.close();//closing datainput stream;
				dos.close();//closing dataoutput stream;
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}//if we don't write this finally block then also the program works correctly but it is recommended to close both the streams so that they should not give any problem if we are using them again and again;
	}
}
