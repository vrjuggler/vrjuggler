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
#ifndef _JCCL_PERFORMANCE_MONITOR_H_
#define _JCCL_PERFORMANCE_MONITOR_H_

#include <jccl/jcclConfig.h>
//#include <jccl/Net/JackalControl.h>
#include <jccl/RTRC/ConfigChunkHandler.h>
#include <jccl/Config/ConfigChunk.h>
#include <jccl/PerfMonitor/LabeledPerfDataBuffer.h>
#include <jccl/PerfMonitor/PerformanceCategories.h>
#include <vpr/Thread/TSObjectProxy.h>
#include <vpr/Thread/Thread.h>
#include <vpr/Util/Singleton.h>

namespace jccl
{

//class Connect;
//class JackalServer;
//class PeriodicCommand;


/** Jackal Performance-monitoring agent.
 *
 *  @version $Revision$, $Date$
 */
//class JCCL_CLASS_API PerformanceMonitor: public JackalControl,
//                                         public ConfigChunkHandler
class JCCL_CLASS_API PerformanceMonitor: public ConfigChunkHandler
{

public:

   /** Constructor. */
   PerformanceMonitor();


   /** Destructor. */
   virtual ~PerformanceMonitor();


   /** Get access to the thread-specific labeled buffers. */
   inline LabeledPerfDataBuffer* getLabeledTSBuffer ()
   {
      return &(*mTSBuffers);
   }


/*
   //--------------- JackalControl Stuff -----------------------

   virtual void addConnect (Connect *c);

   virtual void removeConnect (Connect* c);
*/

   //---------------- ConfigChunkHandler Stuff -----------------

   virtual bool configAdd(ConfigChunkPtr chunk);

   virtual bool configRemove(ConfigChunkPtr chunk);

   virtual bool configCanHandle(ConfigChunkPtr chunk);



private:

   std::string               mPerfTargetName;
//   Connect*                  mPerfTarget;
   float                     mPerfRefreshTime;  // in milliseconds
   ConfigChunkPtr            mCurrentPerfConfig;

   vpr::TSObjectProxy<LabeledPerfDataBuffer> mTSBuffers;

//   PeriodicCommand* mBuffersCommand;

   void activatePerfBuffers();
   void deactivatePerfBuffers();

//   void setPerformanceTarget (Connect* con);


   // These are needed to appease Visual C++ in its creation of DLLs.
//   PerformanceMonitor(const PerformanceMonitor&)
//      : JackalControl(), ConfigChunkHandler()
//   {;}
   PerformanceMonitor(const PerformanceMonitor&)
      : ConfigChunkHandler()
   {;}
   void operator=(const PerformanceMonitor&) {;}

   vprSingletonHeader(PerformanceMonitor);

}; // end PerformanceMonitor

} // End of jccl nameapce


#endif
