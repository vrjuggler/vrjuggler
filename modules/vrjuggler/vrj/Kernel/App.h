/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef _VJ_APP_
#define _VJ_APP_

#include <vjConfig.h>
//#include <Kernel/vjAPI.h>
#include <Kernel/vjKernel.h>
#include <Kernel/vjDebug.h>
#include <Kernel/vjConfigChunkHandler.h>
class vjDrawManager;
class vjKernel;

//----------------------------------------------------------------
//: Encapsulates the actually application.
//
//	    This class defines the class that all API specific
//  application classes should be derived from.  The interface
//  given is the interface that the Kernel expects in order to
//  interface with the application.  Most of the application's
//  interface will be defined in the derived API specific classes.
//
//  Users should sub-class the API specific classes to create
//  user-defined applications.  A user application is required
//  to provide function definitions for any of the virual functions
//  that the application needs to use.  This is the method that
//  the application programmer uses to interface with VR Juggler.
//
//  The control loop will look similar to this: <br> <br>
//
//  while (drawing)          <br>
//  {                        <br>
//       <b>preDraw()</b>;   <br>
//	      draw();             <br>
//	      <b>postDraw()</b>;  <br>
//       sync();             <br>
//       <b>postSync()</b>;  <br>
//                           <br>
//	      UpdateTrackers();   <br>
//  }                        <br>
//
// @author Allen Bierbaum
//  Date: 9-8-97
//!PUBLIC_API:
//------------------------------------------------------------------
class vjApp : public vjConfigChunkHandler
{
public:
   //: Constructor
   //! ARGS: kern - The vjKernel that is active.  So application has easy access to kernel
   vjApp(vjKernel* kern)
   {
      vjASSERT(kern != NULL);    // We don't want a NULL Kernel
      kernel = kern;
   }

public:
   //: Application init function
   // Execute any initialization needed before the API is started
   virtual void init()
   {;}

   //: Application API init function
   // Execute any initialization needed <b>after</b> API is started
   //  but before the drawManager starts the drawing loops.
   virtual void apiInit()
   {;}

   //: Execute any final cleanup needed for the application
   virtual void exit()
   {;}

   //: Function called after tracker update but before start of drawing
   virtual void preDraw()
   {;}
   //: Function called after drawing has been triggered but BEFORE it completes
   virtual void postDraw()
   {;}
   //: Function called before updating trackers but after the frame is drawn
   virtual void postSync()
   {;}

public:
   //vjAPI       api;        // Used to signal which API this application works with
   vjKernel*   kernel;     // The library kernel (here for convienence)

public:  // --- Factory functions --- //
   //: Get the DrawManager to use
   //! NOTE: Each derived app MUST implement this function
   virtual vjDrawManager*    getDrawManager() = 0;
};

#endif
