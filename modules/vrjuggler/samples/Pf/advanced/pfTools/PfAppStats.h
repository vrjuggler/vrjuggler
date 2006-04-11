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

#ifndef _VRJ_PF_APP_STATS_
#define _VRJ_PF_APP_STATS_

#include <gadget/Type/DigitalInterface.h>
#include <gadget/Type/DigitalProxy.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pfutil.h>
#include <pfMPStats.h>

namespace vrj
{
   
class PfAppStats
{
public:
   enum statMode {
      NONE, FrameTime, Gfx, Pipe, MPipe, Fill
   };

   // The data that has to be shared across the funtions
   class statsSharedData
   {
   public:
      statsSharedData()
      { mDrawStatsMode = PfAppStats::NONE;}

      int mDrawStatsMode;              // Should we draw the stats
   };

public:
   // Empty virtual destructor to make GCC happy.
   virtual ~PfAppStats (void) {
      /* Do nothing. */ ;
   }

   // Must be called in preForkInit
   // -Allocates the shared data buffer
   // -Initializes stats mode
   void preForkInit();

   // Must be called as part of postFrame
   // -Reset change trigger flag
   virtual void postFrame();

   // Must be called by appChanFunc of application
   // -Sets the stats state for each channel for next frame
   //
   // Called immediately before draw (pfFrame()) for each channel
   // This tells the channel to collect stats (or not to)
   virtual void appChanFunc(pfChannel* chan);

   // Must be called as part of preDrawChan in application
   // POST: Stats draw trigger set
   void preDrawChan(pfChannel* chan, void* chandata);

   // Must be called at some point in app preFrame
   // Updates the stats state based upon pressing the mStatsButton
   // Also updates MPStats if that is turned on
   void preFrame();

   PfAppStats::statMode getStatsMode()
   { return mStatsMode; }

   // Set the new statistics mode
   // Must be called during preFrame() processing either by local preFrame or user's preFrame()
   void setStatsMode(PfAppStats::statMode mode);

   void setToggleButton(std::string button_name)
   {
      mStatsButton.init(button_name);
      vprDEBUG(vprDBG_ALL,0)
         << clrOutNORM(clrCYAN,"vjPfAppStats::toggleButton set to:")
         << button_name.c_str() << std::endl << vprDEBUG_FLUSH;
   }

protected:  // -- Helper functions --- //

