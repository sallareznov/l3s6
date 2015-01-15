package exercice1;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.SocketException;
import java.net.BindException;

public class SendUDP {
	
	private SocketAddress address;
	
	public SendUDP(String hostname, int port) throws SocketException {
		this.address = new InetSocketAddress(hostname,port);
	}
	
	public void envoieMessage(String message) throws IOException {
		DatagramSocket socket = new DatagramSocket();
		byte[] buffer = new byte[1024];
		buffer = message.getBytes();
		DatagramPacket packet = new DatagramPacket(buffer,buffer.length,this.address);
		socket.send(packet);
		socket.close();
	}
	
	public static void main(String[] args) throws IOException {
		try {
			SendUDP sUDP = new SendUDP(args[0],Integer.parseInt(args[1]));
			sUDP.envoieMessage(args[2]);
		}
		catch (BindException e) {
			System.out.println("Port déjà utilisé, veuillez en choisir un autre");
		}
		catch (Exception e) {
			System.out.println("Usage : java protocoleUDP/SendUDP hostname port message");
			System.out.println("		- hostname [String] : nom de la machine réceptrice du message");
			System.out.println("		- port [int] : le numéro du port");
			System.out.println("		- message [String] : le message à envoyer");
		}
	}

}
