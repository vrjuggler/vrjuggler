
#ifndef _PARSEUTIL_H_
#define _PARSEUTIL_H_

#include <vjConfig.h>
#include <Config/vjVarValue.h>

int readString (istream& in, char* buffer, int size, bool *quoted = NULL);
VarType readType (istream &ip);
char *typeString (VarType t);
char *unitString (CfgUnit t);
float toFeet (float val, CfgUnit unit);

// string stuff
// this is the same as strcasecmp, so it returns true if strings are
// different!
bool vjstrcasecmp (const std::string& a, const std::string& b);

#endif
