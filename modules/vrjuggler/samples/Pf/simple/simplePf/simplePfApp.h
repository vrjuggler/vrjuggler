/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _SIMPLE_PF_NAV_APP_H_
#define _SIMPLE_PF_NAV_APP_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfNode.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pfdu.h>
#include <Performer/pfutil.h>

// --- VR Juggler Stuff --- //
#include <vrj/Kernel/Kernel.h>
#include <vrj/Draw/Pf/PfApp.h>    /* the performer application base type */
#include <vrj/Util/Debug.h>
#include <vrj/Display/Projection.h>  /* for setNearFar (for setting clipping planes) */

using namespace vrj;

// Declare my application class
class simplePfApp : public PfApp
{
public:
   simplePfApp()
      : mModelFileName("")
      , mLightGroup(NULL)
      , mSun(NULL)
      , mRootNode(NULL)
      , mModelRoot(NULL)
   {
   }

   virtual ~simplePfApp();

   /**
    * Called between pfInit and pfConfig.  This function allows the user
    * application to do any processing that needs to happen before Performer
    * forks its processes off but after pfInit().
    */
   virtual void preForkInit()
   {
      // Initialize model converters
      pfdInitConverter( mModelFileName.c_str() );
   }

   /**
    * Initializes the scene graph.
    * Called after pfInit & pfConfig but before apiInit.
    */
   virtual void initScene();

   /// Return the current scene graph
   virtual pfGroup* getScene()
   {
      return mRootNode;
   }

   /**
    * Sets the name of the model to load.  This must be called before the
    * application is given to the kernel.
    */
   void setModel(std::string modelFile)
   {
      mModelFileName = modelFile;
   }

public:
   // CONFIG PARAMS
   std::string    mModelFileName;

   // SCENE GRAPH NODES
   pfGroup*       mLightGroup;
   pfLightSource* mSun;                 /**< Sun to light the environment */
   pfGroup*       mRootNode;            /**< The root of the scene graph */
   pfDCS*         mSceneScale;          /**< Scale for the root node */
   pfNode*        mModelRoot;           /**< Root of the model */
};

#endif
