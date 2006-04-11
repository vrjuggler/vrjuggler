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
// implementation of Performance Monitor
//
#include <jccl/PerfMonitor/PerformanceMonitor.h>
#include <jccl/PerfMonitor/PerformanceCategories.h>
/*
#include <jccl/Net/JackalServer.h>
#include <jccl/Net/Connect.h>
#include <jccl/PerfMonitor/PerfCommand.h>
*/
#include <jccl/Config/ConfigChunk.h>
#include <jccl/Config/ParseUtil.h>


namespace jccl {

vprSingletonImp(PerformanceMonitor);


PerformanceMonitor::PerformanceMonitor()
{
   mPerfRefreshTime = 500;

   mPerfTargetName = "";
//   mPerfTarget = NULL;

//   mBuffersCommand = new CommandWriteLabeledPerfData (mPerfRefreshTime);
}



PerformanceMonitor::~PerformanceMonitor()
{
//   delete mBuffersCommand;
}


/*
void PerformanceMonitor::addConnect (Connect* con)
{
   vprASSERT (con != NULL);

   if (con->getName() == mPerfTargetName)
   {
      setPerformanceTarget (con);
   }
}


void PerformanceMonitor::removeConnect (Connect* con)
{
   vprASSERT (con != NULL);

   if (con == mPerfTarget)
      setPerformanceTarget (NULL);
}
*/


//---------------------- ConfigChunkHandler Stuff ----------------------

bool PerformanceMonitor::configAdd(ConfigChunkPtr chunk)
{
   std::string s = chunk->getDescToken();
   if ( s == std::string("PerfMeasure") )
   {
      mCurrentPerfConfig = chunk;//new ConfigChunk (*chunk);

      mPerfTargetName = chunk->getProperty<std::string>("PerformanceTarget");
/*
      if ((mPerfTarget == 0) || mPerfTarget->getName() == mPerfTargetName)
      {
         std::vector<Connect*>& connections =
            JackalServer::instance()->getConnections();
         for (unsigned int i = 0; i < connections.size(); i++)
         {
            if ( connections[i]->getName() == mPerfTargetName )
            {
               setPerformanceTarget (connections[i]);
               break;
            }
         }
         JackalServer::instance()->releaseConnections();
      }
      else
      {
*/
         // just activate buffers to pick up changes to individual
         // buffer activation states.
         activatePerfBuffers();
/*
      }
*/

      return true;
   }

   return false;
}



bool PerformanceMonitor::configRemove(ConfigChunkPtr chunk)
{
   std::string s = chunk->getDescToken();
   if ( s == std::string("PerfMeasure") )
   {
      if (mCurrentPerfConfig.get())
      {
         if ( mCurrentPerfConfig->getName() == chunk->getName() )
         {
            mCurrentPerfConfig.reset();
            deactivatePerfBuffers ();
         }
      }
      return true;
   }
   return false;
}



bool PerformanceMonitor::configCanHandle(ConfigChunkPtr chunk)
{
   return chunk->getDescToken() == std::string("PerfMeasure");
}



//-------------------- PRIVATE MEMBER FUNCTIONS -------------------------


/*
void PerformanceMonitor::setPerformanceTarget (Connect* con)
{
   if (con == mPerfTarget)
   {
      return;
   }
   deactivatePerfBuffers();
   mPerfTarget = con;
   activatePerfBuffers();
}
*/


void PerformanceMonitor::deactivatePerfBuffers ()
{
   PerformanceCategories::instance()->deactivate();
/*
   if (mPerfTarget)
   {
            mPerfTarget->removePeriodicCommand (mBuffersCommand);
   }
*/
}



void PerformanceMonitor::activatePerfBuffers ()
{
   // activates all perf buffers configured to do so
   // this is still a bit on the big and bulky side.

//   if (mPerfTarget == NULL || mCurrentPerfConfig.get() == NULL)
   if (mCurrentPerfConfig.get() == NULL)
   {
      deactivatePerfBuffers();
      return;
   }

   /* individually enable/disable the buffers */
   ConfigChunkPtr ch;
   int category_count = mCurrentPerfConfig->getNum("PerfCategories");

   for ( int i = 0; i < category_count; i++ )
   {
      ch = mCurrentPerfConfig->getProperty<ConfigChunkPtr>("PerfCategories", i);

      if ( ch->getProperty<bool>("Enabled") )
      {
         PerformanceCategories::instance()->activateCategory(ch->getProperty<std::string>("Prefix"));
      }
      else
      {
         PerformanceCategories::instance()->deactivateCategory(ch->getProperty<std::string>("Prefix"));
      }
   }

   PerformanceCategories::instance()->activate();
/*
   if (mPerfTarget)
   {
      mPerfTarget->addPeriodicCommand (mBuffersCommand);
   }
*/
}

}
