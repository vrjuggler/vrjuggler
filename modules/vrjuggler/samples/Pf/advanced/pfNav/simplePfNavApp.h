/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _SIMPLE_PF_NAV_APP_H_
#define _SIMPLE_PF_NAV_APP_H_

// animation
#include "KeyFramer.h" // for keyframer...
#include <PfAppStats.h> // for stats
#include "StopWatch.h"    // for time
#include <vrj/Draw/Pf/App.h>    // the performer application base type

#include <snx/SoundHandle.h>

class pfNavDCS;
class navigator;

extern int AppNotifyPreTrav( pfTraverser* trav, void* data );
extern int AppNotifyPostTrav( pfTraverser* trav, void* data );

//: Ready-Made Application
//
// The ready-made-application-in-a-box-handy-dandy-automatic-do-all
// vr juggler performer application for sounds, models, and scripted
// flythroughs.  Just tell it your models and sounds, and it does the rest.
//
// Better than snake oil, this application easy to use,
// but really hard to understand.
// - i.e. don't use this to learn vrjuggler or performer!
//        you've been warned...
class simplePfNavApp : public vrj::pf::App
{
public:
   // Model and sound member classes
   class Model
   {
   public:
      Model();
      Model( const std::string& desc, const std::string& file_name,
             const float& s, const gmtl::Vec3f& position,
             const gmtl::Vec3f& rotation, const bool& collidable );

      // Config parameters
      std::string description;
      std::string filename;
      float       scale;
      gmtl::Vec3f pos;
      gmtl::Vec3f rot;
      bool        isCollidable;

      // Run-time information
      // --- modelDCS -- modelNode --- (Model info) -- bok bok bok
      pfDCS*      modelDCS;
      pfNode*     modelNode;
   };

   class Sound
   {
    public:
      Sound();
      Sound( const std::string& sound_name, const std::string& alias_name,
            const bool& isPositional, const gmtl::Vec3f& position );

      std::string name;
      std::string alias;
      bool        positional;
      gmtl::Vec3f pos;
   };

// application callbacks:
public:
   //: default constructor
   simplePfNavApp();

   //: virtual destructor
   virtual ~simplePfNavApp();

   //: data init
   virtual void init();
   virtual void apiInit();
   virtual void preForkInit();

   //: Initialize the scene graph
   virtual void initScene();

   //: load the model into the scene graph
   // If another model is already in the scene graph, we destroy it and load the newly configured one.
   // The model loaded is based on the configuration information that we currently have
   // This may be called multiple times
   virtual void initializeModels();
   virtual void initializeSounds();

   //: Return the current scene graph
   virtual pfGroup* getScene();

   //: Function called in application process for each active channel each frame
   // Called immediately before draw (pfFrame())
   virtual void appChanFunc(pfChannel* chan);

   //: Function called by the DEFAULT drawChan function before clearing the channel
   // and drawing the next frame (pfFrame())
   virtual void preDrawChan(pfChannel* chan, void* chandata);

   //: Function called after pfSync and before pfDraw
   virtual void preFrame();

   //: Function called after pfDraw
   virtual void intraFrame();

// Methods to configure the application
public:
   //: Reset the application to initial state (removes all sounds and models)
   virtual void reset();

   //: set to true to enable navigation
   void enableNav( bool state = true );

   //: set the keyframed animation file to use
   void loadAnimation( const char* const filename );

   //: These must be set before the kernel starts calling the application
   void addModelFile( const std::string& filename );
   
   // Add a model to the application
   void addModel( const Model& m );

   // add a sound to the application
   void addSound( const Sound& s );

   void addFilePath( const std::string& path );
   void setFilePath( const std::string& path );
   void setInitialNavPos( const gmtl::Vec3f& initialPos );

   // Go to the next navigator
   void cycleNavigator();

   void setNavigator( unsigned new_index );

   void enableStats();
   void disableStats();

// data access:
public:
   // alias to the keyframer...
   kev::KeyFramer& keyFramer();
   const kev::KeyFramer& keyFramer() const;


// data: (should be private in the future.)
public:
   // models and sounds.
   std::vector< Model > mModelList;
   std::vector< Sound > mSoundList;

   // animation
   kev::KeyFramer      mKeyFramer;
   StopWatch           mStopWatch;
   bool                mDisableNav;
   pfDCS*              mAnimDCS;

   // CONFIG PARAMS
   std::string    mFilePath;
   gmtl::Vec3f    mInitialNavPos;
   float          mBoundingSize;       // XXX: This is a hack and should be refactored

   int            mStatusMessageEmitCount;

   bool            mUseStats;
   vrj::PfAppStats mStats;

   // navigation objects.
   std::vector<navigator*>    mNavigators;      // A list of the navigators in the system
   unsigned                   mCurNavIndex;     // Index of the current navigator
   pfNavDCS*                  mNavigationDCS;
   gadget::DigitalInterface      mNavCycleButton;  // Button to cycle the navigation

   // SCENE GRAPH NODES
   pfGroup*       mLightGroup;
   pfGroup*       mNoNav;
   pfLightSource* mSun;                      // Sun to light the environment
   pfGroup*       mRootNode;                 // The root of the scene graph
   pfGroup*       mConfiguredCollideModels;               // Collidable model of the world
   pfGroup*       mConfiguredNoCollideModels;                 // UnCollidable model of the world
   pfGroup*       mSoundNodes;               // The sounds of the world
   pfGroup*       mCollidableModelGroup;     // Part of the world that is collidable
   pfGroup*       mUnCollidableModelGroup;   // Part of the world that is collidable

   snx::SoundHandle mStopSound;
   snx::SoundHandle mAccelSound;
   snx::SoundHandle mLandSound;
   snx::SoundHandle mBumpSound, mAmbientSound, mWalkingSound;

   gadget::DigitalInterface      mGoButton;  
   gadget::DigitalInterface      mStopButton;
   
   float mWalkingCount, mRideCount;
   bool mColliding, mRiding;
   gmtl::Vec3f mVelocity;
   gmtl::Vec3f mPrevPosition;
};

#endif

