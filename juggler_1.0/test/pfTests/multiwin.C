//
// Copyright 1995, Silicon Graphics, Inc.
// ALL RIGHTS RESERVED
//
// UNPUBLISHED -- Rights reserved under the copyright laws of the United
// States.   Use of a copyright notice is precautionary only and does not
// imply publication or disclosure.
//
// U.S. GOVERNMENT RESTRICTED RIGHTS LEGEND:
// Use, duplication or disclosure by the Government is subject to restrictions
// as set forth in FAR 52.227.19(c)(2) or subparagraph (c)(1)(ii) of the Rights
// in Technical Data and Computer Software clause at DFARS 252.227-7013 and/or
// in similar or successor clauses in the FAR, or the DOD or NASA FAR
// Supplement.  Contractor/manufacturer is Silicon Graphics, Inc.,
// 2011 N. Shoreline Blvd. Mountain View, CA 94039-7311.
//
// Permission to use, copy, modify, distribute, and sell this software
// and its documentation for any purpose is hereby granted without
// fee, provided that (i) the above copyright notices and this
// permission notice appear in all copies of the software and related
// documentation, and (ii) the name of Silicon Graphics may not be
// used in any advertising or publicity relating to the software
// without the specific, prior written permission of Silicon Graphics.
//
// THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
//
// IN NO EVENT SHALL SILICON GRAPHICS BE LIABLE FOR ANY SPECIAL,
// INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY
// DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY
// THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE
// OR PERFORMANCE OF THIS SOFTWARE.
//
//
// multiwinC.C: Performer program to demonstrate multiple windows
//              in one pipe.  Derived from simple.c
//
// $Revision$
// $Date$
//
//


#include <stdlib.h>
#include <iostream.h>
#include <Performer/pf.h>
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pr/pfLight.h>
#include <Performer/pf/pfTraverser.h>
#include <Performer/pfdu.h>

static void OpenPipeWin(pfPipeWindow *pw);
void chanAppTravFunc(pfChannel *chan, void* chandata);
int nodeTravAppPre(pfTraverser* trav, void* userData);
int nodeTravAppPost(pfTraverser* trav, void* userData);

void openSomeWindows();
pfPipe* getPipeNum(const int pipe_num);
void initPipes();

const int NUM_WINS = 12;             // Number of windows total
const int NUM_OPEN_AT_ONCE = 2;     // Number of windows to open at once
const int NUM_PIPES = 2;            // Total number of pipes to use

char* pipeStrs[6] = { ":0.0", ":1.0", ":2.0", ":3.0", ":4.0", ":5.0" };

pfPipe* pipes[8] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};


//
// Usage() -- print usage advice and exit. This
//      procedure is executed in the application process.
//
static void
Usage (void)
{
    pfNotify(PFNFY_FATAL, PFNFY_USAGE, "Usage: multiwinC file.ext ...\n");
    exit(1);
}

pfPipeWindow*  gPwin[16];
pfChannel*     gChan[16];
int            gNumWinActive = 0;
pfScene*       gScene = NULL;
pfSphere       bsphere;

int
main (int argc, char *argv[])
{
   int  loop;
   float       t = 0.0f;


   if (argc < 2)
      Usage();

   // Initialize Performer
   pfInit();

   // Setup number of pipes
   pfMultipipe(NUM_PIPES);

   // Use default multiprocessing mode based on number of
   // processors.
   //
   pfMultiprocess( PFMP_DEFAULT );

   // Load all loader DSO's before pfConfig() forks
   pfdInitConverter(argv[1]);

   // Configure multiprocessing mode and start parallel
   // processes.
   //
   pfConfig();


   // Get all the pipes that I need
   initPipes();

   // Append to PFPATH additional standard directories where
   // geometry and textures exist
   //
   pfFilePath(".:/usr/share/Performer/data");

   // Read a single file, of any known type.
   pfNode *root = pfdLoadFile(argv[1]);
   if (root == NULL)
   {
      pfExit();
      exit(-1);
   }

   // Attach loaded file to a pfScene.
   gScene = new pfScene;
   gScene->addChild(root);
   root->setName("Root");
   //root->setTravFuncs(PFTRAV_APP,nodeTravAppPre,nodeTravAppPost);

   // determine extent of scene's geometry

   gScene->getBound(&bsphere);

   // Create a pfLightSource and attach it to scene.
   gScene->addChild(new pfLightSource);


   //openSomeWindows();
   
   int frame_num = 1;

   // Simulate for twenty seconds.
   while (t < 200.0f)
   {
      frame_num++;

      // Periodically try to open some windows
      if((frame_num % 60) == 0)
      {
         openSomeWindows();
      }

      float      s, c;
      pfCoord     view;

      // Go to sleep until next frame time.
      pfSync();

      //cout << "------------- << Synced >> --------------" << endl;

      // Compute new view position.
      t = pfGetTime();
      pfSinCos(45.0f*t, &s, &c);
      view.hpr.set(45.0f*t, -10.0f, 0);
      view.xyz.set(2.0f * bsphere.radius * s,
                   -2.0f * bsphere.radius *c,
                   0.5f * bsphere.radius);

      for (loop=0; loop < gNumWinActive; loop++)
         gChan[loop]->setView(view.xyz, view.hpr);

      // Initiate cull/draw for this frame.
      pfFrame();
   }

   // Terminate parallel processes and exit.
   pfExit();

   return 0;
}

