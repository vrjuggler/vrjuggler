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


#ifndef _VJ_POS_INTERFACE_H_
#define _VJ_POS_INTERFACE_H_

#include <Input/InputManager/vjPosProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides a simplified user interface to a pos device
//
// See Also: vjDeviceInterface
//!PUBLIC_API:
class vjPosInterface : public vjDeviceInterface
{
public:
   vjPosInterface() : mPosProxy(NULL)
   {;}

   vjPosProxy* operator->();
   vjPosProxy& operator*();

   virtual void refresh();

private:
   vjPosProxy* mPosProxy;     // The proxy that is being wrapped
};

#endif
