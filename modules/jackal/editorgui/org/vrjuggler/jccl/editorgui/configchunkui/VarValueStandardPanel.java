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
 *     | Type(optional) valuegadget  removebutton(optional)|
 *     |                                                   |
 *     -----------------------------------------------------
 *
 * the value gadget is going to be either a TextArea or a multisetSelectedItem
 * with the acceptable values in it.  removebutton will be there if the
 * associated property has var numbers of values.
 */

package VjComponents.ConfigEditor.ConfigChunkUI;

import java.awt.*;
import java.util.*;
import java.awt.event.*;
import javax.swing.*;

import VjControl.Core;
import VjConfig.*;
import VjComponents.ConfigEditor.*;
import VjComponents.UI.Widgets.*;


public class VarValueStandardPanel 
    extends VarValuePanel 
    implements ActionListener {

    java.util.List              action_listeners;
    PropertyDesc      desc;
    JTextField        text;
    JComboBox         choice;   // note that only one of text,choice will be used.
    JButton           removebutton;

    ConfigUIHelper    uihelper_module;

    public VarValueStandardPanel(PropertyDesc d,
                                 ConfigUIHelper _uihelper_module) {
	super();
	desc = d;
        uihelper_module = _uihelper_module;
        action_listeners = new ArrayList();
	int i, j, k;

	setLayout(new BorderLayout (1,1));

	if (desc.valtype.equals(ValType.t_chunk)) {
	    /* we build up a choice of all chunks in the db of allowed types */
	    ConfigChunkDB db;
	    ConfigChunk ch;
	    String s;

	    choice = new JComboBox(generateChunkSelectionList(desc.getEnumerations()));
	    choice.setSelectedItem ("<No Selection>");
	    add (choice, "Center");
	}
	else if (desc.getEnumerationsSize() > 0) {
	    /* just present the enumeration names as choices */
	    choice = new JComboBox();
	    ListBoxModel bm = new ListBoxModel();
	    //bm.addObject ("<No Selection>"); may not be safe for enums
            DescEnum[] e = desc.getEnumerations();
	    for (i = 0; i < e.length; i++)
		bm.addObject(e[i].str);
	    choice.setModel(bm);
	    choice.setSelectedItem ("<No Selection>");
	    add(choice, "Center");
	}
	else if (desc.valtype.equals(ValType.t_bool)) {
	    choice = new JComboBox();
	    choice.addItem("True");
	    choice.addItem("False");
	    add (choice,"East");
	}
	else {
	    /* just add a type label & the text string */
	    add (new JLabel ("(" + desc.valtype.toString() + ")"), "West");
	    if (desc.valtype.equals(ValType.t_int))
		text = new IntegerTextField (5);
	    else if (desc.valtype.equals(ValType.t_float))
		text = new FloatTextField (5);
	    else
		text = new StringTextField (10);
	    add(text,"Center");
	}

	if (desc.num == -1) {
	    /* then it's a variable # of values */
	    removebutton = new JButton("Remove");
	    Insets in = new Insets (0,0,0,0);
	    removebutton.setMargin (in);
	    add (removebutton,"East");
	    removebutton.addActionListener(this);
	}
    }


    /** Utility for generateChunkSelectionList(). */
    private void addEmbeddedChunks (ListBoxModel bm, ConfigChunk ch, DescEnum[] chunktypes) {
        java.util.List v = ch.getEmbeddedChunks();
        ConfigChunk ch2;
        for (int i = 0; i < v.size(); i++) {
            ch2 = (ConfigChunk)v.get(i);
            if (matchesTypes (ch2, chunktypes))
                bm.addObject (ch2.getName());
            addEmbeddedChunks (bm, ch2, chunktypes);
        }
    }


    private ListBoxModel generateChunkSelectionList( DescEnum[] chunktypes) {
        int i,j;
        Vector v;
        ListBoxModel bm = new ListBoxModel();
        ConfigChunkDB db;
        ConfigChunk ch;

        ConfigModule config_module = uihelper_module.getConfigModule();

        bm.addObject ("<No Selection>");
        for (i = 0; i < config_module.chunkdbs.size(); i++) {
            db = (ConfigChunkDB)config_module.chunkdbs.elementAt(i);
            for (j = 0; j < db.size(); j++) {
                ch = db.get(j);
                if (matchesTypes(ch, chunktypes))
                    bm.addObject (db.getName() + ": " + ch.getName());
                addEmbeddedChunks (bm, ch, chunktypes);
            }
        }
        return bm;
    }


    /** Utility for generateChunkSelectionList(). */
    private boolean matchesTypes (ConfigChunk ch, DescEnum[] chunktypes) {
        int k;
        if (chunktypes == null)
            return true;
        for (k = 0; k < chunktypes.length; k++) {
            if (ch.getDescToken().equalsIgnoreCase (chunktypes[k].str)) {
                return true;
            }
        }
        return false;
    }



    public void setValue (VarValue v) {
	// sets the displayed value.
        if (choice != null) {
	    choice.setSelectedItem(desc.getEnumString(v));
        }
	else
	    /* set text in textbox */
	    text.setText(v.toString());
    }



    public VarValue getValue () {
	VarValue v = new VarValue(desc.valtype);
	if (choice != null) {
	    /* enumeration */
	    String s = desc.getEnumValue((String)choice.getSelectedItem()).toString();
	    if ((s == null) || (s.equals("<No Selection>")))
		s = "";

	    if (desc.valtype.equals (ValType.t_chunk)) {
		// remove the "filename: " prefix if it's there...
		int i = s.indexOf(": ");
		if (i != -1)
		    s = s.substring(i+2);
	    }

	    v.set(s);
	}
	else
	    v.set (text.getText());
	
	return v;
    }



    public void actionPerformed (ActionEvent e) {
	if (e.getSource() == removebutton)
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

