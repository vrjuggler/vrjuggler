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

// Generated from $Revision$ of $RCSfile$


#ifndef __VRJ_KERNEL_ADAPTER__
#define __VRJ_KERNEL_ADAPTER__

#include "sharppy.h"
#include <vrj/Kernel/Kernel.h>

class SHARPPY_API vrj_Kernel_Adapter : public vrj::Kernel
{
public:
   virtual ~vrj_Kernel_Adapter()
   {;}

   typedef int (*configProcessPending_callback_t)();
   configProcessPending_callback_t configProcessPending_callback;

   /// Override for virtual function jccl::ConfigElementHandler::configProcessPending.
   virtual int configProcessPending()
   {
      return configProcessPending_callback();
   }

   typedef bool (*configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement_t configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::Kernel::configCanHandle.
   virtual bool configCanHandle(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configCanHandle_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef bool (*configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configAdd_callback_boost_shared_ptr_jccl__ConfigElement_t configAdd_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::Kernel::configAdd.
   virtual bool configAdd(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configAdd_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   typedef bool (*configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   configRemove_callback_boost_shared_ptr_jccl__ConfigElement_t configRemove_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::Kernel::configRemove.
   virtual bool configRemove(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return configRemove_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

   /// Public wrapper for protected virtual function vrj::Kernel::configCanHandle.
   bool configCanHandle_wrapper(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return vrj::Kernel::configCanHandle(p0);
   }

   /// Public wrapper for protected virtual function vrj::Kernel::configAdd.
   bool configAdd_wrapper(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return vrj::Kernel::configAdd(p0);
   }

   /// Public wrapper for protected virtual function vrj::Kernel::configRemove.
   bool configRemove_wrapper(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return vrj::Kernel::configRemove(p0);
   }

   /// Public wrapper for protected non-virtual function vrj::Kernel::addUser.
   bool addUser(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return vrj::Kernel::addUser(p0);
   }

   /// Public wrapper for protected non-virtual function vrj::Kernel::removeUser.
   bool removeUser(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return vrj::Kernel::removeUser(p0);
   }

   /// Public wrapper for protected non-virtual function vrj::Kernel::updateFrameData.
   void updateFrameData()
   {
      vrj::Kernel::updateFrameData();
   }

   /// Public wrapper for protected non-virtual function vrj::Kernel::checkForReconfig.
   void checkForReconfig()
   {
      vrj::Kernel::checkForReconfig();
   }

   /// Public wrapper for protected non-virtual function vrj::Kernel::changeApplication.
   void changeApplication(vrj::App* p0)
   {
      vrj::Kernel::changeApplication(p0);
   }

   /// Public wrapper for protected non-virtual function vrj::Kernel::initSignalButtons.
   void initSignalButtons()
   {
      vrj::Kernel::initSignalButtons();
   }

   /// Public wrapper for protected non-virtual function vrj::Kernel::checkSignalButtons.
   void checkSignalButtons()
   {
      vrj::Kernel::checkSignalButtons();
   }

   /// Public wrapper for protected non-virtual function vrj::Kernel::startDrawManager.
   void startDrawManager(bool p0)
   {
      vrj::Kernel::startDrawManager(p0);
   }

   /// Public wrapper for protected non-virtual function vrj::Kernel::stopDrawManager.
   void stopDrawManager()
   {
      vrj::Kernel::stopDrawManager();
   }

}; // class vrj_Kernel_Adapter




#endif
