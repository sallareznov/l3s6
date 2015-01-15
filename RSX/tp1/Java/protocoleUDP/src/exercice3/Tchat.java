package exercice3;

import java.io.IOException;

import exercice2.MulticastReceiveUDP;
import exercice2.MulticastSendUDP;

public class Tchat {
	
	public static void main(String[] args) throws IOException {
		Thread threadReceive = new Thread(new MulticastReceiveUDP("224.0.0.1",7654));
		Thread threadSend = new Thread(new MulticastSendUDP("224.0.0.1",7654));
		threadReceive.start();
		threadSend.start();
	}
}
