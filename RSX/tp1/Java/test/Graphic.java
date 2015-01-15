import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Label;
import java.awt.TextArea;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.Observable;
import java.util.Observer;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.JTextArea;
import javax.swing.JTextField;


public class Graphic extends JFrame implements Observer {
	
	private JTextField address_area;
	private JTextField port_area;
	private JTextArea receive_area;
	private JTextArea send_area;
	private JButton send_button;
	private JButton connect_button;
	private Model model;
	
	
	public Graphic(final Model model) {
		this.model = model;
		this.setLayout(new GridBagLayout());
		GridBagConstraints gc = new GridBagConstraints();
		
		gc.fill = GridBagConstraints.BOTH;
		gc.weightx = 1.0;
		gc.weighty = 1.0;
		
		gc.gridx = 0;
		gc.gridy = 0;
		address_area = new JTextField();
		address_area.setText("224.0.0.1");
		address_area.setPreferredSize(new Dimension(0, 30));
		gc.weighty = 0.0;
		this.add(address_area, gc);
		
		gc.gridx = 1;
		gc.gridy = 0;
		port_area = new JTextField();
		port_area.setText("7654");
		this.add(port_area, gc);
		
		gc.gridx = 2;
		gc.gridy = 0;
		connect_button = new JButton("Connect");
		connect_button.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				
				model.connect(address_area.getText(), port_area.getText());
				if(model.isConnected()) {
					connect_button.setText("Connected");
					connect_button.setEnabled(false);
				}
			}
			
		});
		this.add(connect_button, gc);
		
		gc.gridx = 0;
		gc.gridy = 1;
		gc.gridwidth = 3;
		gc.weighty = 1.0;
		receive_area = new JTextArea();
		receive_area.setEditable(false);
		receive_area.setLineWrap(true);
		
		JScrollPane scroll = new JScrollPane(receive_area);

		this.add(scroll, gc);
		
		gc.gridx = 0;
		gc.gridy = 2;
		gc.weighty = 0.0;
		send_area = new JTextArea();
		send_area.setPreferredSize(new Dimension(0, 100));
		send_area.setMinimumSize(new Dimension(0, 100));
		send_area.setBorder(BorderFactory.createLineBorder(Color.black));
		send_area.addKeyListener(new KeyListener() {
			public void keyTyped(KeyEvent e) {

			}

			public void keyPressed(KeyEvent e) {

			}

			public void keyReleased(KeyEvent e) {
				if(e.getKeyCode() == KeyEvent.VK_ENTER) {
					if(send_area.getText().length() > 0)
						model.send(send_area.getText().substring(0, send_area.getText().length()-1));
					send_area.setText("");
				}
			}
			
		});
		send_area.setEnabled(false);
		send_area.setLineWrap(true);
		this.add(send_area, gc);
		
		
		
		gc.gridx = 0;
		gc.gridy = 3;
		send_button = new JButton("Send");
		send_button.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent e) {
				model.send(send_area.getText());
				send_area.setText("");
			}
			
		});
		send_button.setEnabled(false);
		this.add(send_button, gc);
		
		this.setSize(400, 600);
		this.setVisible(true);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
	}
	
	public void update(Observable o, Object arg) {
		receive_area.setText(model.getContents());
		send_area.setEnabled(model.isConnected());
		send_button.setEnabled(model.isConnected());
		this.validate();
	}
	
	public static void main(String [] args) {
		Model model = new Model();
		model.addObserver(new Graphic(model));
	}

}
