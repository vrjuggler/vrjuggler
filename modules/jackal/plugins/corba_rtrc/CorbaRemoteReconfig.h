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

#ifndef _JCCL_RTRC_INTERFACE_H_
#define _JCCL_RTRC_INTERFACE_H_

#include <jccl/Plugins/PluginConfig.h>

#include <string>

#include <tweek/CORBA/CorbaManager.h>
#include <jccl/RTRC/RemoteReconfig.h>
#include <jccl/RTRC/ConfigElementHandler.h>


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
class CorbaRemoteReconfig
   : public jccl::RemoteReconfig
   , public jccl::ConfigElementHandler
{

public:
   CorbaRemoteReconfig();

   virtual ~CorbaRemoteReconfig();

   void setConfigManager(jccl::ConfigManager* configMgr);

   bool configCanHandle(jccl::ConfigElementPtr e);

   bool configAdd(jccl::ConfigElementPtr e);

   bool configRemove(jccl::ConfigElementPtr e);

   /** Stubbed out function required by the jccl::RemoteReconfig interface. */
   bool init();

   /** Stubbed out function required by the jccl::RemoteReconfig interface. */
   bool enable();

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
   /**
    * Initializes and activates the local CORBA instance that we need for
    * handling remote reconfiguration requests.
    *
    * @param nsHost  The name of the host computer where the CORBA Naming
    *                Service is running.
    * @param nsport  The port number on which the CORBA Naming Service is
    *                listening for incoming connections.
    * @param iiopVer The version of IIOP the Naming Service is using.  This
    *                is normally 1.0 or 1.2.
    */
   bool startCorba(const std::string& nsHost, const vpr::Uint16 nsPort,
                   const std::string& iiopVer);

   /**
    * Disables the local CORBA instance and releases the resources allocated
    * in startCorba().  The CORBA instance is only disabled if isEnabled()
    * returns true.
    *
    * @post The local CORBA instance is running, and the memory allocated for
    *       it (held in mCorbaManager and mInterface) is released.
    */
   void stopCorba();

   // XXX: We hold a pointer to the Config Manager because we can get into a
   // deadlock state by trying to access the Config Manager through its
   // singleton interface.
   jccl::ConfigManager* mConfigManager;
   tweek::CorbaManager* mCorbaManager;
   RemoteReconfigSubjectImpl* mInterface;
   bool mEnabled;
   std::string mInterfaceName;
};

} // namespace jccl

#endif
