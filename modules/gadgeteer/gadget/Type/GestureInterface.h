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


#ifndef _VJ_GESTURE_INTERFACE_H_
#define _VJ_GESTURE_INTERFACE_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjGestureProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides a simplified user interface to an gesture device
//
// See also: vjDeviceInterface
//!PUBLIC_API:
class vjGestureInterface : public vjDeviceInterface
{
public:
   vjGestureInterface() : mGestProxy(NULL)
   {;}

   vjGestureProxy* operator->()
   { return mGestProxy; }

   vjGestureProxy& operator*()
   { return *(mGestProxy); }

   virtual void refresh()
   {
      vjDeviceInterface::refresh();
      if(mProxyIndex != -1)
         mGestProxy = vjKernel::instance()->getInputManager()->getGestureProxy(mProxyIndex);
      else
         mGestProxy = NULL;
   }

private:
   vjGestureProxy* mGestProxy;     // The proxy that is being wrapped
};

#endif
