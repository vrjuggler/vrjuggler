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


#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjPosInterface.h>


vjPosProxy* vjPosInterface::operator->()
{ return mPosProxy;}

vjPosProxy& vjPosInterface::operator*()
{ return *(mPosProxy);}

void vjPosInterface::refresh()
{
   vjDeviceInterface::refresh();
   if(mProxyIndex != -1)
      mPosProxy = vjKernel::instance()->getInputManager()->getPosProxy(mProxyIndex);
   else
      mPosProxy = NULL;
}
