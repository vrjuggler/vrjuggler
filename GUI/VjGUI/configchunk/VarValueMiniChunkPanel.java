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



/* VarValuePanel is just a little panel that we'll stick several of 
 * in a PropertyPanel.
 *
 *     -----------------------------------------------------
 *     |                                                   |
 *     | Type(optional) valuegadget  removebutton(optional)|
 *     |                                                   |
 *     -----------------------------------------------------
 *
 * the value gadget is going to be either a TextArea or a multisetSelectedItem
 * with the acceptable values in it.  removebutton will be there if the
 * associated property has var numbers of values.
 */

package VjGUI.configchunk;

// displays an entire configchunk as a miniature VarValuePanel. I hope

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.Vector;
import VjConfig.*;
import VjGUI.*;
import VjGUI.configchunk.*;
import VjGUI.util.*;

public class VarValueMiniChunkPanel extends VarValuePanel implements ActionListener, VarValuePanel.VarValuePanelParent {

    VarValuePanelParent     parent; // the parent is a listener on the remove button
    JButton           removebutton;
    ConfigChunk       chunk;
    ChunkDescDB       descdb;
    Property          prop;
    Vector panels;

    public VarValueMiniChunkPanel(VarValuePanelParent par, Property _prop, ConfigChunk _chunk) {
	super();
	parent = par;
	prop = _prop;
	chunk = new ConfigChunk (_chunk);

	setLayout(new BorderLayout (1,1));

	JPanel mainp = new JPanel();
	mainp.setLayout (new BoxLayout (mainp, BoxLayout.X_AXIS));

	VarValue v2;
	Property p;
	int i,j,k,l;

	panels = new Vector();

	for (i = 0, l=0; i < chunk.props.size(); i++) {
	    p = (Property)chunk.props.elementAt(i);
	    mainp.add (new JLabel (p.name));
	    j = p.getNum();
	    for (k = 0; k < j; k++) {
		v2 = p.getValue (j);
		VarValuePanel vp = new VarValueStandardPanel (this, p.desc);
		if (v2 != null)
		    vp.setValue (v2);
		mainp.add (vp);
		panels.addElement (vp);
	    }
	}

	add (mainp, "Center");

	if (prop.desc.num == -1) {
	    /* then it's a variable # of values */
	    removebutton = new JButton("Remove");
	    Insets in = new Insets (0,0,0,0);
	    removebutton.setMargin (in);
	    add (removebutton,"East");
	    removebutton.addActionListener(this);
	}
    }



    public void setValue (VarValue v) {
	if (v.getValType().equals (ValType.t_embeddedchunk)) {
	    chunk = new ConfigChunk (v.getEmbeddedChunk());
	    VarValue v2;
	    Property p;
	    int i,j,k,l;
	    for (i = 0, l=0; i < chunk.props.size(); i++) {
		p = (Property)chunk.props.elementAt(i);
		j = p.getNum();
		for (k = 0; k < j; k++) {
		    VarValuePanel vp = (VarValuePanel)panels.elementAt(l);
		    v2 = p.getValue (k);
		    vp.setValue (v2);
		    l++;
		}
	    }
	}
    }



    public VarValue getValue () {
	/* by the way, the fact that this works is entirely dependant on
	 * the fact that the chunks displayed by MiniChunkPanel won't have
	 * var values...
	 */
	int i,j,k,l;
	Property p;
	for (i = 0, l = 0; i < chunk.props.size(); i++) {
	    p = (Property)chunk.props.elementAt(i);
	    for (j = 0; j < p.getNum(); j++) {
		p.setValue (((VarValuePanel)panels.elementAt(l++)).getValue(),j);
	    }
	}
	return new VarValue(chunk);

    }

    public void removePanel (VarValuePanel p) {
	System.out.println ("Not Implemented");
    }

    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == removebutton)
	    parent.removePanel(this);
    }


}

