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

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import VjConfig.*;
import VjGUI.*;
import VjGUI.configchunk.*;
import VjGUI.util.*;

public class VarValueBigChunkPanel extends VarValuePanel implements ActionListener, JFrameParent {

    VarValuePanelParent     parent; // the parent is a listener on the remove button
    Property      prop;
    ConfigChunk  chunk;
    JButton           removebutton;
    JButton           editbutton;
    ConfigChunkFrame chunkframe;

    public VarValueBigChunkPanel(VarValuePanelParent par, Property _prop, ConfigChunk _chunk) {
	super();
	parent = par;
	prop = _prop;
	chunk = new ConfigChunk (_chunk);
	chunkframe = null;

	setLayout (new BoxLayout (this, BoxLayout.X_AXIS));

	editbutton = new JButton ("Edit Chunk");
	editbutton.addActionListener (this);
	add (editbutton);
	
		   

	if (prop.desc.num == -1) {
	    /* then it's a variable # of values */
	    removebutton = new JButton("Remove");
	    Insets in = new Insets (0,0,0,0);
	    removebutton.setMargin (in);
	    add (removebutton);
	    removebutton.addActionListener(this);
	}
    }



    public void setValue (VarValue v) {
	// sets the displayed value.
	ValType tp = v.getValType();
	if (!tp.equals (ValType.t_embeddedchunk)) {
	    Core.consoleErrorMessage ("VarValuePanel", "Unexpected type mismatch");
	    return;
	}
	if (chunkframe != null) {
	    chunkframe.dispose();
	    chunkframe = null;
	}
	chunk = new ConfigChunk (v.getEmbeddedChunk());
    }



    public VarValue getValue () {
	return new VarValue (chunk);
    }



    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == removebutton)
	    parent.removePanel(this);
	else if (e.getSource() == editbutton) {
	    if (chunkframe == null)
		chunkframe = new ConfigChunkFrame (this, chunk, null);
	    // BUG!!! that call to ConfigChunkFrame() ought to pass the
	    // chunkdb, but this panel doesn't know what it is!!!
	}
    }


    public void closedChild (ChildFrame frame, boolean ok) {
	if (chunkframe != frame) {
	    Core.consoleErrorMessage ("VarValueBigChunkPanel", 
				      "Got a closedChunkFrame for a ChunkFrame I never made!!! EEEK!");
	}
	else if (ok) {
	    chunk = chunkframe.getNewValue();
	}
	chunkframe.dispose();
	chunkframe = null;
    }

}






