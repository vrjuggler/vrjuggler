/*
 *  File:	    $Name$
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
