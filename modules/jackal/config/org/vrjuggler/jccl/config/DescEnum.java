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


/* DescEnum -- an entry in the enum vector of a PropertyDesc */

package VjConfig;

import VjConfig.VarValue;

public class DescEnum {

    public String str;
    public VarValue val;



    public String toString () {
	if (val != null)
	    return new String (str + "=" + val);
	else 
	    return new String (str);
    }



    public DescEnum (DescEnum other) {
	str = other.str;
	val = new VarValue (other.val);
    }



    public DescEnum (String s, VarValue v) {
	str = s;
	val = new VarValue(v);
	//System.out.println(toString());
    }


    public DescEnum (String s, String v) {
	str = s;
	val = new VarValue (v);
    }

    public DescEnum (String s, int v) {
	str = s;
	val = new VarValue(v);
    }
    


    public boolean equals (DescEnum d) {
	if (!str.equalsIgnoreCase(d.str))
	    return false;
	if (!val.equals(d.val))
	    return false;
	return true;
    }

    
}
