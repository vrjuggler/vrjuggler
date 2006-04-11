/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

import java.util.ArrayList;
import java.util.List;
import java.io.DataInputStream;
import java.io.IOException;
import VjConfig.ConfigStreamTokenizer;
import VjConfig.ChunkDesc;



/** A ConfigChunk is a logical collection of configuration information.
 *  A vjConfigChunk stores a number of vjPropertys that describe
 *  the configuration of a particular component of the system.
 *  It has an associated vjChunkDesc which describes its type
 *  and the vjPropertys that belong to it.
 *
 *  @see ChunkDesc
 *  @author Christopher Just
 *  @version $Revision$
 */
public class ConfigChunk {

    protected ArrayList props;
    protected String    name;
    protected ChunkDesc desc = null;

    public final static String embedded_separator = "/";


    public ConfigChunk (ConfigChunk ch) {
        init (null, ch);
    }
    public ConfigChunk (ChunkDesc d) {
        init (d, null);
    }

    /** Initializes a new ConfigChunk - utility for Constructor methods.
     *  If ch is non-null, self's value is copied from ch.  Otherwise it's
     *  built up from the ChunkDesc d.
     *  At least one of the arguments must be non-null.
     */
    private void init (ChunkDesc d, ConfigChunk ch) {
        props = new ArrayList();
        int i, n;
        if (ch != null) {
            desc = ch.desc;
            name = ch.name;
            n = ch.props.size();
            for (i = 0; i < n; i++)
                props.add (new Property ((Property)ch.props.get(i)));
            //validateEmbeddedChunkNames(); -- shoulda already been done to c.
        }
        else {
            desc = d;
            name = "";
            /* we start at one here cuz we don't include name, which is always
             * the first element in a propertydesc
             */
            n = d.propertyDescsSize();
            for (i = 1; i < n; i++)
                props.add (new Property(d.getPropertyDesc(i)));
            validateEmbeddedChunkNames();
        }
    }

    /** Applies a new ChunkDesc to self, attempting to preserve its values.
     *  THIS IS VERY DANGEROUS.  It is designed for, and should only be used
     *  by, the ChunkDesc editor panel to update a defaults chunk.
     */
    public void applyNewDesc (ChunkDesc d) {
        PropertyDesc pd;
        Property p;
        desc = d;
        ArrayList newprops = new ArrayList();
        int i, n = desc.propertyDescsSize();
        for (i = 1; i <n; i++) {
            pd = desc.getPropertyDesc(i);
            p = getPropertyFromToken (pd.getToken());
            if (p != null) {
                p.applyNewDesc (pd);
                newprops.add(p);
            }
            else
                newprops.add (new Property(pd));
        }
        props = newprops;
    }


    public final String getName() {
	return name;
    }

    public final void setName (String s) {
        setLastNameComponent(s);
	//name = s;
        //validateEmbeddedChunkNames();
    }

    public final void setFullName (String s) {
        name = s;
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
            p = (Property)props.get(i);
            if (p.valtype == ValType.EMBEDDEDCHUNK) {
                for (k = 0; k < p.getNum(); k++) {
                    ch = p.getValue(k).getEmbeddedChunk();
                    // set name will also cause recursive validation.
                    ch.name = ch.getLastNameComponent();
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


    public final ChunkDesc getDesc() {
        return desc;
    }

    public final String getDescName() {
	return desc.getName();
    }


    public final String getDescToken() {
	return desc.getToken();
    }

    public final String getDescHelp() {
        return desc.getHelp();
    }


    /** This is helpful for the GUI. */
    public List getEmbeddedChunks () {
        ArrayList v = new ArrayList();
        Property p;
        int i, j;
        for (i = 0; i < props.size(); i++) {
            p = (Property)props.get(i);
            if (p.valtype == ValType.EMBEDDEDCHUNK) {
                for (j = 0; j < p.getNum(); j++) {
                    v.add (p.getValue(j).getEmbeddedChunk());
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
	    p1 = (Property) props.get(i);
	    p2 = c.getProperty(p1.getToken());
	    if (!p1.equals(p2))
		return false;
	}
	return true;
    }


    public int getPropertiesSize() {
        return props.size();
    }


    public Property getProperty (int i) {
        return (Property)props.get(i);
    }


    public Property getProperty(String n) {
	Property p;
	for (int i = 0; i < props.size(); i++) {
	    p = (Property)props.get(i);
	    if (p.getName().equalsIgnoreCase(n))
		return p;
	}
	return null;
    }



    public boolean setPropertyFromToken (String tok, VarValue val, int i) {
	// sets the ith value in property tok to val
	Property p = getPropertyFromToken (tok);
	if (p != null) {
	    p.setValue (val, i);
            return true;
        }
        else {
            return false;
        }
    }


    public boolean setProperty (Property p) {
        // replaces a property with p's token w/ p itself.
        int i, n = props.size();
        Property p2;
        for (i = 0; i < n; i++) {
            p2 = (Property)props.get(i);
            if (p2.getToken().equalsIgnoreCase (p.getToken())) {
                props.set (i, p);
                return true;
            }
        }
        return false;
    }


    public Property getPropertyFromToken(String _token) {
	Property p;
        int i, n = props.size();
	for (i = 0; i < n; i++) {
	    p = (Property)props.get(i);
	    if (p.getToken().equalsIgnoreCase(_token))
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
    public List getDependencyNames() {
	int j, k;
	ConfigChunk ch2;
	Property p;
	String s;
	VarValue val;
        ArrayList results = new ArrayList();

	for (j = 0; j < props.size(); j++) {
	    p = (Property)props.get(j);
	    if (p.valtype == ValType.CHUNK) {
		for (k = 0; k < p.vals.size(); k++) {
		    s = ((VarValue)p.vals.get(k)).getString();
                    if (!s.equals(""))
                        results.add (s);
                }
	    }
	    else if (p.valtype == ValType.EMBEDDEDCHUNK) {
		for (k = 0; k < p.vals.size(); k++) {
		    ch2 = ((VarValue)p.vals.get(k)).getEmbeddedChunk();
                    List results2 = ch2.getDependencyNames();
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
	    s += ((Property)props.get(i)).toString(pad + "  ") + "\n";
	s += pad + "  end\n";
	return s;
    }


    public String xmlRep () {
        return xmlRep ("");
    }

    public String xmlRep (String pad) {
        StringBuffer s = new StringBuffer (256);
        s.append(pad);
        s.append('<');
        s.append(XMLConfigIOHandler.escapeString(desc.token));
        s.append(" name=\"");
        s.append(XMLConfigIOHandler.escapeString(name));
        s.append("\">\n");

        // careful - rem that name isn't a property in this implementation.
        for (int i = 0; i < props.size(); i++)
            s.append (((Property)props.get(i)).xmlRep(pad + "  "));
        s.append(pad);
        s.append("</");
        s.append(XMLConfigIOHandler.escapeString(desc.token));
        s.append(">\n");
        return s.toString();
    }


}

