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



// frame to display a vector of dependency information
// NOTE: A whole bunch of things could be done with the new event system
// to autoupdate the dependency data.  The only question is if we really
// want to be listening to change notification on every open chunkdb.
// Given that it would only be an issue when the depframe is open, this
// might not be a bad idea.

package VjComponents.ConfigEditor;

import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.util.List;

import VjConfig.*;
import VjComponents.UI.Widgets.ChildFrame;



public class DependencyFrame extends JFrame 
    implements ChildFrame, ActionListener {

    ConfigChunkDB    chunkdb;
    List             deps;
    ChunkDBPanel     par;
    JTextArea        textarea;
    JPanel           mainpanel;
    JPanel           innerpanel;
    GridBagLayout    gblayout;


    private void buildInnerPanel() {
	ChunkDependEntry cde;
	PropDependEntry pde;
	JButton chbutton;
	JTextArea chtext;
	JPanel panel;
	GridBagConstraints gbc = new GridBagConstraints();
	int i, j, n, m;

	gbc.anchor = gbc.NORTHWEST;
	gbc.insets = new Insets (4, 4, 4, 4);
	gbc.fill = gbc.HORIZONTAL;

	innerpanel.removeAll();
	
        n = deps.size();
	for (i = 0; i < n; i++) {
	    cde = (ChunkDependEntry)deps.get(i);
	    chbutton = new JButton (cde.chunk.getName());
	    chbutton.addActionListener (this);
	    chtext = new JTextArea();
	    chtext.setEditable (false);
	    chtext.setBorder (BorderFactory.createBevelBorder (BevelBorder.LOWERED));
            m = cde.propdepends.size();
	    for (j = 0; j < m; j++) {
		pde = (PropDependEntry)cde.propdepends.get(j);
		chtext.append (pde.toString());
	    }
	    gbc.gridwidth = 1;
	    gblayout.setConstraints (chbutton, gbc);
	    innerpanel.add (chbutton);
	    gbc.gridwidth = gbc.REMAINDER;
	    gblayout.setConstraints (chtext, gbc);
	    innerpanel.add (chtext);

	}

	// without this repaint, we'll sometimes get leftovers of removed
	// components.
	innerpanel.repaint();


    }


    public DependencyFrame (ChunkDBPanel _par, ConfigChunkDB _db, List _deps) {
	super ("Dependencies for '" + _db.name + "'");

	chunkdb = _db;
	deps = _deps;
	par = _par;

	mainpanel = new JPanel();
	getContentPane().add(mainpanel);
	mainpanel.setLayout (new BorderLayout(5,5));
	mainpanel.setBorder (BorderFactory.createEmptyBorder (5,5,5,5));

	mainpanel.add (new JLabel ("The Following External Dependencies were found:"), "North");
	innerpanel = new JPanel();
	mainpanel.add (new JScrollPane (innerpanel, JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS), "Center");
	innerpanel.setLayout (gblayout = new GridBagLayout());

	buildInnerPanel();

	int y = 90 + innerpanel.getPreferredSize().height;
	int x = 40 + innerpanel.getPreferredSize().width;
	y = (y < 350)?350:y;
	y = (y > 500)?500:y;
	x = (x < 350)?350:x;
	setSize (x, y);
    }



    public void refreshData (List v) {
	deps = v;
	buildInnerPanel();
    }


    public void actionPerformed (ActionEvent e) {
	String s = e.getActionCommand();
	par.openChunkFrame (s);
	//System.out.println (s);
    }



    /********************** ChildFrameStuff ************************/

    public void destroy () {
        dispose ();
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
	if (chunkdb != db)
	    return false;
        return true;
    }


    public void updateUI () {
	SwingUtilities.updateComponentTreeUI (this);
    }

}

