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
		chunkframe = new ConfigChunkFrame (this, chunk);
	}
    }


    public void closedChild (JFrame frame, boolean ok) {
	if (chunkframe != frame) {
	    Core.consoleErrorMessage ("VarValueBigChunkPanel", 
				      "Got a closedChunkFrame for a ChunkFrame I never made!!! EEEK!");
	}
	else if (ok) {
	    chunk = chunkframe.getValue();
	}
	chunkframe.dispose();
	chunkframe = null;
    }

}