// If there are more windows to open, then open some up
void openSomeWindows()
{
   cout << "Opening some more windows." << endl;
   cout << "  gNumWinActive:" << gNumWinActive << endl;

   if(gNumWinActive >= NUM_WINS)
      return;

   // Configure and open GL window
   char str[PF_MAXSTRING];
   int cur_win(0);

   for (int i=0; i < NUM_OPEN_AT_ONCE; i++)
   {
      cur_win = gNumWinActive+i;
      
      pfPipe* p = getPipeNum(cur_win%NUM_PIPES);    // Get a pipe in range [0..NUM_PIPES-1]

      gPwin[cur_win] = new pfPipeWindow(p);
      sprintf(str, "IRIS Performer - Win %d", cur_win);
      gPwin[cur_win]->setName(str);
      gPwin[cur_win]->setOriginSize((cur_win%4)*315, (cur_win/4)*340,
                                300, 300);
      gPwin[cur_win]->setConfigFunc(OpenPipeWin);
      gPwin[cur_win]->config();
/*
   }

   // Create and configure a pfChannel.
   for (i=0; i < NUM_OPEN_AT_ONCE; i++)
   {
      cur_win = gNumWinActive+i;
*/

      gChan[cur_win] = new pfChannel(p);
      gPwin[cur_win]->addChan(gChan[cur_win]);
      gChan[cur_win]->setScene(gScene);
      gChan[cur_win]->setNearFar(1.0f, 10.0f * bsphere.radius);
      gChan[cur_win]->setFOV(45.0f, 0.0f);
   }


   // Configure channel group
   pfChannel* master_chan = gChan[0];


   unsigned share = master_chan->getShare();
   master_chan->setShare((share | PFCHAN_NEARFAR | PFCHAN_EARTHSKY |
                   PFCHAN_STRESS | PFCHAN_LOD | PFCHAN_SWAPBUFFERS |
                   PFCHAN_APPFUNC | PFCHAN_CULLFUNC | PFCHAN_STATS_DRAWMODE)
                   & (~PFCHAN_SCENE)
                   //& (~PFCHAN_APPFUNC)
                   );

   for(i=gNumWinActive;i<gNumWinActive+NUM_OPEN_AT_ONCE;i++)
   {
     if(0 != i)
     {
        master_chan->attach(gChan[i]);
      }
   }

   // Inc num wins active
   gNumWinActive += NUM_OPEN_AT_ONCE;

   cout << "  opened: " << NUM_OPEN_AT_ONCE << endl;
   cout << "  gNumWinActive:" << gNumWinActive << endl;
}


// Get a pipe to use in the system
pfPipe* getPipeNum(const int pipe_num)
{
   if(pipe_num >= NUM_PIPES)
   {
      cout << "getPipeNum: PipeNum out of range\n";
      return NULL;
   }

   if(pipes[pipe_num] != NULL)
      return pipes[pipe_num];

   pfPipe* pipe;
   pipe = pfGetPipe(pipe_num);
   pipe->setScreen(pipe_num);
   pipe->setWSConnectionName(pipeStrs[pipe_num]);

   return pipe;
}

void initPipes()
{
   for(int i;i<NUM_PIPES;i++)
   {
      pipes[i] = getPipeNum(i);

      pfPipeWindow* pw = new pfPipeWindow(pipes[i]);
      pw->setOriginSize(0,0,1,1);
   }
}


//
// OpenPipeWin() -- create a GL window: set up the
//      window system, IRIS GL, and IRIS Performer. This
//      procedure is executed for each window in the draw process
// for that pfPipe.
//
static void
OpenPipeWin(pfPipeWindow *pw)
{
    pfPipe *p = pw->getPipe();

    // share GL objects with all the windows on the pipe
    pw->open();

    // create a light source in the "south-west" (QIII)
    pfLight *Sun = new pfLight;
    Sun->setPos(-0.3f, -0.3f, 1.0f, 0.0f);
}

void chanAppTravFunc(pfChannel *chan, void* chandata)
{
   cout << "App traversal chan: " << chan << endl;
   pfApp();
}

int nodeTravAppPre(pfTraverser* trav, void* userData)
{
   cout << "App node pre-trav: node:" << (void*)trav->getNode() << " named:" << trav->getNode()->getName() << endl << flush;
   return PFTRAV_CONT;
}

int nodeTravAppPost(pfTraverser* trav, void* userData)
{
   cout << "App node post-trav: node:" << (void*)trav->getNode() << " named:" << trav->getNode()->getName() << endl << flush;
   return PFTRAV_CONT;
}

