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
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
#include <Kernel/vjWin32SystemFactory.h>
#include <Kernel/GL/vjGlWinWin32.h>


vjWin32SystemFactory* vjWin32SystemFactory::_instance = NULL;

vjGlWindow* vjWin32SystemFactory::getGLWindow()
{
   return new vjGlWinWin32;
}

