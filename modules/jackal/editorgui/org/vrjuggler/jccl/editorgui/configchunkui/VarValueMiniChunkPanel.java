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


package VjComponents.ConfigEditor.ConfigChunkUI;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;

import VjConfig.*;
import VjComponents.ConfigEditor.*;
import VjComponents.UI.Widgets.*;

/** Panel for displaying an embedded chunk varvalue "inline".
 *  This panel tries to display a small embedded ConfigChunk with all its
 *  properties on a single horizontal line.  For obvious reasons, this can
 *  only work with a fairly small ConfigChunk - one with only a few 
 *  properties.  Its algorithms fail completely if the embedded chunk has
 *  any properties with variable numbers of values.
 *
 * @author Christopher Just
 * @version $Revision$
 */
public class VarValueMiniChunkPanel 
    extends VarValuePanel 
    implements ActionListener {


    protected java.util.List    action_listeners;
    protected JButton           remove_button;
    protected ConfigChunk       chunk;
    protected PropertyDesc         desc;
    protected Vector            panels;
    protected ConfigUIHelper    uihelper_module;


    public VarValueMiniChunkPanel () {
	super();

        action_listeners = new ArrayList();

        setLayout (new BoxLayout (this, BoxLayout.X_AXIS));

	panels = new Vector();
    }



    public void setConfigUIHelper (ConfigUIHelper helper) {
        if (uihelper_module == null) {
            uihelper_module = helper;
            int i, n = panels.size();
            for (i = 0; i < n; i++)
                ((VarValuePanel)panels.get(i)).setConfigUIHelper (helper);
        }
    }


    public void setPropertyDesc (PropertyDesc _desc) {
        if (desc == null) {
            desc = _desc;

	    if (desc.getHasVariableNumberOfValues()) {
                remove_button = new JButton("Remove");
                Insets in = new Insets (0,0,0,0);
                remove_button.setMargin (in);
                //add (remove_button);
                remove_button.addActionListener(this);
            }
        }
    }


    public void setValue (VarValue v) {
	if (v.getValType() == ValType.EMBEDDEDCHUNK) {
            if (chunk == null) {
                // first-time assignment, build UI
                chunk = new ConfigChunk (v.getEmbeddedChunk());
                VarValue v2;
                Property p;
                int i,j,k,l;
                
                int n = chunk.getPropertiesSize();
                for (i = 0, l=0; i < n; i++) {
                    p = chunk.getProperty(i);
                    add (new JLabel (p.getName(), JLabel.RIGHT));
                    j = p.getNum();
                    for (k = 0; k < j; k++) {
                        v2 = p.getValue (k);
                        VarValuePanel vp = new VarValueStandardPanel ();
                        vp.setConfigUIHelper (uihelper_module);
                        vp.setPropertyDesc (p.getDesc());
                        if (v2 != null)
                            vp.setValue (v2);
                        add (vp);
                        panels.add (vp);
                    }
                }

                if (remove_button != null)
                    add (remove_button);
            }
            else {
                // refresh values
                chunk = new ConfigChunk (v.getEmbeddedChunk());
                VarValue v2;
                Property p;
                int i,j,k,l;
                int n = chunk.getPropertiesSize();
                for (i = 0, l=0; i < n; i++) {
                    p = chunk.getProperty(i);
                    j = p.getNum();
                    for (k = 0; k < j; k++) {
                        VarValuePanel vp = (VarValuePanel)panels.get(l);
                        v2 = p.getValue (k);
                        vp.setValue (v2);
                        l++;
                    }
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
        int n = chunk.getPropertiesSize();
	for (i = 0, l = 0; i < n; i++) {
	    p = chunk.getProperty(i);
	    for (j = 0; j < p.getNum(); j++) {
		p.setValue (((VarValuePanel)panels.get(l++)).getValue(),j);
	    }
	}
	return new VarValue(chunk);

    }


    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == remove_button)
	    notifyActionListenersRemove();
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



}

