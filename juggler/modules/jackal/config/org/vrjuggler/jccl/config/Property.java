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

/** A single property of a ConfigChunk.
 *  A property can have 0 or more values, all of the same type.
 *  Each property has an associated PropertyDesc which defines its
 *  name, type, labels for individual values, etc.
 *
 *  @see PropertyDesc
 *  @version $Revision$, $Date$
 */
public class Property {

    protected PropertyDesc desc;
    protected int num;             // cached from desc
    protected ValType valtype;     // cached from desc
    protected List vals;
    protected ChunkDesc embeddesc; // used only for t_embeddedchunk



    /** Constructor. */
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
        {
            appendVarValue();
        }
    }

    /** Copy constructor. */
    public Property (Property p) {
        num = p.num;
        desc = p.desc;
        embeddesc = p.embeddesc;
        valtype = p.valtype;
        vals = new ArrayList();
        int i, n = p.vals.size();
        for (i = 0; i < n; i++)
        {
            vals.add (new VarValue ((VarValue)p.vals.get(i)));
        }
    }

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
            {
                vals.remove(vals.size()-1);
            }
            while (vals.size() < num)
            {
                appendVarValue();
            }
        }
    }

    /** Adds a single value to self. 
     *  The new value is appended to the end of the values array.
     *  If the new value is an embedded chunk, it is given a reasonable
     *  instance name.
     *  This is a utility function used by various other methods to 
     *  create the values vector.  As such, it does not itself do error
     *  checking on the value size or mutability.
     */
    private void appendVarValue () {
        int i = vals.size();
        if (valtype == ValType.EMBEDDEDCHUNK) {
            ConfigChunk ch = ChunkFactory.createChunk (embeddesc);
            if (i < desc.getValueLabelsSize())
            {
                ch.setName (desc.getValueLabel(i));
            }
            else
            {
                ch.setName (desc.getName() + " " + i);
            }
            vals.add (new VarValue (ch));
        }
        else
        {
            vals.add (new VarValue (valtype));
        }
    }

    /** Returns the name of this property.
     *  (This is the same value as the name of self's PropertyDesc).
     */
    public final String getName () {
        return desc.getName();
    }



    /** Returns the token of this property.
     *  (This is the same value as the token of self's PropertyDesc).
     */
    public final String getToken () {
        return desc.getToken();
    }



    /** Returns the type of values stored in self.
     */
    public final ValType getValType () {
        return desc.getValType();
    }



    /** Returns the ChunkDesc used to describe self.
     */
    public final PropertyDesc getDesc () {
        return desc;
    }



    /** Removes all values from self, if allowed.
     *  Clearing values is only allowed if the property is specified to
     *  have a variable number of values.  Otherwise this method is a
     *  no-op.
     */
    public void clearValues () {
        if (num == -1)
            vals.clear();
    }



    /** Sets the ind'th value of self. */
    public void setValue (String s, int ind) {
        if (ind < 0)
        {
            return;
        }
        if (num == -1)
        {
            while (ind >= vals.size())
            {
                appendVarValue();
            }
        }
        else if (ind >= num)
        {
            return;
        }
        ((VarValue)vals.get(ind)).set(s);
    }

    public void setValue (boolean s, int ind) {
        if (ind < 0)
        {
            return;
        }
        if (num == -1)
        {
            while (ind >= vals.size())
            {
                appendVarValue();
            }
        }
        else if (ind >= num)
        {
            return;
        }
        ((VarValue)vals.get(ind)).set(s);
    }


    public void setValue (VarValue s, int v) {
        if (valtype != s.getValType())
        {
            System.out.println ("Property.setValue() - " + desc.getToken() + " - type mismatch");
        return;
        }
        if (v < 0)
        {
            return;
        }
        if (num == -1)
        {
            while (v >= vals.size())
            {
                appendVarValue();
            }
        }
        else if (v >= num)
        {
            return;
        }
        ((VarValue)vals.get(v)).set(s);
    }

    /** Returns the ith value of self.
     *  @return The ith value of self, or null if i is out of bounds.
     */
    public VarValue getValue (int i) {
        if (i < 0 || i >= vals.size())
        {
            return null;
        }
        return (VarValue)vals.get(i);
    }



    /** Returns the number of values contained in self.
     */
    public final int getNum () {
        return vals.size();
    }



    /** Returns whether self has a variable number of values.
     *  @return True iff values can be added to or removed from self.
     */
    public final boolean getHasVariableNumberOfValues () {
        return desc.getHasVariableNumberOfValues ();
    }



    /** Returns the ChunkDesc describing self's embedded chunk values.
     *  @return The ChunkDesc used to describe self's embedded chunk values.
     *          Returns null if self's type is not ValType.EMBEDDEDCHUNK, or
     *          if the appropriate ChunkDesc could not be found.
     */
    public final ChunkDesc getEmbeddedDesc () {
        return embeddesc;
    }


    
    /** Checks if two Properties are equal.
     *  Note - this does a total value comparison, not just checking names
     *  The exception to this is that right now I only check the name of the
     *  chunkdesc
     *  @return True iff self and p are equal - ie they use the same
     *          PropertyDesc, contain equal values, etc.
     */
    public boolean equals (Property p) {
        VarValue v1,v2;

        if (p == null)
        {
            return false;
        }
        if (num != p.num)
        {
            return false;
        }
        if (!getToken().equals(p.getToken()))
        {
            return false;
        }
        if (valtype != p.getValType())
        {
            return false;
        }
        if (vals.size() != p.vals.size())
        {
            return false;
        }
        int i, n = vals.size();
        for (i = 0; i < n; i++)
        {
            v1 = (VarValue)vals.get(i);
            v2 = (VarValue)p.vals.get(i);
            if (!v1.equals(v2))
            {
                return false;
            }
        }
        return true;
    }

    public String toString () {
        return xmlRep ("");
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
}
