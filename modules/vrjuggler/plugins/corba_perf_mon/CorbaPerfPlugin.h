/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_CORBAPERF_PLUGIN_H_
#define _VRJ_CORBAPERF_PLUGIN_H_

#include <vrj/Performance/PluginConfig.h>

#include <string>

#include <tweek/CORBA/CorbaManager.h>
#include <vrj/Performance/PerfPlugin.h>
#include <jccl/RTRC/ConfigElementHandler.h>


namespace jccl
{
   class ConfigManager;
}

namespace vrj
{

   class PerformanceMonitorSubjectImpl;
   class PerformanceMediator;

/**
 * CORBA remote performance monitor implementation. Handles
 * configuration of the Tweek CORBA Manager and creation of the interface
 * subject.
 */
   class CorbaPerfPlugin
      : public vrj::PerfPlugin
      , public jccl::ConfigElementHandler
   {

   public:
      CorbaPerfPlugin(vrj::PerformanceMediator* mediator, jccl::ConfigManager* configMgr);

      virtual ~CorbaPerfPlugin();

      bool configCanHandle(jccl::ConfigElementPtr e);

      bool configAdd(jccl::ConfigElementPtr e);

      bool configRemove(jccl::ConfigElementPtr e);

      /** Stubbed out function required by the vrj::PerfPlugin interface. */
      bool init();

      /** Stubbed out function required by the vrj::PerfPlugin interface. */
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
       * handling remote performance monitoring requests.
       *
       * @param listenAddr The address to which the CORBA endpoint will be
       *                   bound. Using an empty string will result in binding
       *                   to the default interface.
       * @param listenPort The port number for the CORBA endpoint. This must
       *                   be a valid port number.
       */
      bool startCorba(const std::string& listenAddr,
                      const vpr::Uint16 listenPort);

      /**
       * Disables the local CORBA instance and releases the resources allocated
       * in startCorba().  The CORBA instance is only disabled if isEnabled()
       * returns true.
       *
       * @post The local CORBA instance is running, and the memory allocated for
       *       it (held in mCorbaManager and mInterface) is released.
       */
      void stopCorba();

      vrj::PerformanceMediator* mMediator;
      // XXX: We hold a pointer to the Config Manager because we can get into a
      // deadlock state by trying to access the Config Manager through its
      // singleton interface.
      jccl::ConfigManager* mConfigManager;
      tweek::CorbaManager* mCorbaManager;
      PerformanceMonitorSubjectImpl* mInterface;
      bool mEnabled;
      std::string mInterfaceName;
   };

} // namespace vrj

#endif
