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

#include <Performer/pf.h> /* IRIS Performer includes */
#include <Performer/pfutil.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfFrameStats.h>
#include <stdio.h>
#include <pfMPStats.h>



/*****************************************************************************/
/* static query   definitions                                                */
/*****************************************************************************/

#define PREV_MPSFRAME   0 /* it could change in the future */

static unsigned int AppQuery[] = {
    PFFSTATS_BUF_PREV | PFFSTATSVAL_PFTIMES_HIST_LAST_APP,
    0
};
static unsigned int IsectQuery[] = {
    PFFSTATS_BUF_PREV | PFFSTATSVAL_PFTIMES_HIST_LAST_ISECT,
    0
};
static unsigned int ChanDrawQuery[] = {
    PFFSTATS_BUF_PREV | PFFSTATSVAL_PFTIMES_HIST_LAST_DRAW,
    0
};

static unsigned int ChanCullQuery[] = {
    PFFSTATS_BUF_PREV | PFFSTATSVAL_PFTIMES_HIST_LAST_CULL,
    0
};



int pfMPStats::manageMPipeStats(int nFrames, int nSampledPipes)
{
   static int firstMPipeStats=0;
   static int testNum = 0;
   char fname [32] ;
   void *arena;

   if (!firstMPipeStats)
   {
      firstMPipeStats = 1;

      arena = pfGetSharedArena();
      MPS = (SharedMPipeStats*) pfMalloc(sizeof(SharedMPipeStats), arena);

      /* user data */
      MPS->nFrames = nFrames;
      MPS->nSampledPipes = nSampledPipes;
      MPS->mpsControl.endMPipeStats = 0;

      /* internal MPS data buffers */
      pfuCreateMPipeStatsBuffers();
      pfuInitMPipeStats();

      pfuRewindMPipeFrameStamps();
   }

   if (MPS->mpsControl.frCounter >= MPS->mpsControl.nFrames)
   {
      sprintf (fname, "MPipeStats%d.log", testNum++);
      pfuDumpMPipeStats(fname);

      /* reset the stats trace */
      pfuResetMPipeStats() ;
      pfuRewindMPipeFrameStamps() ;
      MPS->mpsControl.endMPipeStats = 0;
      firstMPipeStats = 0;

      return(0);  /* give the finish code to the requester */
   }
   else
      pfuGetMPipeFrameStamp();

   return(1);
}

int pfMPStats::pfuCreateMPipeStatsBuffers()
{
   void *arena;

   arena = pfGetSharedArena();

   /* getting the pipes/channels configuration */
   pfuGetMPipeStatsConfig();

   /* allocating frame stamp pointers array, connecting it */
   MPS->mpsFrameStamps = (pfuMPSFrameStamp *)
                         pfMalloc(MPS->mpsControl.nFrames * sizeof(pfuMPSFrameStamp),
                                  arena);

   /* allocating the data stamps, connecting them */
   /* they are intelazed in order to get cache locality */
   for ( unsigned int i = 0; i < MPS->mpsControl.nFrames; ++i )
   {
      MPS->mpsFrameStamps[i].channels =
         (pfuMPSChannelStamp*) pfMalloc(MPS->mpsConfig.nChannels * sizeof(pfuMPSChannelStamp), arena);
   }

   return 0;
}


