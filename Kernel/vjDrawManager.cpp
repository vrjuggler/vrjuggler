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
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <vjConfig.h>
#include <Kernel/vjDrawManager.h>
#include <Kernel/vjDisplayManager.h>

void vjDrawManager::setDisplayManager(vjDisplayManager* _dispMgr)
{ displayManager = _dispMgr; }


ostream& operator<<(ostream& out, vjDrawManager& drawMgr)
{
   drawMgr.outStream(out);
   return out;
}
