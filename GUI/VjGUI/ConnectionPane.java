/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 */


package VjGUI;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class ConnectionPane extends JPanel 
    implements ActionListener, NetControlListener {

    JButton connect_button;
    JButton disconnect_button;
    JTextField hostfield;
    JTextField portfield;


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

	Core.net.addNetControlListener (this);
    }


    protected void setRemoteAddress (String _host, int port) {
	hostfield.setText (_host);
	portfield.setText (Integer.toString(port));
    }


    public void actionPerformed (ActionEvent e) {
        System.out.println ("connection panel event called");
        int i;

        if (e.getSource() == connect_button) {
            Core.net.disconnect(); // if we were connected, drop it
            int portnum = Integer.parseInt(portfield.getText());
	    Core.net.setRemoteHost (hostfield.getText(), portnum);
            Core.net.connect ();
        }
        else if (e.getSource() == disconnect_button) {
            Core.net.disconnect();
        }
    }  // actionPerformed()


    /********************* NetControlListener Stuff ******************/
    public void openedConnection (NetControlEvent e) {;}
    public void closedConnection (NetControlEvent e) {;}
    public void addressChanged (NetControlEvent e) {
	setRemoteAddress (e.host, e.port);
    }

}


