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



/* VarValuePanel is just a little panel that we'll stick several of 
 * in a PropertyPanel.
 *
 *     -----------------------------------------------------
 *     |                                                   |
 *     | Type(optional) valuegadget  remove_button(optional)|
 *     |                                                   |
 *     -----------------------------------------------------
 *
 * the value gadget is going to be either a TextArea or a multisetSelectedItem
 * with the acceptable values in it.  remove_button will be there if the
 * associated property has var numbers of values.
 */

package VjComponents.ConfigEditor.ConfigChunkUI;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;

import VjControl.Core;
import VjConfig.*;
import VjComponents.ConfigEditor.*;
import VjComponents.UI.Widgets.*;


public class VarValueBigChunkPanel 
    extends VarValuePanel 
    implements ActionListener {


    protected Property           prop;
    protected ConfigChunk        chunk;
    protected JButton            remove_button;
    protected JButton            edit_button;
    protected GenericEditorFrame chunkframe;
    protected ConfigUIHelper     uihelper_module;
    protected java.util.List     action_listeners;


    public VarValueBigChunkPanel(Property _prop, 
                                 ConfigChunk _chunk, 
                                 ConfigUIHelper _uihelper_module) {
	super();
	prop = _prop;
	chunk = new ConfigChunk (_chunk);
	chunkframe = null;
        uihelper_module = _uihelper_module;
        action_listeners = new ArrayList();

	//setLayout (new BoxLayout (this, BoxLayout.X_AXIS));
        setLayout (new GridLayout (1, 2));

        Insets in = new Insets (0,0,0,0);

	edit_button = new JButton ("Edit");
        edit_button.setMargin (in);
	edit_button.addActionListener (this);
	add (edit_button);

	if (prop.getHasVariableNumberOfValues()) {
	    /* then it's a variable # of values */
	    remove_button = new JButton("Remove");
	    remove_button.setMargin (in);
	    add (remove_button);
	    remove_button.addActionListener(this);
	}
    }



    public void setValue (VarValue v) {
	// sets the displayed value.
	ValType tp = v.getValType();
	if (tp != ValType.EMBEDDEDCHUNK) {
	    Core.consoleErrorMessage ("VarValuePanel", "Unexpected type mismatch");
	    return;
	}
	if (chunkframe != null) {
            chunkframe.destroy();
	    chunkframe.dispose();
	    chunkframe = null;
	}
	chunk = new ConfigChunk (v.getEmbeddedChunk());

        //edit_button.setText ("Edit " + chunk.getLastNameComponent());
    }



    public VarValue getValue () {
	return new VarValue (chunk);
    }



    public void actionPerformed (ActionEvent e) {
        Object source = e.getSource();
	if (source == remove_button)
            notifyActionListenersRemove();
	else if (source == edit_button) {
	    if (chunkframe == null) {
                ConfigChunkPanel p = uihelper_module.configchunkpanel_factory.createConfigChunkPanel (chunk.getDescToken());
                p.setChunk (chunk, null);
                chunkframe = new GenericEditorFrame (p);
                chunkframe.addActionListener (this);
                //ui_module.addChildFrame (f);
            }
            else
                chunkframe.show();
	    // BUG!!! that call to p.setChunk ought to pass the
	    // chunkdb, but this panel doesn't know what it is!!!
	}
        else if (source instanceof GenericEditorFrame) {
            if (e.getActionCommand().equals ("Close")) {
                chunkframe.destroy();
                chunkframe.dispose();
                chunkframe = null;
            }
            else if (e.getActionCommand().equals ("Apply")) {
                ConfigChunkPanel p = 
                    (ConfigChunkPanel)chunkframe.getEditorPanel();
                chunk = p.getNewValue();
            }
        }
    }


    //--------------------- ActionEvent Stuff ------------------------

    public void addActionListener (ActionListener l) {
	synchronized (action_listeners) {
	    action_listeners.add (l);
	}
    }

    public void removeActionListener (ActionListener l) {
	synchronized (action_listeners) {
	    action_listeners.remove (l);
	}
    }

    private void notifyActionListenersRemove () {
        ActionEvent e = new ActionEvent (this, ActionEvent.ACTION_PERFORMED,
                                         "Remove");
        ActionListener l;
        int i, n;
        synchronized (action_listeners) {
            n = action_listeners.size();
            for (i = 0; i < n; i++) {
                l = (ActionListener)action_listeners.get(i);
                l.actionPerformed (e);
            }
        }
    }


//     /******************** JFrameParent Stuff *****************************/

//     public void closeChild (ChildFrame frame) {
// 	if (chunkframe != frame) {
// 	    Core.consoleErrorMessage ("VarValueBigChunkPanel", 
// 				      "Got a closedChunkFrame for a ChunkFrame I never made!!! EEEK!");
// 	}
//         chunkframe.destroy();
// 	chunkframe.dispose();
// 	chunkframe = null;
//     }


//     public void applyChild (ChildFrame frame) {
//         ConfigChunkPanel p = (ConfigChunkPanel)chunkframe.getEditorPanel();
//         chunk = p.getNewValue();
//         //edit_button.setText ("Edit " + chunk.getLastNameComponent());
//     }

}






