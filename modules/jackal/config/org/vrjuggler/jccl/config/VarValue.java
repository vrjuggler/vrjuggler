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

import VjConfig.*;
import java.lang.Cloneable;

public final class VarValue {
  /* of course, this can't do any of the leet things the C++ varvalue
   * class does, because Sun would track us down and kill us for doing
   * things like that in Java ;)
   */

    private String         strval;
    private int            intval;
    private float          floatval;
    private boolean        boolval;
    private ConfigChunk    embeddedchunkval;

    private ValType        valtype;



    public VarValue (VarValue v) {
	valtype = v.valtype;
	strval = v.strval;
	intval = v.intval;
	floatval = v.floatval;
	boolval = v.boolval;
	if (v.embeddedchunkval == null)
	    embeddedchunkval = null;
	else
	    embeddedchunkval = new ConfigChunk (v.embeddedchunkval);
    }



    public VarValue (ValType t) {
	valtype = t;
	strval = "";
        intval = 0;
        floatval = 0.0f;
        boolval = false;
	embeddedchunkval = null;
    }

    public VarValue (int v) {
	valtype = new ValType(ValType.t_int);
	intval = v;
    }

    public VarValue (float v) {
	valtype = new ValType (ValType.t_float);
	floatval = v;
    }

    public VarValue (boolean v) {
	valtype = new ValType (ValType.t_bool);
	boolval = v;
    }

    public VarValue (String v) {
	valtype = new ValType (ValType.t_string);
	strval = v;
    }

    public VarValue (ConfigChunk v) {
	valtype = new ValType (ValType.t_embeddedchunk);
	embeddedchunkval = v;
    }



    public boolean equals(VarValue v) {
        if (v == null)
            return false;
	if (!valtype.equals(v.valtype))
	    return false;
	else if (valtype.equals (ValType.t_int))
	    return (intval == v.intval);
	else if (valtype.equals (ValType.t_float))
	    return (floatval == v.floatval);
	else if (valtype.equals (ValType.t_bool))
	    return (boolval == v.boolval);
	else if (valtype.equals (ValType.t_embeddedchunk))
	    return (embeddedchunkval.equals (v.embeddedchunkval));
	else
	    return (strval.equals(v.strval));
    }



    public String toString() {
	String s = null;
	if (valtype.equals(ValType.t_bool)) {
	    s = (boolval)?"True":"False";
	}
	else if (valtype.equals(ValType.t_string)) {
	    s = strval;
	}
	else if (valtype.equals(ValType.t_chunk)) {
	    s = strval;
	}
	else if (valtype.equals(ValType.t_int)) {
	    s = String.valueOf(intval);
	}
	else if (valtype.equals(ValType.t_float)) {
	    s = String.valueOf(floatval);
	}
	else if (valtype.equals (ValType.t_embeddedchunk)) {
	    s = embeddedchunkval.toString();
	}
	return s;
    }
    


    public ValType getValType () {
	return valtype;
    }



    /* I've decided life will be a lot easier if set is a bit smarter about
     * converting strings into different things
     */
    public void set (String s) {
	try {
	    switch (valtype.getInt()) {
	    case ValType.t_string:
		strval = s;
		break;
	    case ValType.t_chunk:
		strval = s;
		break;
	    case ValType.t_bool:
		boolval = s.equalsIgnoreCase("true")? true:false;
		break;
	    case ValType.t_int:
		intval = Integer.parseInt(s);
		break;
	    case ValType.t_float:
		floatval = new Float(s).floatValue();
		/* why doesn't Float have something analogous to 
		 * Integer.parseInt()? 
		 */
		break;
	    case ValType.t_embeddedchunk:
		embeddedchunkval = null;
		throw (new VarValueException ("Error assigning string to " +
					      "embeddedchunk VarValue"));
		//break;
	    default:
		break;
	    }
	}
	catch (NumberFormatException e) {
	    /* just so it's defined: if an error occurred, we set the numeric
	     * values to 0
	     */
	    intval = 0;
	    floatval = 0.0f;
	    if (s.equals (""))
		return;
	    throw (new VarValueException ("Error assigning '" + s 
					  + "' to VarValue"));
	}
    }



    public void set (boolean s) {
	if (valtype.equals(ValType.t_bool))
	    boolval = s;
	else
	    throw (new VarValueException ("Error assigning to Boolean VarValue"));
    }


    public void set (int s) {
	switch (valtype.getInt()) {
	case ValType.t_int:
	    intval = s;
	    break;
	case ValType.t_float:
	    floatval = (float)s;
	    break;
	case ValType.t_bool:
	    boolval = (s==0)?false:true;
	    break;
	default:
	    throw (new VarValueException ("Error assigning to Integer VarValue"));
	}
    }


    public void set (float s) {
	if (valtype.equals(ValType.t_float))
	    floatval = s;
	else
	    throw (new VarValueException ("Error assigning to Float VarValue"));
    }


    public void set (ConfigChunk ch) {
	if (valtype.equals (ValType.t_embeddedchunk))
	    embeddedchunkval = ch;
	else
	    throw (new VarValueException ("Error assigning to EmbeddedChunk VarValue"));
    }


    public void set (VarValue s) {
	/* so we can copy one varvalue into another */
	if (!valtype.equals(s.getValType()))
	    throw (new VarValueException ("Incompatible types assigning one VarValue to another."));
	boolval = s.boolval;
	intval = s.intval;
	floatval = s.floatval;
	strval = s.strval;
	embeddedchunkval = s.embeddedchunkval;
    }


    public int getInt() {
	switch (valtype.getInt()) {
	case ValType.t_int:
	    return intval;
	case ValType.t_float:
	    return (int)floatval;
	case ValType.t_bool:
	    return boolval?1:0;
	default:
	    throw (new VarValueException ("Error in getInt()"));
	}
    }


    public float getFloat() {
	if (valtype.equals(ValType.t_float))
	    return floatval;
	else
	    throw (new VarValueException ("Error in getFloat()"));
    }



    public boolean getBoolean() {
	if (valtype.equals(ValType.t_bool))
	    return boolval;
	else
	    throw (new VarValueException ("Error in getBool()"));
    }


    public String getString() {
	if (valtype.equals(ValType.t_string) || valtype.equals(ValType.t_chunk))
	    return strval;
	else
	    throw (new VarValueException ("Error in getInt()"));
    }


    public ConfigChunk getEmbeddedChunk() {
	if (valtype.equals (ValType.t_embeddedchunk))
	    return embeddedchunkval;
	else
	    throw (new VarValueException ("Error in getEmbeddedChunk()"));
    }


    public Object get() {
	if (valtype.equals(ValType.t_string))
	    return strval;
	else if (valtype.equals(ValType.t_chunk))
	    return strval;
	else if (valtype.equals(ValType.t_int))
	    return new Integer(intval);
	else if (valtype.equals(ValType.t_float))
	    return new Float(floatval);
	else if (valtype.equals(ValType.t_bool))
	    return new Boolean(boolval);
	else if (valtype.equals (ValType.t_embeddedchunk))
	    return embeddedchunkval;
	else
	    throw (new VarValueException ("Error in get()"));
    }


}

