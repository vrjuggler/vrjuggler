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
// multipipeC.C: simple Performer program to demonstrate use of 
//               multiple pfPipe's.  based on simple.c
//
// $Revision$
// $Date$ 
//
//

#include <stdlib.h>

#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pr/pfLight.h>
#include <iostream.h>

#include <Performer/pfdu.h>

static int NumScreens=1;
static int NumPipes=5;
static int ProcSplit = PFMP_DEFAULT;
static int GangSwap = 0;
static int PipeStats = 0;
char ProgName[PF_MAXSTRING];

static void ConfigPipeDraw(int pipe, uint stage);
static void OpenPipeWin(pfPipeWindow *pw);


//
//	Usage() -- print usage advice and exit. This
//      procedure is executed in the application process.
//
static void
Usage (void)
{
    pfNotify(PFNFY_FATAL, PFNFY_USAGE, "Usage: multipipeC file.ext ...\n");
    exit(1);
}

//
//	docmdline() -- use getopt to get command-line arguments, 
//	executed at the start of the application process.
//

static int
docmdline(int argc, char *argv[])
{
    int	    opt;

    strcpy(ProgName, argv[0]);
    
    /* process command-line arguments */
    while ((opt = getopt(argc, argv, "gs:mp:?")) != -1)
    {
	switch (opt)
	{
	case 'g': 
	    GangSwap = 1;
	    break;
	case 's': 
	    PipeStats = 100;
	    break;
	case 'S': 
	    PipeStats = atoi(optarg);
	    break;
	case 'm':
	case 'p':
	    ProcSplit = atoi(optarg);
	    break;
	}
    }
    return optind;
}

int
main (int argc, char *argv[])
{
    float       t = 0.0f;
    int		arg, found;
    
    if (argc < 2)
	Usage();
    
    arg = docmdline(argc, argv);
    
    // Initialize Performer
    pfInit();	
    
    // specify the number of pfPipes
    pfMultipipe (NumPipes);
    
    // Use default multiprocessing mode based on number of
    // processors.
    //
    pfMultiprocess(ProcSplit);
    
    // Load all loader DSO's before pfConfig() forks 
    for (found = arg; found < argc; found++)
	pfdInitConverter(argv[found]);

    // Configure multiprocessing mode and start parallel
    // processes.
    //
    pfConfig();			

    // Append to PFPATH additional standard directories where 
    // geometry and textures exist 
    //
    if (!(getenv("PFPATH")))
        pfFilePath(
                   "."
                   ":./data"
                   ":../data"
                   ":../../data"
                   ":/usr/share/Performer/data"
                   );
    
    // Read a single file, of any known type.
    pfNode *root = pfdLoadFile(argv[1]);
    if (root == NULL) 
    {
	pfExit();
	exit(-1);
    }
    
    // Attach loaded file to a pfScene.
    pfScene *scene = new pfScene;
    for (found = 0; arg < argc; arg++)
    {
	if ((root = pfdLoadFile(argv[arg])) != NULL)
	{
	    scene->addChild(root);
	    found++;
	}
    }
    
    // determine extent of scene's geometry
    pfSphere bsphere;
    scene->getBound(&bsphere);
    
    // Create a pfLightSource and attach it to scene.
    scene->addChild(new pfLightSource);
    
    // Configure and open GL windows
    int max;
    
    if ((NumScreens = ScreenCount(pfGetCurWSConnection())) == 1)
	max = NumPipes;
    else
	max = NumScreens;


    char* pipeStrs[6] = { ":0.0", ":1.0", ":2.0", ":3.0", ":4.0", ":5.0" };

    cout << "----- Num pipes: " << max << " --------" << endl;    
    
    int loop;
    pfPipe      *pipe[4];
    pfChannel   *chan[4];
    
    for (loop=0; loop < max; loop++)
    {
	char str[PF_MAXSTRING];
	
	pipe[loop] = pfGetPipe(loop);

	pipe[loop]->setScreen(loop);
        pipe[loop]->setWSConnectionName(pipeStrs[loop]);
	
	pfPipeWindow *pw = new pfPipeWindow(pipe[loop]);
	sprintf(str, "IRIS Performer - Pipe %d", loop);
	pw->setName(str);
	if (NumScreens > 1)
	{
	    pw->setOriginSize(0, 0, 300, 300);
	} else
	    pw->setOriginSize((loop&0x1)*315, ((loop&0x2)>>1)*340, 300, 300);
	
	pw->setConfigFunc(OpenPipeWin);
	pw->config();
    }
    
    // Create and configure pfChannels - by default, channels are
    // placed in the first window on their pipe
    //
    for (loop=0; loop < max; loop++)
    {
	chan[loop] = new pfChannel(pipe[loop]);
	if (!loop)
	{
	    chan[loop]->setScene(scene);
	    chan[loop]->setNearFar(1.0f, 10.0f * bsphere.radius);
	    chan[loop]->setFOV(45.0f, 0.0f);
	    chan[loop]->setShare(PFCHAN_FOV | PFCHAN_SCENE 
		| (GangSwap * PFCHAN_SWAPBUFFERS_HW));
	}
	else
	    chan[0]->attach(chan[loop]);
    }
    
    // set up optional DRAW pipe stage config callback
    pfStageConfigFunc(-1 /* selects all pipes */, 
			PFPROC_DRAW /* stage bitmask */, 
			ConfigPipeDraw);
    pfConfigStage(-1, PFPROC_DRAW);

    // Simulate for twenty seconds.
    while (t < 20.0f)
    {
	float      s, c;
	pfCoord	   view;
	
	// Go to sleep until next frame time.
	pfSync();		
	
	// Compute new view position.
	t = pfGetTime();
	pfSinCos(45.0f*t, &s, &c);
	view.hpr.set(45.0f*t, -10.0f, 0);
	view.xyz.set(2.0f * bsphere.radius * s, 
		     -2.0f * bsphere.radius *c, 
		     0.5f * bsphere.radius);
	
	for (loop=0; loop < max; loop++)
	    chan[loop]->setView(view.xyz, view.hpr);
	
	// Initiate cull/draw for this frame.
	pfFrame();		

	if (PipeStats)
	    pfuManageMPipeStats(PipeStats, NumPipes);
    }
    
    // Terminate parallel processes and exit.
    pfExit();
    
    return 0;
}

// ConfigPipeDraw() --
// Application state for the draw process can be initialized
// here. This is also a good place to do real-time
// configuration for the drawing process, if there is one.
// There is no graphics state or pfState at this point so no
// rendering calls or pfApply*() calls can be made.
//
static void
ConfigPipeDraw(int pipe, uint stage)
{
    
    pfPipe *p = pfGetPipe(pipe);
    int x, y;

    pfNotify(PFNFY_NOTICE, PFNFY_PRINT, 
	"Initializing stage 0x%x of pipe %d", stage, pipe);
    p->getSize(&x, &y);
    pfNotify(PFNFY_NOTICE, PFNFY_PRINT, "Pipe %d size: %dx%d", pipe, x,y);
}

//
//	OpenPipeWin() -- create a GL window: set up the
//      window system, IRIS GL, and IRIS Performer. This
//      procedure is executed for each window in the draw process 
//	for that pfPipe.
//

static void
OpenPipeWin(pfPipeWindow *pw)
{
    // open the window on the specified screen. By default,
    // if a screen has not yet been set and we are in multipipe mode,
    // a window of pfPipeID i will now open on screen i
    //
    pw->open();
    
    // create a light source in the "south-west" (QIII)
    pfLight *Sun = new pfLight;
    Sun->setPos(-0.3f, -0.3f, 1.0f, 0.0f);
}
