/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/


/*
 *
 * A window for viewing/editing a single ChunkDesc
 *
 * Author: Christopher Just
 *
 */

package VjGUI.chunkdesc;

import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import javax.swing.*;
import javax.swing.border.*;
import VjConfig.*;
import VjGUI.chunkdesc.*;
import VjGUI.Core;
import VjGUI.util.*;

public class ChunkDescFrame 
    extends JFrame 
    implements ChildFrame, ActionListener, WindowListener { 

    JFrameParent parent;
    ChunkDesc desc;
    ChunkDescDB descdb;
    Vector proppanels; // property description panels.
    JPanel properties;
    JTextField namefield;
    JTextField tokenfield;
    JTextField helpfield;
    JButton removebutton;
    JButton insertbutton;
    JButton cancelbutton;
    JButton okbutton;
    boolean editable;
    JPanel mainpanel, buttonspanel;


    public ChunkDescFrame (JFrameParent p, 
			   ChunkDesc _desc,
			   ChunkDescDB _descdb,
			   boolean _editable) {
	super("Edit ChunkDesc: "+_desc.name);

	JScrollPane sp;
	PropertyDescPanel t;
	
	parent = p;
	editable = _editable;
	desc = _desc;
	descdb = _descdb;
	proppanels = new Vector();
	
	//setFont(core.ui.windowfont);
	
	/******************* North Panel **********************/
	mainpanel = new JPanel();
	getContentPane().add(mainpanel);
	
	mainpanel.setBorder (BorderFactory.createEmptyBorder (3,3,3,3));

	JLabel l;
	GridBagLayout gbl;
	GridBagConstraints gbc = new GridBagConstraints();
	mainpanel.setLayout (gbl = new GridBagLayout());
	
	gbc.fill = gbc.HORIZONTAL;
	gbc.insets = new Insets (2,4,2,4);
	gbc.gridwidth = 1;
	
	l = new JLabel ("ChunkDesc Name: ", JLabel.RIGHT);
	gbl.setConstraints (l, gbc);
	mainpanel.add (l);
	
	namefield = new StringTextField (desc.name, 15);
	gbc.weightx = 0.5;
	gbl.setConstraints (namefield, gbc);
	mainpanel.add (namefield);

	l = new JLabel ("Token: ", JLabel.RIGHT);
	gbc.weightx = 0;
	gbl.setConstraints (l, gbc);
	mainpanel.add (l);

	tokenfield = new NoSpaceTextField (desc.token, 15);
	gbc.gridwidth = gbc.REMAINDER;
	gbc.weightx = 0.5;
	gbl.setConstraints (tokenfield, gbc);
	mainpanel.add (tokenfield);

	l = new JLabel ("Help String: ", JLabel.RIGHT);
	gbc.gridwidth = 1;
	gbc.weightx = 0;
	gbl.setConstraints (l, gbc);
	mainpanel.add (l);

	helpfield = new StringTextField (desc.help, 50);
	gbc.weightx = 1;
	gbc.gridwidth = gbc.REMAINDER;
	gbl.setConstraints (helpfield, gbc);
	mainpanel.add (helpfield);


	/******************* Center Panel ********************/
	properties = new JPanel ();
	sp = new JScrollPane(properties, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
			     JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        sp.getVerticalScrollBar().setUnitIncrement(40);
        sp.getHorizontalScrollBar().setUnitIncrement(40);

	gbc.weighty = 1;
	gbc.weightx = 1;
	gbc.fill = gbc.BOTH;
	gbc.gridheight = gbc.RELATIVE;
	gbl.setConstraints (sp, gbc);
	mainpanel.add(sp);
	
	properties.setLayout (new BoxLayout (properties, BoxLayout.Y_AXIS));

	// make property description panels
	PropertyDesc prop;
	for (int i = 0; i < desc.props.size(); i++) {
	    prop = (PropertyDesc)desc.props.elementAt(i);
	    t = new PropertyDescPanel (this, 
				       !prop.name.equalsIgnoreCase("name") && editable,
				       prop);
	    proppanels.addElement(t);
	    properties.add(t);
	}

	/****************** South Panel ***********************/

	if (editable) {
	    buttonspanel = new JPanel();
	    insertbutton = new JButton ("Insert");
	    insertbutton.addActionListener (this);
	    buttonspanel.add (insertbutton);
	    removebutton = new JButton ("Remove");
	    removebutton.addActionListener (this);
	    buttonspanel.add (removebutton);
	    okbutton = new JButton ("  OK  ");
	    okbutton.addActionListener (this);
	    buttonspanel.add (okbutton);
	    cancelbutton = new JButton ("Cancel");
	    cancelbutton.addActionListener (this);
	    buttonspanel.add (cancelbutton);
	}
	else { /* not editable */
	    buttonspanel = new JPanel();
	    cancelbutton = new JButton ("Close");
	    cancelbutton.addActionListener (this);
	    buttonspanel.add (cancelbutton);
	}
	
	gbc.weighty = 0;
	gbc.fill = gbc.HORIZONTAL;
	gbc.gridwidth = 1;
	gbc.gridheight = gbc.REMAINDER;
	gbc.gridwidth = gbc.REMAINDER;
	gbl.setConstraints (buttonspanel, gbc);
	mainpanel.add (buttonspanel);
	
	addWindowListener (this);

	Dimension d = getPreferredSize();
	d.height = Math.min (d.height + 28, Core.screenHeight);
	d.width = Math.min (d.width, Core.screenWidth);
	setSize(d);
	//properties.setSize (properties.getMinimumSize());

        setVisible(true);

    }



    public ChunkDescDB getDescDB() {
	return descdb;
    }


    public void closeFrame(boolean ok) {
	/* for each property panel, call closeEnums() */
	for (int i = 0; i < proppanels.size(); i++) {
	    ((PropertyDescPanel)proppanels.elementAt(i)).closeFrames();
	}
	parent.closedChild(this, ok);
	/* tell our parent ChunkDescDBPanel we're going away */
    }



    public String getName () {
	return desc.name;
    }



    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == removebutton) {
	    for (int i = 0; i < proppanels.size(); ) {
		PropertyDescPanel p = 
		    (PropertyDescPanel)proppanels.elementAt(i);
		if (p.getSelected()) {
		    properties.remove(p);
		    proppanels.removeElementAt(i);
		}
		else
		    i++;
	    }
            // validate & repaint needed to get components to "shift up"
            // after removing one in the middle.
            validate();
	    repaint();
	}
	if (e.getSource() == insertbutton) {
	    PropertyDescPanel t = new PropertyDescPanel (this, true);
	    proppanels.addElement(t);
	    properties.add(t); 
	    //setReasonableSize();
	    validate();
	}
	if (e.getSource() == okbutton) {
	  closeFrame (true);
	}
	if (e.getSource() == cancelbutton) {
	  closeFrame (false);
	}
    }



    public ChunkDesc getOldValue() {
	return desc;
    }



    public ChunkDesc getNewValue() {
	// returns the value currently shown in the panel...
	if (tokenfield.getText().equals("")) {
	    /* this really oughtta bring up a dialog box */
	    System.err.println ("Error: Must fill in ChunkDesc token field");
	    return null;
	}
	    
	ChunkDesc d = new ChunkDesc();
	d.token = tokenfield.getText();
	d.help = helpfield.getText();
	d.name = namefield.getText();
	if (d.name.equals(""))
	    d.name = d.token;

	/* start at one to skip instance name field which is already there */
	for (int i = 1; i < proppanels.size(); i++) {
	    PropertyDesc p = 
		((PropertyDescPanel)proppanels.elementAt(i)).getPropertyDesc();
	    if (p != null)
		d.props.addElement(p);
	}
	
	return d;
    }
    

    /* WindowListener Stuff */
    public void windowActivated(WindowEvent e) {}
    public void windowClosed(WindowEvent e) {}
    public void windowClosing(WindowEvent e) {
	closeFrame (false);
    }
    public void windowDeactivated(WindowEvent e) {}
    public void windowDeiconified(WindowEvent e) {}
    public void windowIconified(WindowEvent e) {}
    public void windowOpened(WindowEvent e) {}


    /***************** ChildFrame Stuff ***************************/

    public void destroy () {
	dispose();
    }


    public boolean matches (String cl, Object db, Object o) {
	if (cl != null) {
	    try {
		if (!(Class.forName(cl).isInstance(this)))
		    return false;
	    }
	    catch (Exception e) {
		return false;
	    }
	}
	if (db != descdb)
	    return false;
	return (o == null) || (o == desc);
    }


    public void updateUI () {
	SwingUtilities.updateComponentTreeUI (this);
    }


}



