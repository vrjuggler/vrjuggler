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



/* This corresponds to the config ChunkDesc class */

package VjConfig;

import java.util.Vector;
import java.util.List;
import java.io.*;
import org.w3c.dom.*;

import VjConfig.PropertyDesc;

public class ChunkDesc implements Cloneable {
    
    private List props;
    public String name;
    public String token;
    public String help;

    private ConfigChunk default_chunk;
    private Node default_node;

    public ChunkDesc (String n) {
	props = new Vector();
	addNameProperty();
	name = n;
	token = n;
	help = "";
        default_node = null;
        default_chunk = null;
    }


    public ChunkDesc () {
	props = new Vector();
	addNameProperty();
	name = "";
	token = "";
	help = "";
        default_node = null;
        default_chunk = null;
    }


    public Object clone () {
        try {
            ChunkDesc d = (ChunkDesc)super.clone();
            d.props = new Vector();
            int n = props.size();
            for (int i = 0; i < n; i++) {
                d.props.add (((PropertyDesc)props.get(i)).clone());
            }
            return d;
        }
        catch (CloneNotSupportedException e) {
            return null;
        }
    }



    public final String getName () {
	return name;
    }


    public final void setName (String _name) {
        name = _name;
    }


    public final String getToken () {
	return token;
    }


    public final void setToken (String _token) {
        token = _token;
    }

    public final String getHelp () {
        return help;
    }

    public final void setHelp (String _help) {
        help = _help;
    }


    public void setDefaultChunk (ConfigChunk ch) {
        default_chunk = ch;
    }

    public void setDefaultChunk (Node n) {
        default_node = n;
        default_chunk = null;
    }

    public ConfigChunk getDefaultChunk () {
        synchronized (this) {
            if ((default_chunk == null) && (default_node != null)) {
                XMLConfigIOHandler h = 
                    (XMLConfigIOHandler)ConfigIO.getHandler (ConfigIO.XML);
                default_chunk = h.buildConfigChunk (default_node, false);
                ConfigIO.releaseHandler(h);
            }
            return default_chunk;
        }
    }


    public void addPropertyDesc (PropertyDesc newp) {
        PropertyDesc oldp;
        int n = props.size();
        for (int i = 0; i < n; i++) {
            oldp = (PropertyDesc)props.get(i);
            if (oldp.getToken().equalsIgnoreCase (newp.getToken())) {
                props.set (i, newp);
                return;
            }
        }
        props.add (newp);
    }


    public PropertyDesc[] getPropertyDescs () {
        PropertyDesc[] p = new PropertyDesc[props.size()];
        return (PropertyDesc[])props.toArray(p);
    }

    public int propertyDescsSize() {
        return props.size();
    }

    public PropertyDesc getPropertyDesc(int i) {
        return (PropertyDesc)props.get(i);
    }

    public boolean equals (ChunkDesc c) {
	PropertyDesc p1, p2;
	if (!token.equals(c.token))
	    return false;
	
	/* This next part is O(n^2) <sigh> */
        int n = props.size();
	for (int i = 0; i < n; i++) {
	    p1 = (PropertyDesc) props.get(i);
	    p2 = c.getPropertyDesc(p1.token);
	    if ((p2 == null) || (!p1.equals(p2)))
		return false;
	}
	return true;
    }



    private void addNameProperty () {
	/* quick little function to add in a name property if it isn't already
	 * in the ChunkDesc.
	 */
	PropertyDesc named;
	named = getPropertyDesc("Name");
	if (named == null) {
	    named = new PropertyDesc();
	    named.name = named.token = "Name";
	    named.help = "Unique name of an instance of this chunk type";
	    named.valtype = new ValType("string");
	    props.add(0, named);
	}
	else
	    named.help = "Unique name of an instance of this chunk type";
    }



    public String toString() {
	String s = "chunk " + token + " \""
	    + name + "\" \"" + help + "\"\n";
	for (int i = 0; i < props.size(); i++)
	    s += "  " + ((PropertyDesc)props.get(i)).toString() + "\n";
	s += "  end\n";
	//System.out.println ("string rep is:\n" + s);
	return s;
    }



    public PropertyDesc getPropertyDesc (String tok) {
	PropertyDesc p;
        int n = props.size();
	for (int i = 0; i < n; i++) {
	    p = (PropertyDesc)props.get(i);
	    if (p.token.equalsIgnoreCase(tok))
		return p;
	}
	return null;
    }



    public boolean read (ConfigStreamTokenizer st) {
        /* the first line of the description ("chunk name") has already been
         * read. So, we need to read a series of property lines terminated
         * with an "end"
         */
        String s;
        PropertyDesc p;
        try {
            st.nextToken();
            name = st.sval;
            st.nextToken();
            help = st.sval;
            while (true) {
                st.nextToken();
                if (st.sval.equalsIgnoreCase ("end"))
                    break;
                else
                    st.pushBack();
                // else it's a property description.
                p = new PropertyDesc (st);
                if (p != null && p.name != null)
                    if (!p.name.equalsIgnoreCase("Name"))
                        props.add (p);
            }
            return true;
        }
        catch (IOException io) {
            System.err.println ("IOException in ChunkDesc read");
            return false;
        }   
    }


    public String xmlRep (String pad) {
        String newpad = pad + pad;
        StringBuffer retval = new StringBuffer(256);
        retval.append(pad);
        retval.append("<ChunkDesc token=\"");
        retval.append(XMLConfigIOHandler.escapeString(token));
        retval.append("\" name=\"");
        retval.append(XMLConfigIOHandler.escapeString(name));
        retval.append("\">\n");
        if (!help.equals ("")) {
            retval.append(newpad);
            retval.append("<help>");
            retval.append(XMLConfigIOHandler.escapeString(help));
            retval.append("</help>\n");
        }
        int n = props.size();
        for (int i = 0; i < n; i++) {
            PropertyDesc p = (PropertyDesc)props.get(i);
            retval.append(p.xmlRep(newpad));
        }
        ConfigChunk ch = getDefaultChunk();
        if (ch != null) {
            retval.append(newpad);
            retval.append("<Defaults>\n");
            retval.append(ch.xmlRep(newpad + "  "));
            retval.append(newpad);
            retval.append("</Defaults>\n");
        }
        retval.append(pad);
        retval.append("</ChunkDesc>\n");
        return retval.toString();
    }
  
}

