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


package VjComponents.ConfigEditor;

import java.awt.event.*;
import java.util.Vector;
import java.io.*;
import VjConfig.ConfigStreamTokenizer;

public class ChunkOrgTree {

    public OrgTreeElem root;
    Vector listeners; // ActionListeners - we'll send a "reload" action when we change
    String name;
    File file;

    public void copyValueFrom (ChunkOrgTree ot) {
	root = ot.root;
	name = ot.name;
	file = ot.file;
    }

    public void setRoot (OrgTreeElem _root) {
	root = _root;
	processActionEvent (new ActionEvent (this, 5, "reload"));
    }

    public void addActionListener (ActionListener l) {
	if (!listeners.contains (l))
	    listeners.addElement (l);
    }
    public void removeActionListener (ActionListener l) {
	listeners.removeElement (l);
    }

    public void processActionEvent (ActionEvent e) {
	int i;
	ActionListener l;
	for (i = 0; i < listeners.size(); i++) {
	    l = (ActionListener)listeners.elementAt(i);
	    l.actionPerformed (e);
	}
    }


    public ChunkOrgTree () {
	root = new OrgTreeElem ("Root");
	root.children.add (new OrgTreeElem ("*"));
	listeners = new Vector();
	name = "orgtree.org";
	file = new File("orgtree.org");
    }


    public void setName (String s) {
	name = s;
    }
    public void setFile (File f) {
	file = f;
    }
    public String getName () {
	return name;
    }
    public File getFile () {
	return file;
    }



    // reading... we assume that the initial 'begin label' has
    // not been read or has been pushed back.
    public boolean read (ConfigStreamTokenizer st) {
	boolean retval = root.read (st);
	processActionEvent (new ActionEvent (this, 5, "reload"));
	return retval;
    }


    // and writing
    public String toString() {
        return root.toString("");
    }


}





