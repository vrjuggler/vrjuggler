/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _VRJ_PERFORMANCE_MEDIATOR_H_
#define _VRJ_PERFORMANCE_MEDIATOR_H_

#include <vrj/vrjConfig.h>
#include <vector>
#include <list>

#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Singleton.h>
#include <vpr/DynLoad/Library.h>


namespace vrj
{

   class PerfPlugin;

   /** \class PerformanceMediator PerformanceMediator.h vrj/Performance/PerformanceMediator.h
    *
    * Performance mediator used by the kernel to communicate with performance
    * monitoring plug-ins.
    */
   class VJ_CLASS_API PerformanceMediator
   {
   public:
      PerformanceMediator();
      virtual ~PerformanceMediator();
      void setPerfPlugin(vrj::PerfPlugin* plugin);

   protected:
      /** Enables the remote performance monitoring interface object. */
      void loadPerfPlugin();

      // needed for windows:
      PerformanceMediator(const PerformanceMediator&)
      {
         ;
      }
      void operator= (const PerformanceMediator&)
      {
         ;
      }
   private:
      vpr::LibraryPtr mPluginDSO;
      PerfPlugin* mPerfIf;
   }; // class Mediator

} // namespace vrj


#endif
