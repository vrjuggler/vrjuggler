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


package VjGUI;

import java.awt.event.*;
import java.util.Vector;
import java.io.IOException;
import VjConfig.ConfigStreamTokenizer;

public class OrgTreeElem {

    String label;     // label for this node
    Vector children;  // mixed vec. of Strings (chunkdescnames)
                      // and OrgTreeElems



    public OrgTreeElem (String _label){
        label = _label;
	children = new Vector();
    }


    // reading... we assume that the initial 'begin label' has
    // not been read or has been pushed back.
    public boolean read (ConfigStreamTokenizer st) {
        OrgTreeElem ch;

	label = "";
	children.removeAllElements();

        try {
            st.nextToken();
            if ((st.ttype != ConfigStreamTokenizer.TT_WORD) ||
		(!st.sval.equalsIgnoreCase("begin"))) {
                throw new IOException ("expected 'begin'");
	    }
            st.nextToken();
            if ((st.ttype != st.TT_WORD) && (st.ttype != '"')) {
                throw new IOException ("unexpected token '" + st.ttype + "'");
            }
            label = st.sval;
            for (;;) {
                st.nextToken();
                if ((st.ttype != st.TT_WORD) && (st.ttype != '"')) {
                    throw new IOException ("unexpected token '" + st.ttype + "'");
                }
                if (st.sval.equalsIgnoreCase ("end"))
                    break;
                else if (st.sval.equalsIgnoreCase ("begin")) {
                    st.pushBack();
                    ch = new OrgTreeElem("");
                    ch.read(st);
                    children.addElement(ch);
                }
                else
                    children.addElement(st.sval);
            }
            return true;
        }
        catch (IOException e) {
	    Core.consoleErrorMessage ("Error: OrgTree Read: " + e.getMessage());
            return false;
        }
    }



    public String toString (String pad) {
        int i;
        Object o;
        String newpad = pad + "    ";
        String s = pad + "Begin \"" + label + "\"\n";
        for (i = 0; i < children.size(); i++) {
            o = children.elementAt(i);
            if (o instanceof String)
                s += newpad + "\"" + (String)o + "\"\n";
            else if (o instanceof OrgTreeElem)
                s += ((OrgTreeElem)o).toString(newpad);
        }
        s += pad + "End\n";
        return s;
    }


}





