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


#ifndef __JCCL_CONFIGELEMENTHANDLER_ADAPTER__
#define __JCCL_CONFIGELEMENTHANDLER_ADAPTER__

#include "sharppy.h"
#include <jccl/RTRC/ConfigElementHandler.h>

class SHARPPY_API jccl_ConfigElementHandler_Adapter : public jccl::ConfigElementHandler
{
public:
   jccl_ConfigElementHandler_Adapter()
      : jccl::ConfigElementHandler()
   {;}

   virtual ~jccl_ConfigElementHandler_Adapter()
   {;}

   typedef bool (*configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>*);
   configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function jccl::ConfigElementHandler::configCanHandle.
   virtual bool configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      boost::shared_ptr<jccl::ConfigElement>* p_p0 = new boost::shared_ptr<jccl::ConfigElement>(p0);
      bool result = configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement(p_p0);
      return result;
   }

   typedef int (*configProcessPending_callback_t)();
   configProcessPending_callback_t configProcessPending_callback;

   /// Override for virtual function jccl::ConfigElementHandler::configProcessPending.
   virtual int configProcessPending()
   {
      int result = configProcessPending_callback();
      return result;
   }

   typedef bool (*configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>*);
   configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t configAdd_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function jccl::ConfigElementHandler::configAdd.
   virtual bool configAdd(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      boost::shared_ptr<jccl::ConfigElement>* p_p0 = new boost::shared_ptr<jccl::ConfigElement>(p0);
      bool result = configAdd_callback_boost_shared_ptr_jccl__ConfigElement(p_p0);
      return result;
   }

   typedef bool (*configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>*);
   configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t configRemove_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function jccl::ConfigElementHandler::configRemove.
   virtual bool configRemove(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      boost::shared_ptr<jccl::ConfigElement>* p_p0 = new boost::shared_ptr<jccl::ConfigElement>(p0);
      bool result = configRemove_callback_boost_shared_ptr_jccl__ConfigElement(p_p0);
      return result;
   }

}; // class jccl_ConfigElementHandler_Adapter




#endif
