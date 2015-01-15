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
		String expediteur = this.packet.getAddress().getHostName();
		String[] messageRecu = new String(this.packet.getData()).split("\n");
		for (int i = 0 ; i < messageRecu.length ; i++) {
			System.out.println(expediteur+" : "+messageRecu[i]);
		}
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
		catch (IndexOutOfBoundsException e) {
			System.out.println("Usage : java ReceiveUDP port | java -jar ReceiveUDP.jar port");
			System.out.println("	- port [int] : le numéro du port");
			System.out.println("Fonction : Ce programme écoute sur le port UDP port et affiche les messages ");
			System.out.println("	   reçus sous la forme de chaînes de caractères");
		}
		catch (BindException e) {
			System.out.println("Port déjà utilisé, veuillez en choisir un autre");
		}
		catch (IllegalArgumentException e) {
			System.out.println("Numéro de port incorrect");
		}
	}

}
