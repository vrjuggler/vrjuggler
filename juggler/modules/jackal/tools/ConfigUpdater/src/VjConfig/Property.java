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

import java.util.*;
import java.io.StreamTokenizer;
import java.util.NoSuchElementException;
import java.io.IOException;
import VjConfig.PropertyDesc;
import VjConfig.ChunkFactory;

public class Property {

    protected int num;
    protected PropertyDesc desc;
    protected ValType valtype;
    protected List vals;
    protected ChunkDesc embeddesc; // used only for t_embeddedchunk


    /** Attempts to apply a new PropertyDesc while preserving values.
     *  THIS IS DANGEROUS and should only be called by 
     *  ConfigChunk.applyNewDesc().
     */
    public void applyNewDesc (PropertyDesc _desc) {
        desc = _desc;
        num = desc.getNumValues();
        if (valtype != desc.getValType()) {
            vals.clear();
            valtype = desc.getValType();
        }
	if (valtype == ValType.EMBEDDEDCHUNK) {
	    ChunkDesc newembeddesc = ChunkFactory.getChunkDescByToken(desc.getEnumAtIndex(0).str);
	    if (newembeddesc == null) {
		System.err.println ("Big Messup in Property Constructor!!!");
		embeddesc = new ChunkDesc ();
	    }
            if (newembeddesc != embeddesc) {
                vals.clear();
                embeddesc = newembeddesc;
            }
	}
	else {
	    embeddesc = null;
	}
        if (num != -1) {
            while (vals.size() > num)
                vals.remove(vals.size()-1);
            while (vals.size() < num)
                vals.add (createVarValue(vals.size()));
        }
    }


    public Property (Property p) {
	num = p.num;
	desc = p.desc;
	embeddesc = p.embeddesc;
	valtype = p.valtype;
	vals = new ArrayList();
        int i, n = p.vals.size();
	for (i = 0; i < n; i++)
	    vals.add (new VarValue ((VarValue)p.vals.get(i)));
    }



    public Property (PropertyDesc d) {
	desc = d;
	valtype = desc.getValType();
	num = desc.getNumValues();
	vals = new ArrayList();
	if (valtype == ValType.EMBEDDEDCHUNK) {
	    embeddesc = ChunkFactory.getChunkDescByToken(d.getEnumAtIndex(0).str);
	    if (embeddesc == null) {
		System.err.println ("Big Messup in Property Constructor!!!");
		embeddesc = new ChunkDesc ();
	    }
	}
	else {
	    embeddesc = null;
	}
	for (int i = 0; i < num; i++)
	    vals.add (createVarValue(i));

    }



    private VarValue createVarValue (int i) {
	// if i == -1, we're just tacking onto the end
	if (i == -1)
	    i = vals.size();
	if (valtype == ValType.EMBEDDEDCHUNK) {
	    ConfigChunk ch = ChunkFactory.createChunk (embeddesc);
	    if (i < desc.getValueLabelsSize())
		ch.setName (desc.getValueLabel(i));
	    else
		ch.setName (desc.getName() + " " + i);
	    return new VarValue (ch);
	}
	else
	    return new VarValue (valtype);
    }


    public final String getName () {
	return desc.getName();
    }

    public final String getToken () {
	return desc.getToken();
    }

    public final ValType getValType () {
        return desc.getValType();
    }


    public final PropertyDesc getDesc () {
        return desc;
    }


    public void clearValues () {
        if (num == -1)
            vals.clear();
    }


    public void setValue (String s, int ind) {
	if (ind < 0)
	    return;
	if (num == -1) {
	    while (ind >= vals.size())
		vals.add (createVarValue(-1));
	}
	else if (ind >= num)
	    return;
	((VarValue)vals.get(ind)).set(s);
    }

    public void setValue (boolean s, int ind) {
	if (ind < 0)
	    return;
	if (num == -1) {
	    while (ind >= vals.size())
		vals.add (createVarValue(-1));
	}
	else if (ind >= num)
	    return;
	((VarValue)vals.get(ind)).set(s);
    }


    public void setValue (VarValue s, int v) {
 	if (valtype != s.getValType()) {
            System.out.println ("Property.setValue() - " + desc.getToken() + " - type mismatch");
 	    return;
        }
	if (v < 0)
	    return;
	if (num == -1) {
	    while (v >= vals.size())
		vals.add (createVarValue(-1));
	}
	else if (v >= num)
	    return;
	((VarValue)vals.get(v)).set(s);
    }



