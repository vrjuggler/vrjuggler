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


#ifndef _VJ_ENUMENTRY_H_
#define _VJ_ENUMENTRY_H_

/* Tiny little data structure used to store an entry of enumeration
 * or valuelabel data 
 */


#include <jccl/jcclConfig.h>
#include <jccl/Config/vjVarValue.h>

namespace jccl {
   
class EnumEntry {
private:
    std::string entry;
    VarValue val;

public:
    EnumEntry (const std::string& _entry, VarValue _val): val(T_INT) {
	entry = _entry;
	val = _val;
    }


    EnumEntry (const EnumEntry& e):val(e.val) {
	entry = e.entry;
    }


    const std::string& getName () const {
	return entry;
    }

    const VarValue& getValue () const {
	return val;
    }

    friend std::ostream& operator << (std::ostream& out, const EnumEntry& e) {
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

};
#endif



