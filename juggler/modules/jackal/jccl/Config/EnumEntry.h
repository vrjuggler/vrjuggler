/* Tiny little data structure used to store an entry of enumeration
 * or valuelabel data 
 */

#include <strings.h>

class vjEnumEntry {
private:
    char *entry;
    int val;

public:
    vjEnumEntry (char *e, int v) {
	entry = new char[strlen(e)+1];
	strcpy (entry, e);
	val = v;
    }

    ~vjEnumEntry () {
	delete entry;
    }

    char* getName () {
	return entry;
    }

    int getVal () {
	return val;
    }
};
