/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


package VjGUI;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class ConnectionPane extends JPanel implements ActionListener {

  JButton connect_button;
  JButton disconnect_button;
  JTextField hostfield;
  JTextField portfield;


    public void setHost (String _name) {
	hostfield.setText (_name);
	Core.net.setHost (_name);
    }

    public void setPort (int _port) {
	portfield.setText (Integer.toString(_port));
	Core.net.setPort (_port);
    }


    public ConnectionPane () {
	super(new GridLayout (2, 1, 10, 10));
	JPanel p1, p2;
	
	/*    GridLayout l = new GridLayout(2,1);
	      l.setVgap(10);
	      l.setHgap(10);
	      setLayout (l);
	*/

	p1 = new JPanel();
	p2 = new JPanel();
	p1.add (new JLabel("Host Name:"));
	hostfield = new JTextField (Core.net.getHost(), 30);
	p1.add(hostfield);
	p1.add (new JLabel("Port:"));
	portfield = new JTextField (Integer.toString(Core.net.getPort()), 5);
	p1.add(portfield);
	connect_button = new JButton("Connect");
	connect_button.addActionListener(this);
	p2.add(connect_button);
	disconnect_button = new JButton ("Disconnect");
	disconnect_button.addActionListener(this);
	p2.add (disconnect_button);
	add(p1);
	add(p2);

	connect_button.setToolTipText ("Connect or reconnect to remote host");
	disconnect_button.setToolTipText ("Disconnect from remote host");
    }


    public void actionPerformed (ActionEvent e) {
        System.out.println ("connection panel event called");
        int i;

        if (e.getSource() == connect_button) {
            Core.net.disconnect(); // if we were connected, drop it
            int portnum = Integer.parseInt(portfield.getText());
            Core.net.connect(hostfield.getText(), portnum);
            //Core.net.getChunkDescs();
            //Core.net.getChunks();
        }
        else if (e.getSource() == disconnect_button) {
            Core.net.disconnect();
        }
    }  // actionPerformed()

}


