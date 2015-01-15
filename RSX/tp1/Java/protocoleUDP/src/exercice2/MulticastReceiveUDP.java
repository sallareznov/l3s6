package exercice2;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class MulticastReceiveUDP implements Runnable {
	
	private MulticastSocket socket;
	
	private DatagramPacket packet;
	
	private byte[] buffer;
	
	public MulticastReceiveUDP(String address, int port) throws IOException {
		this.socket = new MulticastSocket(port);
		InetAddress group = InetAddress.getByName(address);
		this.socket.joinGroup(group);
		this.buffer = new byte[1024];
		this.packet = new DatagramPacket(buffer,buffer.length);
	}
	
	public MulticastSocket getSocket() {
		return this.socket;
	}
	
	public DatagramPacket getPacket() {
		return this.packet;
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
	
	@Override
	public void run() {
		while (true) {
			try {
				this.ecoutePort();
			} catch (IOException e) {}
		}
	}
	
}

