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


/* ValType - just replaces our valtype enumeration, more or less */

package VjConfig;

public class ValType {

    private final static int t_string        = 1;
    private final static int t_float         = 2;
    private final static int t_int           = 3;
    private final static int t_bool          = 4;
    private final static int t_chunk         = 5;
    private final static int t_embeddedchunk = 6;
    private final static int t_invalid       = 7;

    public final static ValType STRING = new ValType (t_string);
    public final static ValType FLOAT = new ValType (t_float);
    public final static ValType INT = new ValType (t_int);
    public final static ValType BOOL = new ValType (t_bool);
    public final static ValType CHUNK = new ValType (t_chunk);
    public final static ValType EMBEDDEDCHUNK = new ValType (t_embeddedchunk);
    public final static ValType INVALID = new ValType (t_invalid);

    int val;


    private ValType (int i) {
	val = i;
    }



    public static ValType getValType (String s) {
	String s2 = s.toLowerCase().trim();
	if (s2.equalsIgnoreCase ("string"))
	    return STRING;
	else if (s2.equalsIgnoreCase ("float"))
	    return FLOAT;
	else if (s2.equalsIgnoreCase ("int"))
	    return INT;
	else if (s2.equalsIgnoreCase ("bool"))
	    return BOOL;
	else if (s2.equalsIgnoreCase ("chunk"))
	    return CHUNK;
	else if (s2.equalsIgnoreCase ("embeddedchunk"))
	    return EMBEDDEDCHUNK;
	else 
	    return INVALID;
    }



    public String toString() {
	switch (val) {
	case t_string:
	    return "String";
	case t_float:
	    return "Float";
	case t_int:
	    return "Int";
	case t_bool:
	    return "Bool";
	case t_chunk:
	    return "Chunk";
	case t_embeddedchunk:
	    return "EmbeddedChunk";
	default:
	    return "Invalid";
	}
    }



//     public boolean equals (int t) {
// 	return (val == t);
//     }



    public boolean equals (ValType v) {
        if (v == null)
            return false;
	return (val == v.val);
    }


//     // returns the int representation of type.  need this to do
//     // switch() on ValType
//     public int getInt() {
// 	return val;
//     }
	

}

