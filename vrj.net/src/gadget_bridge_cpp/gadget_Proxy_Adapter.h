/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VRJ.NET is (C) Copyright 2004 by Patrick Hartling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

// Generated from Revision: 1.15 of RCSfile: class_cxx_adapter.tmpl,v


#ifndef __GADGET_PROXY_ADAPTER__
#define __GADGET_PROXY_ADAPTER__

#include "sharppy.h"
#include <gadget/Type/Proxy.h>

class SHARPPY_API gadget_Proxy_Adapter : public gadget::Proxy
{
public:
   gadget_Proxy_Adapter()
      : gadget::Proxy()
   {;}

   virtual ~gadget_Proxy_Adapter()
   {;}

   struct holder_p0_gadget_Proxy_config__boost_shared_ptr_jccl__ConfigElement { boost::shared_ptr<jccl::ConfigElement> mPtr; };
   typedef bool (*config_callback_boost_shared_ptr_jccl__ConfigElement_t)(holder_p0_gadget_Proxy_config__boost_shared_ptr_jccl__ConfigElement*);
   config_callback_boost_shared_ptr_jccl__ConfigElement_t config_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function gadget::Proxy::config.
   virtual bool config(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      holder_p0_gadget_Proxy_config__boost_shared_ptr_jccl__ConfigElement* h_p0 = new holder_p0_gadget_Proxy_config__boost_shared_ptr_jccl__ConfigElement; h_p0->mPtr = p0;
      bool result = config_callback_boost_shared_ptr_jccl__ConfigElement(h_p0);
      ;
      return result;
   }

   typedef bool (*refresh_callback_t)();
   refresh_callback_t refresh_callback;

   /// Override for virtual function gadget::Proxy::refresh.
   virtual bool refresh()
   {
      bool result = refresh_callback();
      return result;
   }

   typedef void (*updateData_callback_t)();
   updateData_callback_t updateData_callback;

   /// Override for virtual function gadget::Proxy::updateData.
   virtual void updateData()
   {
      updateData_callback();
   }

   typedef gadget::Input* (*getProxiedInputDevice_callback_t)();
   getProxiedInputDevice_callback_t getProxiedInputDevice_callback;

   /// Override for virtual function gadget::Proxy::getProxiedInputDevice.
   virtual gadget::Input* getProxiedInputDevice()
   {
      gadget::Input* result = getProxiedInputDevice_callback();
      return result;
   }

   typedef bool (*isStupified_callback_t)();
   isStupified_callback_t isStupified_callback;

   /// Override for virtual function gadget::Proxy::isStupified.
   virtual bool isStupified()
   {
      bool result = isStupified_callback();
      return result;
   }

}; // class gadget_Proxy_Adapter




#endif
