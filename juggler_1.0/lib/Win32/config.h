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
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _CONFIG_H_
#define _CONFIG_H_

//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#pragma warning(disable:4244)  // Converting double to float
#pragma warning(disable:4800)  // Int forcing value to bool
#pragma warning(disable:4101)  // unreferenced local variable

#pragma warning(disable:4786)  // identifier truncated to 255 characters in the debug information

#include <windows.h>
#include <gl\gl.h>
#include <string>
#define string std::string

extern HINSTANCE g_hInst = NULL;

#endif _CONFIG_H_
