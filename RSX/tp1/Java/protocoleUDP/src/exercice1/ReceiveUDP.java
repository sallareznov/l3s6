package exercice1;

import java.io.IOException;
import java.net.BindException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

public class ReceiveUDP {
	
	private DatagramSocket socket;
	
	private DatagramPacket packet;
	
	private byte[] buffer;
	
	public ReceiveUDP(int port) throws SocketException {
		this.buffer = new byte[1024];
		this.socket = new DatagramSocket(port);
		this.packet = new DatagramPacket(this.buffer,this.buffer.length);
	}
	
	public void ecoutePort() throws IOException {
		this.socket.receive(this.packet);
		String messageRecu = new String(this.packet.getData());
		String expediteur = this.packet.getAddress().getHostName();
		System.out.println(expediteur+" : "+messageRecu);
		this.razData();
	}
	
	public void razData() {
		this.buffer = new byte[1024];
		this.packet.setData(this.buffer);
	}
	
	public static void main(String[] args) throws IOException {
		try {
			ReceiveUDP rUDP = new ReceiveUDP(Integer.parseInt(args[0]));
			while (true) {
				rUDP.ecoutePort();
			}	
		}
		catch (BindException e) {
			System.out.println("Port déjà utilisé, veuillez en choisir un autre");
		}
		catch (Exception e) {
			System.out.println("Usage : java protocoleUDP/ReceiveUDP port");
			System.out.println("		- port [int] : le numéro du port");
		}
	}

}
