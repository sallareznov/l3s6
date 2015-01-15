package exercice2;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetSocketAddress;
import java.net.MulticastSocket;
import java.util.Scanner;

public class MulticastSendUDP implements Runnable {
	
	private InetSocketAddress address;
	
	public MulticastSendUDP(String address, int port) throws IOException {
		this.address = new InetSocketAddress(address,port);
	}
	
	public void envoieMessage(String message) throws IOException {
		MulticastSocket socket = new MulticastSocket();
		byte[] buffer = new byte[1024];
		buffer = message.getBytes();
		DatagramPacket packet = new DatagramPacket(buffer,buffer.length,this.address);
		socket.send(packet);
		socket.close();
	}
	
	@Override
	public void run() {
		while (true) {
			Scanner sc = new Scanner(System.in);
			System.out.println("Message à envoyer ? ");
			String message = sc.nextLine();
			try {
				this.envoieMessage(message);
			} catch (IOException e) {}
		}
	}

}
