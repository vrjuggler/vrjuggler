
#ifndef _PARSEUTIL_H_
#define _PARSEUTIL_H_

#include <config.h>
#include <iostream.h>
#include <Config/vjVarValue.h>

int readString (istream& in, char* buffer, int size, bool *quoted = NULL);
VarType readType (istream &ip);
char *typeString (VarType t);
char *unitString (CfgUnit t);
float toFeet (float val, CfgUnit unit);

#endif
