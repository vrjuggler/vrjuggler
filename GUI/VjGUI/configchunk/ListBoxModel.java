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



package VjGUI.configchunk;

import javax.swing.*;
import javax.swing.event.*;
import java.util.Vector;

public class ListBoxModel implements ComboBoxModel  {

    Vector v;
    Object selected;

    ListBoxModel() {
	v = new Vector();
	selected = null;
    }

    public void addObject (Object a) {
	v.addElement(a);
    }

    public Object getSelectedItem() {
	return selected;
    }

    public void setSelectedItem (Object s) {
	if (((String)s).equals(""))
	    selected = "<No Selection>";
	else
	    selected = s;
    }
    
    public void addListDataListener (ListDataListener l) {
	//System.out.println ("addListDataListener not implemented yet");
    }

    public Object getElementAt (int i) {
	return v.elementAt(i);
    }

    public int getSize() {
	return v.size();
    }
    
    public void removeListDataListener (ListDataListener l) {
    }

}

