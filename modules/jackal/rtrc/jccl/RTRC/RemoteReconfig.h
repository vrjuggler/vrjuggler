/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _JCCL_REMOTE_RECONFIG_H_
#define _JCCL_REMOTE_RECONFIG_H_

#include <jccl/jcclConfig.h>


namespace jccl 
{

/** \class RemoteReconfig RemoteReconfig.h jccl/RTRC/RemoteReconfig.h
 *
 * Remote reconfiguration plug-in interface.
 */
class RemoteReconfig
{
public:
   virtual ~RemoteReconfig()
   {
      /* Do nothing. */ ;
   }

   /**
    * Initializes the remote reconfiguration interface.
    */
   virtual bool init() = 0;

   /**
    * Turns on the interface to remote reconfiguration (allow incoming
    * connections).
    */   
   virtual bool enable() = 0;

   /** Indicates whether this remote reconfiguration object is active. */
   virtual bool isEnabled() const = 0;

   /**
    * Turns off the interface to remote reconfiguration (disallow incoming
    * connections).
    */
   virtual void disable() = 0;

#ifdef VPR_OS_Windows
   /**
    * Overlaod delete so that we can delete our memory correctly.  This is
    * necessary for DLLs on Windows to release memory from the correct memory
    * space.  All subclasses must overload delete similarly.
    */
   void operator delete(void* p)
   {
      if ( NULL != p )
      {
         RemoteReconfig* obj_ptr = static_cast<RemoteReconfig*>(p);
         obj_ptr->destroy();
      }
   }
#endif
 
protected:
   /**
    * Subclasses must implement this so that dynamically loaded device drivers
    * delete themselves in the correct memory space.  This uses a template
    * pattern.
    */
   virtual void destroy() = 0;

protected:
   RemoteReconfig()
   {
      /* Do nothing. */ ;
   }

   RemoteReconfig(const RemoteReconfig&)
   {
      /* Do nothing. */ ;
   }
};

} // namespace jccl

#endif
