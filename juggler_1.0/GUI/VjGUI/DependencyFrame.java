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



// frame to display a vector of dependency information

package VjGUI;

import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Vector;
import VjConfig.*;

public class DependencyFrame extends JFrame implements ActionListener {

    ConfigChunkDB    chunkdb;
    Vector           deps;
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
	int i, j;

	gbc.anchor = gbc.NORTHWEST;
	gbc.insets = new Insets (4, 4, 4, 4);
	gbc.fill = gbc.HORIZONTAL;

	innerpanel.removeAll();
	
	for (i = 0; i < deps.size(); i++) {
	    cde = (ChunkDependEntry)deps.elementAt(i);
	    chbutton = new JButton (cde.chunk.getName());
	    chbutton.addActionListener (this);
	    chtext = new JTextArea();
	    chtext.setEditable (false);
	    chtext.setBorder (BorderFactory.createBevelBorder (BevelBorder.LOWERED));
	    for (j = 0; j < cde.propdepends.size(); j++) {
		pde = (PropDependEntry)cde.propdepends.elementAt(j);
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


    public DependencyFrame (ChunkDBPanel _par, ConfigChunkDB _db, Vector v) {
	super ("Dependencies for '" + _db.name + "'");

	chunkdb = _db;
	deps = v;
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



    public void refreshData (Vector v) {
	deps = v;
	buildInnerPanel();
    }


    public void actionPerformed (ActionEvent e) {
	String s = e.getActionCommand();
	par.openChunkFrame (s);
	//System.out.println (s);
    }


}

