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


#ifndef _VJ_ANA_INTERFACE_H_
#define _VJ_ANA_INTERFACE_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjAnalogProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides a simplified user interface to an analog device
//
// See also: vjDeviceInterface
//!PUBLIC_API:
class vjAnalogInterface : public vjDeviceInterface
{
public:
   vjAnalogInterface() : mAnaProxy(NULL)
   {;}

   vjAnalogProxy* operator->()
   { return mAnaProxy; }

   vjAnalogProxy& operator*()
   { return *(mAnaProxy); }

   virtual void refresh()
   {
      vjDeviceInterface::refresh();
      if(mProxyIndex != -1)
         mAnaProxy = vjKernel::instance()->getInputManager()->getAnaProxy(mProxyIndex);
      else
         mAnaProxy = NULL;
   }

private:
   vjAnalogProxy* mAnaProxy;     // The proxy that is being wrapped
};

#endif