   //: Set the statistic info for the given channel
   // This function must be called from the app process
   void setChanStatsState(pfChannel* chan);

private:
   // Stats info
   gadget::DigitalInterface      mStatsButton;           // Button used to cycle the stats info
   bool                    mStatsConfigChange;     // Has the stats stuff been change
   PfAppStats::statMode  mStatsMode;             // The current stats mode
   pfMPStats               mMPStats;               // The Multi-pipe stats
   statsSharedData*        mSharedData;            // The shared data

};


// Set the new statistics mode
// Must be called during preFrame() processing either by local preFrame or user's preFrame()
inline void PfAppStats::setStatsMode(PfAppStats::statMode mode)
{
   mStatsConfigChange = true;
   mStatsMode = mode;

   switch(mode)
   {
      case PfAppStats::NONE:
         vprDEBUG(vprDBG_ALL,0) << "vjPfAppStats: Switching to NONE Stats mode.\n" << vprDEBUG_FLUSH;
         break;
      case PfAppStats::FrameTime:
         vprDEBUG(vprDBG_ALL,0) << "vjPfAppStats: Switching to FrameTime Stats mode.\n" << vprDEBUG_FLUSH;
         break;
      case PfAppStats::Gfx:
         vprDEBUG(vprDBG_ALL,0) << "vjPfAppStats: Switching to Gfx Stats mode.\n" << vprDEBUG_FLUSH;
         break;
      case PfAppStats::Pipe:
         vprDEBUG(vprDBG_ALL,0) << "vjPfAppStats: Switching to Pipe Stats mode.\n" << vprDEBUG_FLUSH;
         break;
      case PfAppStats::MPipe:
         vprDEBUG(vprDBG_ALL,0) << "vjPfAppStats: Switching to MPipe Stats mode.\n" << vprDEBUG_FLUSH;
         break;
      case PfAppStats::Fill:
         vprDEBUG(vprDBG_ALL,0) << "vjPfAppStats: Switching to Fill Stats mode.\n" << vprDEBUG_FLUSH;
         break;
      default:
         vprDEBUG(vprDBG_ALL,0) << "vjPfAppStats: Switching to unknown Stats mode.\n" << vprDEBUG_FLUSH;
         break;
   }

   // if it is MPStats, then we need to init
   if(mStatsMode == PfAppStats::MPipe)
   {
      mMPStats.init(120,1);
      vprDEBUG(vprDBG_ALL,0) << clrOutNORM(clrGREEN,"--> MP Stats mode starting!")
                           << std::endl << vprDEBUG_FLUSH;
   }
}

//: Set the statistic info for the given channel
// This function must be called from the app process
inline void PfAppStats::setChanStatsState(pfChannel* chan)
{
   pfFrameStats* fstats = chan->getFStats();    // Get stats structure

   if(mStatsMode == PfAppStats::MPipe)
   {
      /* Do Nothing */
   }
   else if(mStatsMode != PfAppStats::NONE)
   {
      mSharedData->mDrawStatsMode = mStatsMode;        // We want to draw stats now

      int stats_enable(0);
	  
      // Perfly stat modes
      if(mStatsMode == PfAppStats::FrameTime)
         stats_enable = PFFSTATS_ENPFTIMES;
      if(mStatsMode == PfAppStats::Gfx)
         stats_enable = PFFSTATS_ENPFTIMES | PFSTATS_ENGFX | PFSTATS_ENTEXLOAD | PFFSTATS_ENDB | PFFSTATS_ENCULL;
      if(mStatsMode == PfAppStats::Pipe)
         stats_enable = PFFSTATS_ENGFXPFTIMES;
      if(mStatsMode == PfAppStats::Fill)
         stats_enable = PFSTATSHW_ENGFXPIPE_FILL | PFFSTATS_ENPFTIMES;


      fstats->setClass(stats_enable, PFSTATS_ON);                       // Enable some stats
      fstats->setClassMode(PFFSTATS_PFTIMES, PFFSTATS_PFTIMES_MASK, PFSTATS_ON);
      if (stats_enable & PFSTATSHW_ENGFXPIPE_TIMES) /* do just the status line */
         chan->setStatsMode(PFCSTATS_DRAW, 0);
      else
         chan->setStatsMode(PFCSTATS_DRAW, PFSTATS_ALL);


      /* set some stats class modes that are not
       * on by default. this only needs to be done
       * the first time */
      fstats->setClassMode(PFSTATSHW_GFXPIPE_FILL,
            PFSTATSHW_GFXPIPE_FILL_DEPTHCMP |
            PFSTATSHW_GFXPIPE_FILL_TRANSPARENT,
            PFSTATS_ON);
      /*
      * This will also enable the tmesh statistics - not on by
      * default because they are somewhat expensive
      */
      fstats->setClassMode(PFSTATS_GFX,
            PFSTATS_GFX_ATTR_COUNTS |
            PFSTATS_GFX_TSTRIP_LENGTHS,
            PFSTATS_ON);
      fstats->setAttr(PFFSTATS_UPDATE_SECS, 2.0f);

   }
   else
   {
      mSharedData->mDrawStatsMode = PfAppStats::NONE;
      chan->setStatsMode(PFCSTATS_DRAW,0);
      fstats->setClass(PFSTATS_ALL, PFSTATS_DEFAULT); // Set to default
      fstats->setClassMode(PFFSTATS_PFTIMES, PFFSTATS_PFTIMES_BASIC, PFSTATS_SET);   /* set the fast process timing stats */
   }
}


// Must be called in preForkInit
// -Allocates the shared data buffer
// -Initializes stats mode
inline void PfAppStats::preForkInit()
{
   // Allocate space for shared data buffer
   void* myAppDataBuffer = pfCalloc(1,sizeof(statsSharedData),pfGetSharedArena());
   mSharedData = new(myAppDataBuffer) statsSharedData();

   setStatsMode(PfAppStats::NONE);
}

// Must be called as part of postFrame
// -Reset change trigger flag
inline void PfAppStats::postFrame()
{
   if(mStatsConfigChange)        // We just updated this during previous frame
      mStatsConfigChange = false;
}

// Must be called by appChanFunc of application
// -Sets the stats state for each channel for next frame
// Called immediately before draw (pfFrame()) for each channel
// This tells the channel to collect stats
inline void PfAppStats::appChanFunc(pfChannel* chan)
{
   // Set the statistics collection state
   if(mStatsConfigChange)
      setChanStatsState(chan);
}

// Must be called as part of preDrawChan in application
// POST: Stats draw trigger set
inline void PfAppStats::preDrawChan(pfChannel* chan, void* chandata)
{
   if(mSharedData->mDrawStatsMode != PfAppStats::NONE &&
      mSharedData->mDrawStatsMode != PfAppStats::Pipe)
      chan->drawStats();
}


// Must be called at some point in app preFrame
// Updates the stats state based upon pressing the mStatsButton
// Also updates MPStats if that is turned on
inline void PfAppStats::preFrame()
{
   // If we are using MPipe Stats, then we have to update the params
   if(mStatsMode == PfAppStats::MPipe)
   {
      if(0 == mMPStats.update())       // Is it done updating
      {
         vprDEBUG(vprDBG_ALL,0) << "vjPfAppStats: MP Stats mode done!\n" << vprDEBUG_FLUSH;
         setStatsMode(PfAppStats::Fill);    // Go to next mode
      } else { std::cout << ".." <<std::flush; }
   }

   // Check for STATS CYCLE Button
   if(mStatsButton->getData() == gadget::Digital::TOGGLE_ON)
   {
      vprDEBUG(vprDBG_ALL,0) << clrOutNORM(clrGREEN,"vjPfAppStats: mStatsButton pressed. Turning on or cycling stats.\n") << vprDEBUG_FLUSH;

      PfAppStats::statMode cur_stat_mode = getStatsMode();
      switch(cur_stat_mode)
      {
      /*
      case PfAppStats::NONE: setStatsMode(PfAppStats::FrameTime); break;
      case PfAppStats::FrameTime: setStatsMode(PfAppStats::Gfx); break;
      case PfAppStats::Gfx: setStatsMode(PfAppStats::Pipe); break;
      case PfAppStats::Pipe: setStatsMode(PfAppStats::MPipe); break;
      case PfAppStats::MPipe: setStatsMode(PfAppStats::Fill); break;     // setStatsMode initializes it
      case PfAppStats::Fill: setStatsMode(PfAppStats::NONE); break;
      */
      case PfAppStats::NONE: setStatsMode(PfAppStats::FrameTime); break;
      case PfAppStats::FrameTime: setStatsMode(PfAppStats::Gfx); break;
      case PfAppStats::Gfx: setStatsMode(PfAppStats::NONE); break;
      default: setStatsMode(PfAppStats::NONE); break;
      }
   }
}




};

#endif
