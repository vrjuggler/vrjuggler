/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
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
    vjEnumEntry (const std::string& e, int v): val(T_INT) {
	entry = e;
	val = v;
    }

    std::string getName () {
	return entry;
    }

    vjVarValue& getValue () {
	return val;
    }
};


#endif
