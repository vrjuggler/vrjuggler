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

    vjVarValue& getVal () {
	return val;
    }
};


#endif
