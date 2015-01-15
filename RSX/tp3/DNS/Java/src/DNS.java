import java.io.IOException;
import java.net.BindException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.SocketException;
import java.util.Random;

public class DNS {

	private SocketAddress address;
	
	private DatagramSocket socket;
	
	private DatagramPacket packet;
	
	private byte[] buffer;
	
	private String message;
	
	private Random rand;
	
	public DNS(String m) throws SocketException {
		this.address = new InetSocketAddress("172.18.12.9",53);
		this.socket = new DatagramSocket();
		this.message = m;
		this.buffer = new byte[256];
		this.rand = new Random();
	}
		
	public void envoieRequete() throws IOException {
		int ind_buffer = 0;
		// identifiant aléatoire
		this.buffer[ind_buffer++] = (byte) this.rand.nextInt(0xff);
		this.buffer[ind_buffer++] = (byte) this.rand.nextInt(0xff);
		// requête récursive
		this.buffer[ind_buffer++] = 0x01;
		this.buffer[ind_buffer++] = 0x00;
		// 1 question
		this.buffer[ind_buffer++] = 0x00;
		this.buffer[ind_buffer++] = 0x01;
		// 0 réponse
		this.buffer[ind_buffer++] = 0x00;
		this.buffer[ind_buffer++] = 0x00;
		// 0 autorité
		this.buffer[ind_buffer++] = 0x00;
		this.buffer[ind_buffer++] = 0x00;
		this.buffer[ind_buffer++] = 0x00;
		this.buffer[ind_buffer++] = 0x00;
		// décodage du message
		byte[] octets_message = this.message.getBytes();
		int cpt = 0;
		byte separateur = ".".getBytes()[0];
		// on remplace l'octet du point par un octet représentant le nombre d'octets rencontrés précédemment qui ne sont pas des points
		for (int i = octets_message.length - 1 ; i >= 0 ; i--) {
			if (octets_message[i] == separateur) {
				octets_message[i] = (byte) cpt;
				cpt = 0;
			}
			else {
				cpt++;
			}
		}
		// "raccrochage" du message au buffer
		this.buffer[ind_buffer++] = (byte) cpt;
		for (int i = 0 ; i < octets_message.length ; i++) {
			this.buffer[ind_buffer++] = octets_message[i];
		}
		// 00 qui indique la fin du message
		this.buffer[ind_buffer++] = 0x00;
		// type
		this.buffer[ind_buffer++] = 0x00;
		this.buffer[ind_buffer++] = 0x01;
		// classe
		this.buffer[ind_buffer++] = 0x00;
		this.buffer[ind_buffer++] = 0x01;
		// envoi du paquet
		this.packet = new DatagramPacket(this.buffer,this.buffer.length,this.address);
		this.socket.send(this.packet);
		this.razData();
	}
	
	public void razData() {
		this.buffer = new byte[256];
		this.packet.setData(this.buffer);
	}
	
	public void ecoutePort() throws IOException {
		this.socket.receive(this.packet);
	}
	
	public void afficheReponse() {
		int n = this.buffer.length;
		System.out.println("RÉPONSE");
		for (int i = 0; i < n; i++) {
			System.out.print(Integer.toHexString((this.buffer[i]+256)%256) + " ");
		}
		System.out.println();
	}
	
	public int getNbReponses() {
		return 256 * this.buffer[6] + this.buffer[7];
	}
	
	public byte[] getReponse(int numero) {
		int pos;
		int octet_depart = 0;
		int octet_final = 0;
		int n;
		
		pos = 12 + this.message.length() + 1 + 5; // pos est sur le premier octet de la première réponse
		for (int i = 1; i <= numero; i++) {
			octet_depart = pos;
			pos += 10;
			n = (this.buffer[pos] * 256) + this.buffer[pos + 1];
			pos += 2 + n;
			octet_final = pos - 1;
		}
		byte[] reponse = new byte[octet_final - octet_depart + 1];
		for (int i = octet_depart; i <= octet_final; i++) {
			reponse[i - octet_depart] = this.buffer[i];
		}
		
		return reponse;
	}
	
