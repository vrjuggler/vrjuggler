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


/* ValType - just replaces our valtype enumeration, more or less */

package VjConfig;

public class ValType {

    public final static int t_string        = 1;
    public final static int t_float         = 2;
    public final static int t_int           = 3;
    public final static int t_bool          = 4;
    public final static int t_chunk         = 5;
    public final static int t_embeddedchunk = 6;

    int val;



    public ValType (ValType t) {
	val = t.val;
    }


    public ValType (String s) {
	String s2 = s.toLowerCase().trim();
	if (s2.equalsIgnoreCase ("string"))
	    val = t_string;
	else if (s2.equalsIgnoreCase ("float"))
	    val = t_float;
	else if (s2.equalsIgnoreCase ("int"))
	    val = t_int;
	else if (s2.equalsIgnoreCase ("bool"))
	    val = t_bool;
	else if (s2.equalsIgnoreCase ("chunk"))
	    val = t_chunk;
	else if (s2.equalsIgnoreCase ("embeddedchunk"))
	    val = t_embeddedchunk;
	else 
	    val = 0;
    }


    public ValType (int i) {
	val = i;
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
	    return "Undefined Type";
	}
    }



    public boolean equals (int t) {
	return (val == t);
    }



    public boolean equals (ValType v) {
	return (val == v.val);
    }


    // returns the int representation of type.  need this to do
    // switch() on ValType
    public int getInt() {
	return val;
    }
	

}

