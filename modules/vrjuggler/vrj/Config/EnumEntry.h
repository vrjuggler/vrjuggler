/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
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



