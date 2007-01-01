/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _PF_MP_STATS_H_
#define _PF_MP_STATS_H_

#include <Performer/pf.h> /* IRIS Performer includes */
#include <Performer/pfutil.h>
#include <Performer/pf/pfFrameStats.h>

/****************************************************************************/
/*     STRUCTS for MP Data                                                  */
/* User buffer declarations for historical frame log                        */
/****************************************************************************/

typedef struct {
    double                 start;          /* quad align */
    pfFStatsValPFTimesCull cull[PFFSTATS_MAX_HIST_FRAMES];    /* channel cull */
    pfFStatsValPFTimesDraw draw[PFFSTATS_MAX_HIST_FRAMES];    /* channel draw */
} pfuMPSChannelStamp;

typedef struct {
    double                    start;
    pfFStatsValPFTimesApp     app[PFFSTATS_MAX_HIST_FRAMES];
    pfFStatsValPFTimesIsect   isect[PFFSTATS_MAX_HIST_FRAMES];
    pfuMPSChannelStamp*       channels;
} pfuMPSFrameStamp;

// User configuration information
typedef struct {
    int              nChannels;
    int              nChansPipe[3];
    pfChannel        **channels;  /* pointers list to pfChannel pointer */
    pfFrameStats     **fstats;    /* pointers list to fstats channel pointer */
} pfuMPSConfiguration;

// MPipeStats control structure
typedef struct {
    unsigned int  nFrames;          /* nframes requested, USER USE        */
    int           nPipes;           /* number of gfx pipes sampled, USER USE */
    unsigned int  frCounter;        /* internal MPipeStats counter        */
    int           endMPipeStats;    /* flag of completion           */
} pfuMPSControl;


// multiprocess shared data structure
typedef struct
{
    pfuMPSControl          mpsControl;       /* mps control data      */
    pfuMPSConfiguration    mpsConfig;        /* pipes/channels config    */
    pfuMPSFrameStamp       *mpsFrameStamps;  /* user memory stats  buffer */
    int                    nFrames;
    int                    nSampledPipes;    /* user input */
} SharedMPipeStats;



//------------------------------------------------
//: Class for getting multipipe/channel pfStats
//
// Based on mpstats code in libpfutil
//
// Created: 12-13-1999
// Author: Allen Bierbaum
//------------------------------------------------
class pfMPStats
{
public:
   int init(int nFrames, int nSampledPipes)
   {
      if(!mRunning)
      {
         mNumFrames = nFrames;
         mNumPipes = nSampledPipes;
         mRunning = pfuManageMPipeStats(mNumFrames, mNumPipes);
         return 1;
      }
      else
         return 0;
   }

   int update()
   {
      if(mRunning)
         mRunning = manageMPipeStats(mNumFrames, mNumPipes);

      return mRunning;
   }

   bool isRunning()
   { return mRunning; }

private:
   int   mRunning;    //: Is a collection currently running
   int   mNumFrames;
   int   mNumPipes;

protected:
   //  This utility obtains time stamp stats for each Performer
   //  channel/pipe in an automatic fashion. The complete
   //  stats log is stored in a file for later review. Timing
   //  information for pre- and post-callbacks is also provided.
   //
   //  The routine pfuManageMPipeStats can be used to obtain
   //  timing information during a number of frames per
   //  each channel for pre-draw, pfDraw, post- draw, pre-cull,
   //  pfCull, post-cull as well as ISECT and APP.
   //
   //  nframes specifies the number of frames to be measured.
   //  nSampledPipes is the number of pipes for which
   //  statistics are to be gathered.
   //
   //  The first time that pfuManageMPipeStats is called it
   //  automatically determines the pipe and channel
   //  configuration. It also sets up a minimally configured
   //  pfStats in order to obtain the data.
   //
   //  During the requested number of frames, the utility grabs
   //  the latest pfStats buffer into main memory. After
   //  the requested number of frames, the utility creates a
   //  new file called MPipeStats0.log. This the file number
   //  is incremented each time that the utility is used from
   //  the same execution.
   //
   //  pfuManageMPipeStats returns 1 while it is getting time
   //  stamps and 0 when it has finished his collection
   //  and created the historical file.
   //
   //  pfuManageMPipeStats must be called in the APP and it
   //  could be used multiple times.
   int manageMPipeStats(int nFrames, int nSampledPipes);

   // allocates MPipeStats user buffers
   int pfuCreateMPipeStatsBuffers(void);

   // frees MPipeStats user buffers
   int pfuDestroyMPipeStatsBuffers(void);

   // Gets configuration, init stats
   void pfuInitMPipeStats(void);

   // Stops the MPipeStats sampling
   void pfuResetMPipeStats(void);

   // Grab MPipeStats, update frCounter
   // returns 0 if it finished
   int pfuGetMPipeFrameStamp(void);

   // frCounter = 0
   void pfuRewindMPipeFrameStamps(void);

