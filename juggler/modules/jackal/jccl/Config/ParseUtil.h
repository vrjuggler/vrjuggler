/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
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



#ifndef _PARSEUTIL_H_
#define _PARSEUTIL_H_

#include <vjConfig.h>
#include <Config/vjVarValue.h>

bool readString (istream& in, char* buffer, int size, bool *quoted = NULL);
VarType readType (istream &ip);
char *typeString (VarType t);
char *unitString (CfgUnit t);
float toFeet (float val, CfgUnit unit);

//: same as stdlib strcasecmp, but args are std::strings
bool vjstrcasecmp (const std::string& a, const std::string& b);

//: strncasecmp(), but with strings as arguments
//! NOTE: if n < 0, or n is ommitted, n is set to the size of the shorter arg
//! NOTE: comparisons are case-insensitive
//! RETURNS: true - if the first n chars of a & b do NOT match
//! RETURNS: false - if the first n chars of strings a & b match
bool vjstrncasecmp (const std::string& a, const std::string& b, int _n = -1);

//: same as strncmp, but with std::strings as arguments
//! NOTE: if n < 0, or n is ommitted, n is set to the size of the shorter arg
//! RETURNS: true - if the first n chars of a & b do NOT match
//! RETURNS: false - if the first n chars of strings a & b match
bool vjstrncmp (const std::string& a, const std::string& b, int _n = -1);

#endif
