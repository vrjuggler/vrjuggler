// VRJ.NET is (C) Copyright 2004 by Patrick Hartling
// Distributed under the GNU Lesser General Public License 2.1.  (See
// accompanying file COPYING.txt or http://www.gnu.org/copyleft/lesser.txt)

// File:          $RCSfile$
// Date modified: $Date$
// Version:       $Revision$

// Generated from $Revision$ of $RCSfile$


#ifndef __GADGET_BASEDEVICEINTERFACE_ADAPTER__
#define __GADGET_BASEDEVICEINTERFACE_ADAPTER__

#include "sharppy.h"
#include <gadget/Type/DeviceInterface.h>

class SHARPPY_API gadget_BaseDeviceInterface_Adapter : public gadget::BaseDeviceInterface
{
public:
   gadget_BaseDeviceInterface_Adapter(const gadget::BaseDeviceInterface& p0)
      : gadget::BaseDeviceInterface(p0)
   {;}

   gadget_BaseDeviceInterface_Adapter()
      : gadget::BaseDeviceInterface()
   {;}

   virtual ~gadget_BaseDeviceInterface_Adapter()
   {;}

   typedef void (*refresh_callback_t)();
   refresh_callback_t refresh_callback;

   /// Override for virtual function gadget::BaseDeviceInterface::refresh.
   virtual void refresh()
   {
      refresh_callback();
   }

}; // class gadget_BaseDeviceInterface_Adapter




#endif
