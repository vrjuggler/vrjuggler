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
// multichanC.C: Performer program to demonstrate multiple channels
//               in one pipe.  Derived from simple.c
//
// $Revision$ 
// $Date$ 
//
//

#include <stdlib.h>

#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pr/pfLight.h>

#include <Performer/pfdu.h>


static void OpenPipeWin(pfPipeWindow *pw);
static void DrawChannel(pfChannel *chan, void *data);


//
//	Usage() -- print usage advice and exit. This
//      procedure is executed in the application process.
//
static void
Usage (void)
{
    pfNotify(PFNFY_FATAL, PFNFY_USAGE, "Usage: multichanC file.ext ...\n");
    exit(1);
}

int
main (int argc, char *argv[])
{
    float       t = 0.0f;
    
    if (argc < 2)
	Usage();
    
    // Initialize Performer
    pfInit();	
    
    // Use default multiprocessing mode based on number of
    // processors.
    //
    pfMultiprocess( PFMP_APP_CULL_DRAW );
    
    // Load all loader DSO's before pfConfig() forks 
    pfdInitConverter(argv[1]);

    // Configure multiprocessing mode and start parallel
    // processes.
    //
    pfConfig();			
    
    // Attach loaded file to a pfScene.
    pfScene *scene = new pfScene;
    
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
    
    // determine extent of scene's geometry
    pfSphere	bsphere;
    root->getBound(&bsphere);
    scene->addChild(root);
    
    // Create a pfLightSource and attach it to scene.
    scene->addChild(new pfLightSource);
    
    // Configure and open GL window
    pfPipe *p = pfGetPipe(0);
    pfPipeWindow *pw = new pfPipeWindow(p);
    pw->setName("IRIS Performer");
    pw->setOriginSize(0, 0, 600, 600);
    pw->setConfigFunc(OpenPipeWin);
    pw->config();
    
    // Create and configure a pfChannel.
    pfChannel   *chan[4];
    
    for (int loop=0; loop < 4; loop++)
    {
	chan[loop] = new pfChannel(p);
	chan[loop]->setTravFunc(PFTRAV_DRAW,DrawChannel);
	chan[loop]->setScene(scene);
	chan[loop]->setNearFar(1.0f, 10.0f * bsphere.radius);
	chan[loop]->setFOV(45.0f, 0.0f);
	pfFrameStats *fstats = chan[loop]->getFStats();
	fstats->setClass(PFSTATS_ENGFX, PFSTATS_ON);
    }
    
    chan[0]->setViewport(0.0f, 0.5f, 0.0f, 0.5f);
    chan[1]->setViewport(0.5f, 1.0f, 0.0f, 0.5f);
    chan[2]->setViewport(0.5f, 1.0f, 0.5f, 1.0f);
    chan[3]->setViewport(0.0f, 0.5f, 0.5f, 1.0f);
    
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
	
	for (loop=0; loop < 4; loop++)
	{
	    chan[loop]->setView(view.xyz, view.hpr);
	    // chan[loop]->drawStats();
	}
	// Initiate cull/draw for this frame.
	pfFrame();		
    }
    
    // Terminate parallel processes and exit.
    pfExit();
    
    return 0;
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
    pw->open();
    
    // create a light source in the "south-west" (QIII)
    pfLight *Sun = new pfLight;
    Sun->setPos(-0.3f, -0.3f, 1.0f, 0.0f);
}

static void
DrawChannel (pfChannel *, void *)
{
    
    static pfVec4 clr[] = { pfVec4(1.0f, 0.0f, 0.0f, 1.0f),
			    pfVec4(0.0f, 0.0f, 1.0f, 1.0f) };
    static int i=0;
    
    // erase framebuffer and draw Earth-Sky model
    pfClear(PFCL_COLOR | PFCL_DEPTH, &clr[i]);
    i ^= 1;
    
    // invoke Performer draw-processing for this frame
    pfDraw();
}
