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

// Generated from Revision: 1.18 of RCSfile: class_cxx_adapter.tmpl,v


#ifndef __GADGET_TYPEDPROXY_GADGET__POSITION_ADAPTER__
#define __GADGET_TYPEDPROXY_GADGET__POSITION_ADAPTER__

#include "sharppy.h"
#include <gadget/Type/Position.h>
#include <gadget/Type/Proxy.h>

class SHARPPY_API gadget_TypedProxy_gadget__Position_Adapter : public gadget::TypedProxy<gadget::Position>
{
public:
   gadget_TypedProxy_gadget__Position_Adapter()
      : gadget::TypedProxy<gadget::Position>()
   {;}

   virtual ~gadget_TypedProxy_gadget__Position_Adapter()
   {;}

   typedef bool (*config_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>*);
   config_callback_boost_shared_ptr_jccl__ConfigElement_t config_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function gadget::Proxy::config.
   virtual bool config(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      boost::shared_ptr<jccl::ConfigElement>* p_p0 = new boost::shared_ptr<jccl::ConfigElement>(p0);
      bool result = config_callback_boost_shared_ptr_jccl__ConfigElement(p_p0);
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
   virtual bool isStupified() const
   {
      bool result = isStupified_callback();
      return result;
   }

   typedef vpr::Interval* (*getTimeStamp_callback_t)();
   getTimeStamp_callback_t getTimeStamp_callback;

   /// Override for virtual function gadget::Proxy::getTimeStamp.
   virtual vpr::Interval getTimeStamp() const
   {
      vpr::Interval result = *(getTimeStamp_callback());
      return result;
   }

   typedef bool (*refresh_callback_t)();
   refresh_callback_t refresh_callback;

   /// Override for virtual function gadget::TypedProxy<gadget::Position>::refresh.
   virtual bool refresh()
   {
      bool result = refresh_callback();
      return result;
   }

   typedef char* (*getDeviceName_callback_t)();
   getDeviceName_callback_t getDeviceName_callback;

   /// Override for virtual function gadget::TypedProxy<gadget::Position>::getDeviceName.
   virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > getDeviceName() const
   {
      std::basic_string<char,std::char_traits<char>,std::allocator<char> > result = getDeviceName_callback();
      return result;
   }

}; // class gadget_TypedProxy_gadget__Position_Adapter




#endif
