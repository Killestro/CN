import java.lang.System;
import java.net.*;
import java.io.*;

public class Client {
	static Socket connection;

	public static void main(String a[]) throws SocketException {
		try {
			int v[] = new int[8];//initializing the empty array to store the data received from 
			//int g[] = new int[8];
			int n = 0;
			InetAddress addr = InetAddress.getByName("Localhost");//getting IP address and system name;
			System.out.println(addr);
			connection = new Socket(addr, 8011);//establishing connection using port number=8011;
			DataOutputStream out = new DataOutputStream(
					connection.getOutputStream());// to send the data to other side of socket i.e receiver side;
			DataInputStream in = new DataInputStream(
					connection.getInputStream());//to get the data from other side of socket i.e sender side;
			int p = in.read();//getting the length of array that sender sent firstly;
			System.out.println("No of frame is:" + p);

			for (int i = 0; i < p; i++) {
				v[i] = in.read();//reading the data received from sender side;
				System.out.println(v[i]);
				//g[i] = v[i];
			}
			v[5] = -1;//manually assigning error to the fifth bit;
			for (int i = 0; i < p; i++)
			 {
					System.out.println("Received frame is: " + v[i]);

				}
			for (int i = 0; i < p; i++)
				if (v[i] == -1) {
					System.out.println("Request to retransmit from packet no "
							+ (i+1) + " again!!");
					n = i;
					out.write(n);//sending the bit which is not received properly(we have assigned the error manually);
					out.flush();
				}

			System.out.println();
			
				v[n] = in.read();//getting the  bit which was not received properly;
				System.out.println("Received frame is: " + v[n]);
			
			

			System.out.println("quiting");
		} catch (Exception e) {
			System.out.println(e);
		}

	}
}
