/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _JCCL_RTRC_INTERFACE_H_
#define _JCCL_RTRC_INTERFACE_H_

#include <jccl/Plugins/PluginConfig.h>

#include <string>

#include <vpr/Util/ReturnStatus.h>
#include <tweek/CORBA/CorbaManager.h>
#include <jccl/RTRC/RemoteReconfig.h>


extern "C" JCCL_PLUGIN_API(jccl::RemoteReconfig*) initPlugin();

namespace jccl 
{

class RemoteReconfigSubjectImpl;

/**
 * CORBA remote run-time reconfiguration interface implementation. Handles
 * configuration of the Tweek CORBA Manager and creation of the interface
 * subject.
 *
 * @date July 31, 2002
 */
class CorbaRemoteReconfig : public jccl::RemoteReconfig
{

public:
   CorbaRemoteReconfig();

   virtual ~CorbaRemoteReconfig();

   /**
    * Initializes the RTRC interface.
    */
   vpr::ReturnStatus init();

   /**
    * Turns on the interface to RTRC (allow incoming connections).
    */   
   vpr::ReturnStatus enable();

   bool isEnabled() const;

   /**
    * Turns off the interface to RTRC (disallow incoming connections).
    */
   void disable();

   /**
    * Invokes the global scope delete operator.  This is required for proper
    * releasing of memory in DLLs on Win32.
    */
   void operator delete(void* p)
   {
      ::operator delete(p);
   }

protected:
   virtual void destroy()
   {
      delete this;
   }

private:
   tweek::CorbaManager* mCorbaManager;
   RemoteReconfigSubjectImpl* mInterface;
   bool mEnabled;
   std::string mInterfaceName;
};

} // namespace jccl

#endif