    public VarValue getValue (int i) {
	if (i < 0 || i >= vals.size())
	    return null;
	return (VarValue)vals.get(i);
    }



    public final int getNum () {
	return vals.size();
    }


    public final boolean getHasVariableNumberOfValues () {
        return desc.getHasVariableNumberOfValues ();
    }


    public final ChunkDesc getEmbeddedDesc () {
        return embeddesc;
    }

    public boolean equals (Property p) {
	/* note - this does a total value comparison, not just checking names
	 * The exception to this is that right now I only check the name of the
	 * chunkdesc
	 */
	VarValue v1,v2;

        if (p == null)
            return false;
	if (num != p.num)
	    return false;
	if (!getToken().equals(p.getToken()))
	    return false;
	if (valtype != p.getValType())
	    return false;
	if (vals.size() != p.vals.size())
	    return false;
        int i, n = vals.size();
	for (i = 0; i < n; i++) {
	    v1 = (VarValue)vals.get(i);
	    v2 = (VarValue)p.vals.get(i);
	    if (!v1.equals(v2))
		return false;
	}
	return true;
    }



    public String toString(String pad) {
	VarValue v;
	String s = pad + getToken() + " { ";
	for (int i = 0; i < vals.size(); i++) {
	    v = (VarValue)vals.get(i);

	    if (valtype == ValType.EMBEDDEDCHUNK) {
		s += "\n" + v.getEmbeddedChunk().toString(pad + "    ");
	    }
	    else {
		s += "\"" + desc.getEnumString(v) + "\"";
	    }

	    s+= " ";
	}
	s += "}";
	return s;
    }

    public String xmlRep (String pad) {
        VarValue v;
        StringBuffer s = new StringBuffer (128);
        String tok = XMLConfigIOHandler.escapeString (getToken());
        s.append(pad);
        s.append('<');
        s.append(tok);
        s.append('>');
        int i, n = vals.size();
        for (i = 0; i < n; i++) {
            v = (VarValue)vals.get(i);
            if (valtype == ValType.EMBEDDEDCHUNK) {
                s.append('\n');
                s.append(v.getEmbeddedChunk().xmlRep (pad + "    "));
            }
            else {
                s.append('"');
                //s.append(XMLConfigIOHandler.escapeString(desc.getEnumString(v)));
                s.append(XMLConfigIOHandler.escapeString(v.toString()));
                s.append("\" ");
            }
        }
        s.append("</");
        s.append(tok);
        s.append(">\n");
        return s.toString();
    }


    /** Reads a Property using the "old" Juggler1.0 syntax.
     */
    public boolean read (ConfigStreamTokenizer st) throws IOException {
	/* the idea here is that st holds the string, less the property name
	 * which was already read by ConfigChunk.read()
	 */
	VarValue v;
	int n = 0;
	try {
	    int t = st.nextToken();
	    String token = st.sval;
	    
	    if (st.ttype == '{') {
		/* multiple entries */
		vals.clear();
		while ((v = buildVarValue (st)) != null) 
		    vals.add(v);
		if (st.ttype != '}') {
		    System.out.println ("ERROR: expected '}'");
		}
	    }
	    else {
		/* one entry */
		v = buildVarValue(st);
		vals.clear();
		vals.add (v);
	    }
	    if (num != -1) {
		/* are we a proper # of values? */
		while (vals.size() < num)
		    vals.add (new VarValue(valtype));
	    }
	    return true;
	}
	catch (IOException e) {
	    /* Either a streamtokenizer error or unexpected eof */
	    System.err.println ("Error in Property.read()");
	    return false;
	}
    }



    private VarValue buildVarValue (ConfigStreamTokenizer st) {
	/* here we build a token into an appopriate VarValue; includes
	 * type resolution and enumeration lookup 
	 */
	try {
	    st.nextToken();
	}
	catch (IOException e) {
	    return null;
	}

	if (st.ttype == '}')
	    return null;

	if (valtype == ValType.EMBEDDEDCHUNK) {
	    if (embeddesc != null) {
		ConfigChunk c = ChunkFactory.createChunk (embeddesc);
		c.read(st);
		return new VarValue (c);
	    }
	    return null;
	}
	else try {
	    return (desc.getEnumValue(st.sval));
	}
	catch (NoSuchElementException e) {
	    VarValue v = createVarValue(-1);
	    v.set(st.sval);
	    return v;
	}
    }


}



