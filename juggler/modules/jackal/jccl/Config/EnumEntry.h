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


#ifndef _VJ_ENUMENTRY_H_
#define _VJ_ENUMENTRY_H_

/* Tiny little data structure used to store an entry of enumeration
 * or valuelabel data 
 */


#include <vjConfig.h>
#include <Config/vjVarValue.h>

class vjEnumEntry {
private:
    std::string entry;
    vjVarValue val;

public:
    vjEnumEntry (const std::string& _entry, vjVarValue _val): val(T_INT) {
	entry = _entry;
	val = _val;
    }


    vjEnumEntry (vjEnumEntry& e):val(e.val) {
	entry = e.entry;
    }


    std::string getName () {
	return entry;
    }

    vjVarValue& getValue () {
	return val;
    }

    friend ostream& operator << (ostream& out, vjEnumEntry& e) {
	switch (e.val.getType()) {
	case T_INT:
	case T_FLOAT:
	case T_BOOL:
	    out << '"' << e.entry.c_str() << '=' << e.val << '"';
	    break;
	default:
	    if ((std::string)e.val == "" || (std::string)e.val == e.entry)
		out << '"' << e.entry.c_str() << '"';
	    else
		out << '"' << e.entry.c_str() << '=' << e.val << '"';
	    break;
	}
	return out;
    }
};


#endif



