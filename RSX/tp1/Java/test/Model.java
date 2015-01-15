import java.net.InetAddress;
import java.net.UnknownHostException;
import java.util.Observable;


public class Model extends Observable {
	
	private Receive receive;
	private Send send;
	
	private InetAddress address;
	private int port;
	private boolean connected;
	
	private String contents;
	
	public static final int BUFFER_SIZE = 1024;
	
	public Model() {
		receive = new Receive(this);
		send = new Send(this);
		contents = "";
		connected = false;
	}
	
	public void connect(String address, String port) {
		
		try {
			this.address = InetAddress.getByName(address);
			this.port = Integer.parseInt(port);
			connected = true;
			if(receive.isAlive()) {
				//TODO update
				receive.stop();
			}
			this.contents += "Vous etes connecté !\n";
			receive.start();
		 	setChanged();
			notifyObservers();
			
		} catch (UnknownHostException e) {

		} catch(NumberFormatException e) {
			
		}
		
	}
	
	public InetAddress getAddress() {
		return this.address;
	}
	
	public int getPort() {
		return this.port;
	}
	
	public void receive(String message) {
		this.contents += message+'\n';
	 	setChanged();
		notifyObservers();
	}
	
	public void send(String message) {
		
		if(address != null && port != 0)
			send.send(message);
	}
	
	public boolean isConnected() {
		return connected;
	}
	
	public String getContents() {
		return this.contents;
	}
	
	

}