   // Draw MPipeStats on req. channel
   void pfuDrawMPipeStats(int);

   // Dumps user buffers into file
   void pfuDumpMPipeStats(const char *);

   // internal API
   void pfuGetMPipeStatsConfig(void);

private:
   SharedMPipeStats* MPS;


   pfFStatsValPFTimesApp dts_app;
   pfFStatsValPFTimesIsect dts_isect;
   pfFStatsValPFTimesCull dts_cull;
   pfFStatsValPFTimesDraw dts_draw;
};


/*****************************************************************************/
/* dump file formats                                                         */
/*****************************************************************************/
/* STRINGS */
const char frameString[] =
    "============================================================================\n";
const char absoluteString[] =
    "Absolute Timeline information (sec)\n...................................\n";
const char relativeString[] =
    "Relative Timeline information (sec)\n...................................\n";
const char durationString[] =
    "Duration information (msec)\n..................................\n";
const char appString[] =
    "_____________________________APP____________________________________________\n";
const char isectString[] =
    "_____________________________ISECT__________________________________________\n";
const char cdString[] =
    "_____________________________CULL & DRAW____________________________________\n";

/* FORMATS */
const char headerFormat[] =
    "Nframes:%i Npipes:%i \n NChannels:%i Pipe0:%i Pipe1:%i Pipe2:%i\n";
const char appCounterFormat[] = "%sFrame: %i\n";
const char appFormat[] =
    "%s%sAppFrame: %i\nAppStart: %g\nenterSync: %g afterClean: %g afterSync: %g pfFrameStart:%g pfFrameEnd: %g\n";
const char appDurationFormat[] =
    "%sAppDuration: %i\nstartToSync:%g syncCleanAndApp:%g syncWait:%g syncToFrame(critical):%g frameClean:%g frameToStart:%g\n";
const char isectFormat[] =
    "%s%sIsectFrame: %i\nStart: %g End: %g\n\n%sTotal Isect Time: %g\n";
const char channelsFormat1[] =
    "%sChan: %i\nFrame: %i\n\n%sCullFrame: %i\n   BeginUpDate: %g EndUpdate: %g\n   Start: %g End: %g\n";
const char channelsFormat2[]=
    "DrawFrame: %i\n   Start: %g End: %g\n   pfDrawStart: %g pfDrawEnd: %g AfterSwap: %g\n\n";
const char channelsFormat3[] =
    "%sTotal Cull Time: %g\nTotal Draw Time: %g\n   PreDraw: %g\n   pfDraw: %g\n   PostDraw: %g\n";

#if 0
/*****************************************************************************/
/* Only for information                                  */
/* internal stats structures defined in pfstats.h                       */
/*****************************************************************************/

typedef struct pfFStatsValHistIndex /* PFFSTATSVAL_PFTIMES_HIST_INDEX   */
{   /* indecies into time stamp history arrays for each process */
    int  frame;
    int  app;
    int  cull;
    int  draw;
    int  isect;
} pfFStatsValHistIndex;

typedef struct pfFStatsValPFTimesApp   /* PFFSTATSVAL_PFTIMES_HIST_ISECT   */
{
    int  frame;         /* app frame stamp */
    double  start;      /* start */
    double  enterSync;  /* time enter pfSync */
    double  afterClean; /* time after clean in pfSync - before sleep */
    double  afterSync;  /* time return from pfSync */
    double  pfFrameStart, pfFrameEnd;
} pfFStatsValPFTimesApp;

typedef struct pfFStatsValPFTimesCull  /* PFFSTATSVAL_PFTIMES_HIST_CULL */
{
    int  frame;         /* app frame stamp */
    double  beginUpdate, endUpdate; /* update of node changes from APP */
    double  start;      /* start cull - before call pfChanCullFunc() callback */
    double  end;     /* end cull - after return from pfChanCullFunc() callback */
} pfFStatsValPFTimesCull;

typedef struct pfFStatsValPFTimesDraw  /* PFFSTATSVAL_PFTIMES_HIST_DRAW    */
{
    int  frame; /* app frame stamp */
    double  start;      /* start draw - before pfChanDrawFunc() callback */
    double  end;     /* end draw - after return from pfChanDrawFunc() callback */
    double  pfDrawStart;   /* time of start of pfDraw() */
    double  pfDrawEnd;  /* time returning from pfDraw() */
    double  afterSwap;  /* time pipe swapbuffers() finishes (est.) */
} pfFStatsValPFTimesDraw;

typedef struct pfFStatsValPFTimesIsect /* PFFSTATSVAL_PFTIMES_HIST_ISECT */
{
    int  frame; /* app frame stamp */
    double  start;   /* time before call pfIsectFunc() callback */
    double  end;  /* time after call pfIsectFunc() callback */
} pfFStatsValPFTimesIsect;

/******************************************************** end of info */
#endif

#endif