	/* 1 : type ANET,5 : type CNAME */
	public static int getTypeReponse (byte[] reponse) {
		return reponse[2] * 256 + reponse[3];
	}
	
	public void afficheUneIP(byte[] reponse) {
		int type = DNS.getTypeReponse(reponse);
		
		switch (type) {
			case 1 : {
				System.out.print("[ANET] ");
				System.out.println((reponse[12]+256)%256 + "." + (reponse[13]+256)%256 + "." + (reponse[14]+256)%256 + "." + (reponse[15]+256)%256);
				break;
			}
			case 5 : {
				System.out.print("[CNAME] ");
				int n = reponse.length;
				int pos = 12;
				String str = "";
				while (pos < n) {
					int c = reponse[pos++];
					
					if ((c+256)%256 == 192) { // cas de l'offset
						int octet_offset = reponse[pos];
						byte[] paquet = this.packet.getData();
						
						while (paquet[octet_offset] != 0) {
							int d = paquet[octet_offset++];
							str += ".";
							for (int i = octet_offset; i < octet_offset + d; i++) {
								str += (Character.toChars(paquet[i]))[0];
							}
							octet_offset += d;
						}
						pos = n;
					}
					
					else {
						if (pos != 13) str += ".";
						for (int i = pos; i < (pos + c); i++) {
							str += Character.toChars(reponse[i])[0];
						}
						pos += c;
					}
				}
				System.out.println(str);
				break;
		
			}
			default : System.out.println("Autre type");
		}
	}
	
	void afficheIP() {
		int nbReponses = getNbReponses();
		System.out.println("\nADRESSES IP");
		for (int i = 1 ; i <= nbReponses ; i++) {
			this.afficheUneIP(getReponse(i));
		}
	}
	
	public String genereAdresseIPAlea() {
		byte[] reponseAlea = this.getReponse(this.rand.nextInt(this.getNbReponses()) + 1);
		String adresseIPAlea = "";
		adresseIPAlea += (reponseAlea[12]+256)%256 + ":" + (reponseAlea[13]+256)%256 + ":" + (reponseAlea[14]+256)%256 + ":" + (reponseAlea[15]+256)%256;
		return adresseIPAlea;
	}
	
	public static int ipToInt(String adresse) throws IOException {
		DNS aDNS = new DNS(adresse);
		aDNS.envoieRequete();
		aDNS.ecoutePort();
		String adresseIPAlea = aDNS.genereAdresseIPAlea();
		int cpt = 0;
		int exp = 0;
		String[] tab = adresseIPAlea.split(":");
		int n = tab.length;
		for (int i = n - 1 ; i >= 0 ; i--) {
			cpt += Math.pow ((Integer.parseInt(tab[i]) + 256) % 256,exp);
			exp++;
		}
		System.out.println("\nUNE ADRESSE IP ALÉATOIRE EN INT");
		adresseIPAlea = adresseIPAlea.replace(':','.');
		System.out.println(adresseIPAlea+" -> "+cpt);
		return cpt;
	}
	
	
	public static void main(String[] args) throws IOException {
		try {
			DNS sDNS = new DNS(args[0]);
			sDNS.envoieRequete();
			sDNS.ecoutePort();
			sDNS.afficheReponse();
			sDNS.afficheIP();
			DNS.ipToInt(args[0]);
		}
		catch (ArrayIndexOutOfBoundsException e) {
			System.out.println("Usage : java DNS requete | java -jar DNS.jar message");
			System.out.println("	- requete [String] : la requête à envoyer");
			System.out.println("Fonction : Ce programme envoie une requête DNS au serveur DNS d'adresse 172.18.12.9");
			System.out.println("	   sur le port UDP 53 puis écoute sur le port et affiche la réponse et les adresses IP");
		}
		catch (BindException e) {
			System.out.println("Port déjà utilisé, veuillez en choisir un autre");
		}
	}
	
}
