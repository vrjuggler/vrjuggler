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


#ifndef _VJ_GLOVE_INTERFACE_H_
#define _VJ_GLOVE_INTERFACE_H_

#include <Kernel/vjKernel.h>
#include <Input/InputManager/vjInputManager.h>
#include <Input/InputManager/vjGloveProxy.h>
#include <Input/InputManager/vjDeviceInterface.h>

//: Provides a simplified user interface to an glove device
//
// See also: vjDeviceInterface
//!PUBLIC_API:
class vjGloveInterface : public vjDeviceInterface
{
public:
   vjGloveInterface() : mGloveProxy(NULL)
   {;}

   vjGloveProxy* operator->()
   { return mGloveProxy; }

   vjGloveProxy& operator*()
   { return *(mGloveProxy); }

   virtual void refresh()
   {
      vjDeviceInterface::refresh();
      if(mProxyIndex != -1)
         mGloveProxy = vjKernel::instance()->getInputManager()->GetGloveProxy(mProxyIndex);
      else
         mGloveProxy = NULL;
   }

private:
   vjGloveProxy* mGloveProxy;     // The proxy that is being wrapped
};

#endif
