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



package VjConfig;

import java.util.Vector;
import java.io.DataInputStream;
import java.io.IOException;
import VjConfig.ConfigStreamTokenizer;
import VjConfig.ChunkDesc;



public class ConfigChunk {

    public Vector    props;
    public String    name;
    public ChunkDesc desc;

    public final static String embedded_separator = "->";


    public ConfigChunk (ConfigChunk c) {
	desc = c.desc;
	name = c.name;
	props = new Vector();
	for (int i = 0; i < c.props.size(); i++)
	    props.addElement (new Property ((Property)c.props.elementAt(i)));
        //validateEmbeddedChunkNames(); -- shoulda already been done to c.
    }



    public ConfigChunk (ChunkDesc d) {
	desc = d;
	name = "";
	props = new Vector();
	/* we start at one here cuz we don't include name, which is always
	 * the first element in a propertydesc
	 */
	for (int i = 1; i < d.props.size(); i++)
	    props.addElement (new Property((PropertyDesc)d.props.elementAt(i)));
        validateEmbeddedChunkNames();
    }



    public ConfigChunk (ChunkDesc d, String n) {
	desc = d;
	name = n;
	props = new Vector();
	for (int i = 1; i < d.props.size(); i++)
	    props.addElement (new Property((PropertyDesc)d.props.elementAt(i)));
        validateEmbeddedChunkNames();
    }



    public final String getName() {
	return name;
    }

    public final void setName (String s) {
        setLastNameComponent(s);
	//name = s;
        //validateEmbeddedChunkNames();
    }


    //-------- Stuff for dealing with embedded chunk names ---------------

    /** Corrects names of embedded chunks contained by this chunk.
     *  This function checks that all embedded ConfigChunks contained in
     *  self have names which accurately reflect the entire path to them
     *  (e.g. "parentchunkname->propertyname->embeddedchunkname").
     *  <p>
     *  It is called when:
     *  <br> - A ConfigChunk is read from a file (to correct legacy
     *  config files).
     *  <br> - setName() is called on a chunk.
     *  <br> - A chunk is edited by VjControl (this last is a semi-kludgey
     *  catchall because the current DefaultChunkEditorPanel doesn't carry
     *  around all the context information needed to accurately keep track
     *  of this).
     */
    public void validateEmbeddedChunkNames() {
        Property p;
        int i, k;
        ConfigChunk ch;
        for (i = 0; i < props.size(); i++) {
            p = (Property)props.elementAt(i);
            if (p.valtype.equals(ValType.t_embeddedchunk)) {
                for (k = 0; k < p.getNum(); k++) {
                    ch = p.getValue(k).getEmbeddedChunk();
                    // set name will also cause recursive validation.
                    ch.name = (name + embedded_separator + p.getToken() + embedded_separator + ch.getLastNameComponent());
                    ch.validateEmbeddedChunkNames();
                }
            }
        }
    }


    public ConfigChunk getEmbeddedChunk (String pathname) {
        // note: expects self's own name/prop pair at start of pathname.
        // note2: a nonrecursive version of this is probably worth it
        //        for the performance increase.  this is yicky.
        try {
//             if (pathname.equalsIgnoreCase (getLastNameComponent()))
//                 return this;
            String s = ConfigChunk.getNameRemainder (pathname);
            String s2 = ConfigChunk.getFirstNameComponent (s);
            Property p = getPropertyFromToken (s2);
            // s3 = embeddedchunkname->prop etc...
            String s3 = ConfigChunk.getNameRemainder (s);
            if (ConfigChunk.hasSeparator(s3)) {
                String s4 = ConfigChunk.getFirstNameComponent (s3);
                for (int i = 0; i < p.getNum(); i++) {
                    ConfigChunk ch = p.getValue(i).getEmbeddedChunk();
                    if (ch.getLastNameComponent().equalsIgnoreCase(s4))
                        return ch.getEmbeddedChunk (s3);
                }
            }
            else {
                for (int i = 0; i < p.getNum(); i++) {
                    ConfigChunk ch = p.getValue(i).getEmbeddedChunk();
                    if (ch.getLastNameComponent().equalsIgnoreCase(s3))
                        return ch;
                }

            }
            return null;
        }
        catch (Exception e) {
            return null;
        }
    }


    public static final String getLastNameComponent (String _name) {
        int i = _name.lastIndexOf (embedded_separator);
        return (i < 0)?_name:_name.substring(i+embedded_separator.length());
    }

    public final String getLastNameComponent() {
        return ConfigChunk.getLastNameComponent (name);
    }

    public final static String setLastNameComponent (String _name, String _last) {
        int i = _name.lastIndexOf (embedded_separator);
        if (i < 0)
            return _last;
        else
            return _name.substring (0, i+embedded_separator.length()) + _last;
    }

    public final void setLastNameComponent (String last) {
        name = ConfigChunk.setLastNameComponent (name, last);
        validateEmbeddedChunkNames();
    }

    public static final String getFirstNameComponent (String _name) {
        int i = _name.indexOf (embedded_separator);
        return (i < 0)?"":_name.substring(0, i);
    }