//
// ---------- Utility Functions ------ //
//
void pfMPStats::pfuGetMPipeStatsConfig(void)
{
   int i, j, prevpipes;
   void *arena;

   arena = pfGetSharedArena();

   /* initialize it in function of the user needs */
   MPS->mpsControl.nFrames = MPS->nFrames;
   MPS->mpsControl.nPipes = MPS->nSampledPipes;

   /* getting the channel distribution of the requested pipes */
   for (i=0; i<MPS->mpsControl.nPipes; i++)
      MPS->mpsConfig.nChansPipe[i] = pfGetPipe(i)->getNumChans();

   /* calculating the total number of channels */
   MPS->mpsConfig.nChannels = 0;
   for (i=0; i<MPS->mpsControl.nPipes; i++)
      MPS->mpsConfig.nChannels += MPS->mpsConfig.nChansPipe[i];
   /* allocating for pointers list of pfChannel and pfFrameStats ptrs */
   MPS->mpsConfig.channels = (pfChannel **)
                             pfMalloc(MPS->mpsConfig.nChannels * sizeof(pfChannel *),
                                      arena);
   MPS->mpsConfig.fstats = (pfFrameStats **)
                           pfMalloc(MPS->mpsConfig.nChannels * sizeof(pfFrameStats *),
                                    arena);

   /* Getting the pointers to channels and fstats */
   prevpipes = 0;
   for (i=0; i<MPS->mpsControl.nPipes; i++)
   {
      for (j=0; j<MPS->mpsConfig.nChansPipe[i]; j++)
      {
         pfChannel* cur_chan = pfGetPipe(i)->getChan(j);
         MPS->mpsConfig.channels[j + prevpipes] = cur_chan;
         MPS->mpsConfig.fstats[j + prevpipes] = cur_chan->getFStats();
      }
      prevpipes += MPS->mpsConfig.nChansPipe[i]; /* i suppose whole use */
   }
}


int pfMPStats::pfuDestroyMPipeStatsBuffers()
{
   /* deallocating channel data stamps */
   for ( unsigned int i = 0; i < MPS->mpsControl.nFrames; ++i )
   {
      pfFree(MPS->mpsFrameStamps[i].channels);
   }

   /* deallocating frame stamp pointers array */
   pfFree(MPS->mpsFrameStamps);

   /* also dealloc the MPSControl structures */
   pfFree(MPS->mpsConfig.channels);
   pfFree(MPS->mpsConfig.fstats);

   /* final dealloc */
   pfFree(MPS);

   return 0;
}



void
pfMPStats::pfuInitMPipeStats(void)
{
   int i;

   for (i=0; i<MPS->mpsConfig.nChannels; i++)
   {
      pfFrameStats* fstats = MPS->mpsConfig.fstats[i];

      // Disable all stats first
      fstats->setClass(PFSTATS_ALL, PFSTATS_OFF);

      // enable the PFTIMES stats
      fstats->setClass(PFFSTATS_ENPFTIMES, PFSTATS_SET);

      // enable the HIST mode
      fstats->setClassMode(PFFSTATS_PFTIMES, PFFSTATS_PFTIMES_HIST, PFSTATS_SET);

      // statistics accumulation and averaging is disabled
      fstats->setAttr(PFFSTATS_UPDATE_FRAMES, 0.0f);
   }
}



void
pfMPStats::pfuResetMPipeStats(void)
{
   int i;

   for (i=0; i<MPS->mpsConfig.nChannels; i++)
   {
      /* be sure that we all stats are disabled*/
      MPS->mpsConfig.fstats[i]->setClass(PFSTATS_ALL, PFSTATS_OFF);
   }
}


int
pfMPStats::pfuGetMPipeFrameStamp(void)
{
   int i;

   /* read app and isect frame data*/
   MPS->mpsConfig.fstats[0]->mQuery(
                  AppQuery,
                  MPS->mpsFrameStamps[MPS->mpsControl.frCounter].app,
                  0);  /* it could be the size in a future release */
   MPS->mpsConfig.fstats[0]->mQuery(
                  IsectQuery,
                  MPS->mpsFrameStamps[MPS->mpsControl.frCounter].isect,
                  0); /* it could be the size in a future release */

   /* read cull and draw frame data per channel*/
   for (i=0; i<MPS->mpsConfig.nChannels; i++)
   {
      MPS->mpsConfig.fstats[i]->mQuery(
                     ChanCullQuery,
                     MPS->mpsFrameStamps[MPS->mpsControl.frCounter].channels[i].cull,
                     0); /* it could be the size in a future release */
      MPS->mpsConfig.fstats[i]->mQuery(
                     ChanDrawQuery,
                     MPS->mpsFrameStamps[MPS->mpsControl.frCounter].channels[i].draw,
                     0); /* it could be the size in a future release */
   }

   /* control status */
   MPS->mpsControl.frCounter++;
   if (MPS->mpsControl.frCounter > MPS->mpsControl.nFrames)
   {
      MPS->mpsControl.endMPipeStats = 1;
      return(0);
   }
   return(1);
}



void
pfMPStats::pfuRewindMPipeFrameStamps(void)
{
    MPS->mpsControl.frCounter = 0;
}


