
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

//: strncasecmp(), but with strings as arguments
//! NOTE: if n < 0, or n is ommitted, n is set to the size of the shorter arg
//! NOTE: comparisons are case-insensitive
//! RETURNS: true - if the first n chars of a & b do NOT match
//! RETURNS: false - if the first n chars of strings a & b match
bool vjstrncasecmp (const std::string& a, const std::string& b, int _n = -1);

#endif
