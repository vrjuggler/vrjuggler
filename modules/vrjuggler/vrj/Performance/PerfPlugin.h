/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _VRJ_PERF_PLUGIN_H_
#define _VRJ_PERF_PLUGIN_H_

#include <vrj/vrjConfig.h>


namespace vrj
{

   /** \class PerfPlugin PerfPlugin.h vrj/Performance/PerfPlugin.h
    *
    * Performance monitoring plug-in interface.
    */
   class PerfPlugin
   {
   public:
      virtual ~PerfPlugin()
      {
         /* Do nothing. */ 
         ;
      }

      /**
       * Initializes the performance monitoring interface.
       */
      virtual bool init() = 0;

      /**
       * Turns on the interface to performance monitoring (allow incoming
       * connections).
       */   
      virtual bool enable() = 0;

      /** Indicates whether this performance monitoring object is active. */
      virtual bool isEnabled() const = 0;

      /**
       * Turns off the interface to performance monitoring (disallow incoming
       * connections).
       */
      virtual void disable() = 0;

#ifdef VPR_OS_Windows
      /**
       * Overlaod delete so that we can delete our memory correctly.  This is
       * necessary for DLLs on Windows to release memory from the correct
       * memory space.  All subclasses must overload delete similarly.
       */
      void operator delete(void* p)
      {
         if ( NULL != p )
         {
            PerfPlugin* obj_ptr = static_cast<PerfPlugin*>(p);
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
      PerfPlugin()
      {
         /* Do nothing. */ 
         ;
      }

      PerfPlugin(const PerfPlugin&)
      {
         /* Do nothing. */ 
         ;
      }
   };

} // namespace vrj

#endif
