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


#ifndef __VRJ_USER_ADAPTER__
#define __VRJ_USER_ADAPTER__

#include "sharppy.h"
#include <vrj/Kernel/User.h>

class SHARPPY_API vrj_User_Adapter : public vrj::User
{
public:
   vrj_User_Adapter()
      : vrj::User()
   {;}

   virtual ~vrj_User_Adapter()
   {;}

   typedef bool (*config_callback_boost_shared_ptr_jccl__ConfigElement_t)(boost::shared_ptr<jccl::ConfigElement>);
   config_callback_boost_shared_ptr_jccl__ConfigElement_t config_callback_boost_shared_ptr_jccl__ConfigElement;

   /// Override for virtual function vrj::User::config.
   virtual bool config(boost::shared_ptr<jccl::ConfigElement> p0)
   {
      return config_callback_boost_shared_ptr_jccl__ConfigElement(p0);
   }

}; // class vrj_User_Adapter




#endif
