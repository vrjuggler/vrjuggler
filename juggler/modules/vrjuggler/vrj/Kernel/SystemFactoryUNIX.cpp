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
#include <Kernel/vjSGISystemFactory.h>
#include <Kernel/GL/vjGlxWindow.h>


vjSGISystemFactory* vjSGISystemFactory::_instance = NULL;

vjGlWindow* vjSGISystemFactory::getGLWindow()
{
   return new vjGlxWindow;
}