/* AWAITING SHARON pfDrawStats_NEW */

/*ARGSUSED*/
void
pfMPStats::pfuDrawMPipeStats(int drawchannel)
{
    /* passing the stats channel, n_channels and fstat pointer list */

    /* pfDrawStats_NEW(MPS->mpsConfig.channels[drawchannel] ,
       MPS->mpsConfig.fstats,
       MPS->mpsConfig.nChannels);*/
}

void pfMPStats::pfuDumpMPipeStats(const char *filename)
{
   int j;
   int ii[10];
   double dd[10], ddd[16];

   double avg_app_start_to_sync(0.0f);
   double avg_app_sync_clean_and_pfApp(0.0f);
   double avg_app_sync_wait(0.0f);
   double avg_app_pre_frame(0.0f);
   double avg_app_frame_clean_and_update(0.0f);
   double avg_app_frame_to_start(0.0f);

   FILE *fid;

   fid = fopen(filename, "w");
   if (!fid) fprintf(stderr, "Failure on opening MPipeStats dump file\n");

   /* creating the dump file header for mpscope */

   ii[0] = MPS->mpsControl.nFrames;
   ii[1] = MPS->mpsControl.nPipes;
   ii[2] = MPS->mpsConfig.nChannels;
   ii[3] = MPS->mpsConfig.nChansPipe[0];
   ii[4] = MPS->mpsConfig.nChansPipe[1];
   ii[5] = MPS->mpsConfig.nChansPipe[2];
   fprintf(fid, headerFormat, ii[0], ii[1], ii[2], ii[3], ii[4], ii[5]);

   /* creating the time stamps */

   for ( unsigned int i = 0; i < MPS->mpsControl.nFrames; ++i )
   {
      // Get app timings
      const int    app_frame = MPS->mpsFrameStamps[i].app[PREV_MPSFRAME].frame;
      const double app_start = MPS->mpsFrameStamps[i].app[PREV_MPSFRAME].start;
      const double app_enter_sync = MPS->mpsFrameStamps[i].app[PREV_MPSFRAME].enterSync;
      const double app_after_clean = MPS->mpsFrameStamps[i].app[PREV_MPSFRAME].afterClean;
      const double app_after_sync = MPS->mpsFrameStamps[i].app[PREV_MPSFRAME].afterSync;
      const double app_pfframe_start = MPS->mpsFrameStamps[i].app[PREV_MPSFRAME].pfFrameStart;
      const double app_pfframe_end = MPS->mpsFrameStamps[i].app[PREV_MPSFRAME].pfFrameEnd;

      // Get values from other timing that we need
      /*
      double prev_pfframe_end;
      if(i>0)
         prev_pfframe_end = MPS->mpsFrameStamps[i-1].app[PREV_MPSFRAME].pfFrameEnd;
      else
         prev_pfframe_end = 0.0f;
      */
      double next_app_start;
      if((i+1)<MPS->mpsControl.nFrames)
         next_app_start = MPS->mpsFrameStamps[i+1].app[PREV_MPSFRAME].start;
      else
         next_app_start = 0.0f;

      // Calculate some app process duration info
      const double app_start_to_sync =          (app_enter_sync   -app_start)*1000.0f;
      const double app_sync_clean_and_pfApp =   (app_after_clean  -app_enter_sync)*1000.0f;
      const double app_sync_wait =              (app_after_sync   -app_after_clean)*1000.0f;
      const double app_pre_frame =              (app_pfframe_start-app_after_sync)*1000.0f;
      const double app_frame_clean_and_update = (app_pfframe_end  -app_pfframe_start)*1000.0f;
      const double app_frame_to_start =         (next_app_start   -app_pfframe_end)*1000.0f;
      // Update averages
      // Don't use first frame though,(bad data)
      if((i!=0) && ((i+1)<MPS->mpsControl.nFrames))     // While not first or last frame
      {
         avg_app_start_to_sync            += app_start_to_sync;
         avg_app_sync_clean_and_pfApp     += app_sync_clean_and_pfApp;
         avg_app_sync_wait                += app_sync_wait;
         avg_app_pre_frame                += app_pre_frame;
         avg_app_frame_clean_and_update   += app_frame_clean_and_update;
         avg_app_frame_to_start           += app_frame_to_start;
      }

      int isect_frame = ii[1] = MPS->mpsFrameStamps[i].isect[PREV_MPSFRAME].frame;
      double isect_start = dd[6] = MPS->mpsFrameStamps[i].isect[PREV_MPSFRAME].start;
      double isect_end = dd[7] = MPS->mpsFrameStamps[i].isect[PREV_MPSFRAME].end;

      double isect_duration = (isect_end - isect_start) * 1000.0f;

      /* saving in file */
      fprintf( fid, appCounterFormat, frameString, i);
      fprintf( fid, appFormat,  appString, absoluteString,
                    app_frame, app_start, app_enter_sync, app_after_clean, app_after_sync,app_pfframe_start, app_pfframe_end);
      fprintf( fid, appDurationFormat, durationString, i,
                    app_start_to_sync, app_sync_clean_and_pfApp, app_sync_wait,
                    app_pre_frame, app_frame_clean_and_update,
                    app_frame_to_start);
      fprintf( fid, isectFormat, isectString, absoluteString, isect_frame, isect_start, isect_end, relativeString, isect_duration);

      for (j=0; j<MPS->mpsConfig.nChannels; j++)
      {
         ii[0] = MPS->mpsFrameStamps[i].channels[j].cull[PREV_MPSFRAME].frame;
         dd[0] = MPS->mpsFrameStamps[i].channels[j].cull[PREV_MPSFRAME].beginUpdate;
         dd[1] = MPS->mpsFrameStamps[i].channels[j].cull[PREV_MPSFRAME].endUpdate;
         dd[2] = MPS->mpsFrameStamps[i].channels[j].cull[PREV_MPSFRAME].start;
         dd[3] = MPS->mpsFrameStamps[i].channels[j].cull[PREV_MPSFRAME].end;

         ddd[0] = (MPS->mpsFrameStamps[i].channels[j].cull[PREV_MPSFRAME].end -
                   MPS->mpsFrameStamps[i].channels[j].cull[PREV_MPSFRAME].start) * 1000.0f;

         ii[1] = MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].frame;
         dd[4] = MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].start;
         dd[5] = MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].end;
         dd[6] = MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].pfDrawStart;
         dd[7] = MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].pfDrawEnd;
         dd[8] = MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].afterSwap;
         dd[9] = (MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].pfDrawEnd -
                  MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].pfDrawStart);

         ddd[1] = (MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].end -
                   MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].start) * 1000.0f;
         /* predraw channel callback */
         ddd[2] = (MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].pfDrawStart -
                   MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].start) * 1000.0f;
         /* pfDraw */
         ddd[3] = (MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].pfDrawEnd -
                   MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].pfDrawStart) * 1000.0f;
         /* postdraw channel callback */
         ddd[4] = (MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].end -
                   MPS->mpsFrameStamps[i].channels[j].draw[PREV_MPSFRAME].pfDrawEnd) * 1000.0f;

         /* dumping in file */
         fprintf( fid, channelsFormat1, cdString, j, i, absoluteString, ii[0], dd[0], dd[1], dd[2], dd[3]);
         fprintf( fid, channelsFormat2, ii[1], dd[4], dd[5], dd[6], dd[7], dd[8]);
         fprintf( fid, channelsFormat3, relativeString, ddd[0], ddd [1], ddd[2], ddd[3], ddd[4]);
      }
   }

   // Update averages
   double num_frames = (MPS->mpsControl.nFrames-2);    // -2 because we don't count first or last frame
   avg_app_start_to_sync /= num_frames;
   avg_app_sync_clean_and_pfApp /= num_frames;
   avg_app_sync_wait /= num_frames;
   avg_app_pre_frame /= num_frames;
   avg_app_frame_clean_and_update /= num_frames;
   avg_app_frame_to_start /= num_frames;
   fprintf( fid, "\n\nApp Averages:\n");
   fprintf( fid, appDurationFormat, durationString, MPS->mpsControl.nFrames - 2,
                 avg_app_start_to_sync, avg_app_sync_clean_and_pfApp,
                 avg_app_sync_wait, avg_app_pre_frame,
                 avg_app_frame_clean_and_update, avg_app_frame_to_start);

   fclose(fid);
}

