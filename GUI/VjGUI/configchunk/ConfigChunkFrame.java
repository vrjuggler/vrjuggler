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
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


/* 
 *
 * Author: Christopher Just
 *
 */

package VjGUI.configchunk;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;
import VjConfig.*;
import VjGUI.*;
//import VjGUI.configchunk.*;
import VjGUI.util.*;

public class ConfigChunkFrame extends JFrame 
    implements ActionListener, WindowListener {

    ConfigChunk   chunk;
    Vector proppanels; // property description panels.
    JPanel properties;
    JButton cancelbutton;
    JButton okbutton;
    JButton helpbutton;
    JTextField namef;
    JTextField helpfield;
    JScrollPane sp;
    JFrameParent parent;

    GridBagLayout playout;
    GridBagConstraints pconstraints;



    public ConfigChunkFrame (JFrameParent par, 
			     ConfigChunk ch) {
	super("Edit " + ch.getDescName() + ": " + ch.getName());

	Core.consoleTempMessage ("Opening ConfigChunk Window: " + ch.getName());

	//Point p = c.ui.getLocation();
	//c.ui.waitdialog.setLocation(p.x + 100,p.y + 100);
	//c.ui.waitdialog.show();

	JPanel northpanel, southpanel, centerpanel;

	proppanels = new Vector();
	PropertyPanel t;

	parent = par;
	chunk = ch;
	JPanel mainp = new JPanel();
	mainp.setBorder (new EmptyBorder (5, 5, 0, 5));
	mainp.setLayout (new BorderLayout (5,5));
	getContentPane().add (mainp);

	JPanel p1;
	northpanel = new JPanel();
	northpanel.setLayout (new BoxLayout (northpanel, BoxLayout.Y_AXIS));
	p1 = new JPanel();
	p1.setLayout (new BoxLayout (p1, BoxLayout.X_AXIS));
	p1.add (new JLabel ("Instance Name: "));
	p1.add (namef = new StringTextField (chunk.getName(), 25));
	northpanel.add (p1);
	if (!chunk.desc.help.equals ("")) {
	    helpfield = new JTextField (chunk.desc.help);
	    helpfield.setEditable (false);
	    northpanel.add (helpfield);
	}

	mainp.add(northpanel, "North");
	
	properties = new JPanel ();
	properties.setLayout (new BoxLayout (properties, BoxLayout.Y_AXIS));

	sp = new JScrollPane(properties, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
			     JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
	sp.getVerticalScrollBar().setUnitIncrement(40);
	sp.getHorizontalScrollBar().setUnitIncrement(40);

	mainp.add(sp,"Center");
	
	// make property description panels
	for (int i = 0; i < chunk.props.size(); i++) {
	    t = new PropertyPanel ((Property)chunk.props.elementAt(i), this);
	    proppanels.addElement(t);
	    properties.add(t);
	}

	southpanel = new JPanel();
	okbutton = new JButton ("  OK  ");
	okbutton.addActionListener (this);
	southpanel.add (okbutton);
	cancelbutton = new JButton ("Cancel");
	cancelbutton.addActionListener (this);
	southpanel.add (cancelbutton);
	helpbutton = new JButton ("Help");
	helpbutton.addActionListener (this);
	southpanel.add (helpbutton);
	mainp.add(southpanel, "South");

	addWindowListener(this);

	Dimension d = getPreferredSize();
	d.height = Math.min (d.height + 28, Core.screenHeight);
	d.width = Math.min (d.width + 20, Core.screenWidth);
	setSize(d);

	show();
	//setVisible(true);

// no good - you just can't tell java to relocate a window...
// 	if (Core.window_pos_kludge == true) {
// 	    Point p = getLocation();
// 	    Dimension d2 = getSize();
// 	    int y = Core.screenHeight - d2.height;
// 	    y = (y < 0)?0:y;
// 	    int x = Core.screenWidth - d2.width;
// 	    x = (x < 0)?0:x;
// 	    if (y < p.y || x < p.x) {
// 		setLocation (x,y);
// 	    }
// 	    System.out.println ("screenWidth is " + Core.screenWidth + " so I'm opening at " + x);
// 	    p = getLocation();
// 	    System.out.println ("new location is " + p);
// 	}

	Core.consoleTempMessage ("");
    }



    public String getName () {
	return chunk.getName();
    }



    public ConfigChunk getOldValue() {
	return chunk;
    }



    public ConfigChunk getValue() {
	/* returns a configchunk based on the values current 
	 * in this window */
	ConfigChunk c = ChunkFactory.createChunk (chunk.desc, namef.getText());
	//	c.setProperty ("Name", namef.getText());
// 	ConfigChunk c = new ConfigChunk (chunk.desc, Core.descdb, 
// 					 namef.getText());
	c.props.removeAllElements();
	for (int i = 0; i < chunk.props.size(); i++) {
	    c.props.addElement (((PropertyPanel)proppanels.elementAt(i)).getValue());
	}
	return c;
    }



    public void actionPerformed (ActionEvent e) {
	ConfigChunk newc;
	if (e.getSource() == cancelbutton) {
	    parent.closedChild (this, false);
	}
	else if (e.getSource() == okbutton) {
	    parent.closedChild (this, true);
	}
	else if (e.getSource() == helpbutton) {
	    Core.ui.loadDescHelp (chunk.desc.getToken());
	}
    }



    /* WindowListener Stuff */
    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
	parent.closedChild (this, false);
    }
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}
    
    
}