    public final String getFirstNameComponent() {
        return ConfigChunk.getFirstNameComponent (name);
    }

    /* Returns everything after the first divider in a name.
     * If there is no separator, returns name.
     */
    public final static String getNameRemainder (String _name) {
        int i = _name.indexOf (embedded_separator);
        return (i < 0)?_name:_name.substring(i+embedded_separator.length());
    }

    public final String getNameRemainder() {
        return ConfigChunk.getNameRemainder (name);
    }

    public final static boolean hasSeparator (String _name) {
        return (_name.indexOf(embedded_separator) >= 0);
    }

    public final String getDescName() {
	return desc.name;
    }

    public final String getDescToken() {
	return desc.token;
    }


    /** This is helpful for the GUI. */
    public Vector getEmbeddedChunks () {
        Vector v = new Vector();
        Property p;
        int i, j;
        for (i = 0; i < props.size(); i++) {
            p = (Property)props.elementAt(i);
            if (p.valtype.equals (ValType.t_embeddedchunk)) {
                for (j = 0; j < p.getNum(); j++) {
                    v.addElement (p.getValue(j).getEmbeddedChunk());
                }
            }
        }
        return v;
    }


    public boolean equals (ConfigChunk c) {
	Property p1, p2;
        if (c == null)
            return false;
	if (!name.equals(c.name))
	    return false;
	if (!desc.token.equals(c.desc.token))
	    return false;

	/* This next part is O(n^2) <sigh> */
	for (int i = 0; i < props.size(); i++) {
	    p1 = (Property) props.elementAt(i);
	    p2 = c.getProperty(p1.token);
	    if (!p1.equals(p2))
		return false;
	}
	return true;
    }



    public Property getProperty(String n) {
	Property p;
	for (int i = 0; i < props.size(); i++) {
	    p = (Property)props.elementAt(i);
	    if (p.name.equalsIgnoreCase(n))
		return p;
	}
	return null;
    }



    public void setPropertyFromToken (String tok, VarValue val, int i) {
	// sets the ith value in property tok to val
	Property p = getPropertyFromToken (tok);
	if (p != null)
	    p.setValue (val, i);
    }



    public Property getPropertyFromToken(String n) {
	Property p;
	for (int i = 0; i < props.size(); i++) {
	    p = (Property)props.elementAt(i);
	    if (p.desc.token.equalsIgnoreCase(n))
		return p;
	}
	return null;
    }


    public VarValue getValueFromToken (String n, int i) {
        Property p = getPropertyFromToken (n);
        if (p != null)
            return p.getValue(i);
        else
            return null;
    }


    /** Returns a vector of ConfigChunk names this chunk depends on */
    public Vector getDependencyNames() {
	int j, k;
	ConfigChunk ch2;
	Property p;
	String s;
	VarValue val;
        Vector results = new Vector();

	for (j = 0; j < props.size(); j++) {
	    p = (Property)props.elementAt(j);
	    if (p.valtype.equals (ValType.t_chunk)) {
		for (k = 0; k < p.vals.size(); k++) {
		    s = ((VarValue)p.vals.elementAt(k)).getString();
                    if (!s.equals(""))
                        results.addElement (s);
                }
	    }
	    else if (p.valtype.equals (ValType.t_embeddedchunk)) {
		for (k = 0; k < p.vals.size(); k++) {
		    ch2 = ((VarValue)p.vals.elementAt(k)).getEmbeddedChunk();
                    Vector results2 = ch2.getDependencyNames();
                    results.addAll (results2);
		}
	    }
	}
	return results;
    }



    public boolean read (ConfigStreamTokenizer st) {
	try {
	    while (true) {
		st.nextToken();
		if (st.ttype == ConfigStreamTokenizer.TT_EOF)
		    break;
		if ((st.ttype != ConfigStreamTokenizer.TT_WORD) && 
		    (st.ttype != '"'))
		    continue;
		if (st.sval.equalsIgnoreCase("end"))
		    break;

		/* begin reading property info out of s */
		if (st.sval.equalsIgnoreCase("name")) {
		    st.nextToken();
		    if (st.ttype == '{') {
			st.nextToken();
			name = st.sval;
			st.nextToken();
		    }
		    else
			name = st.sval;
		}
		else {
		    Property p = getPropertyFromToken(st.sval);
		    if (p != null) {
			p.read(st);
		    }
		    else {
			System.err.println ("Error: ConfigChunk.Read() - no such property "
					    + st.sval);
		    }
		}
		/* finished reading property info from s */
	    }
            //validateEmbeddedChunkNames();
	    return true;
	}
	catch (IOException io) {
	    System.err.println ("IO Error in ConfigChunk.read()");
	    return false;
	}
    }  



    public final String toString () {
	return toString ("");
    }



    public String toString (String pad) {
	String s = pad + desc.token + "\n";
	s += pad + "  Name \"" + name + "\"\n";
	for (int i = 0; i < props.size(); i++)
	    s += ((Property)props.elementAt(i)).toString(pad + "  ") + "\n";
	s += pad + "  end\n";
	return s;
    }


}




