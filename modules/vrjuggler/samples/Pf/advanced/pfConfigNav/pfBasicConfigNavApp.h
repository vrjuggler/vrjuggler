/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _PF_BASIC_CONFIG_NAV_
#define _PF_BASIC_CONFIG_NAV_

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
#include <vrj/Draw/Pf/PfApp.h>    // the performer application base type
#include <vrj/Util/Debug.h>
#include <vrj/Display/Projection.h>  // for setNearFar (for setting clipping planes)
#include <pfFileIO.h>

// nav includes
#include <pfNavDCS.h>
#include <velocityNav.h>
#include <collider.h>
#include <planeCollider.h>
#include <pfPogoCollider.h>
#include <pfRayCollider.h>
#include <pfBoxCollider.h>


#include <simplePfNavApp.h>

// ----- pfBasicConfigNav class ---- //
// Declare my application class
class pfBasicConfigNavApp : public simplePfNavApp
{
public:
   pfBasicConfigNavApp( ) : mCurrentConfig()
   {
   }

public:     // RECONFIG STUFF
   virtual bool configCanHandle(jccl::ConfigChunkPtr chunk)
   {
      return configCanHandleChunk(chunk);
   }

   // This function is so that others can query this object to
   // see if it can be configured with the given information
   static bool configCanHandleChunk(jccl::ConfigChunkPtr chunk)
   {
      std::string chunk_type = (std::string)chunk->getType();

      if(std::string("pf_basic_nav_app") == chunk_type)
         return true;
      else
         return false;
   }


   //: Are any application dependencies satisfied
   // If the application requires anything special of the system for successful
   // initialization, check it here.
   // If retval == false, then the application will not be started yet
   //    retval == true, application will be allowed to enter the system
   virtual bool depSatisfied()
   { return true; }

protected:
   //! NOTE: Inherited from jccl::ConfigChunkHandler
   virtual bool configAdd(jccl::ConfigChunkPtr chunk);

   //! NOTE: INherited from jccl::ConfigChunkHandler
   virtual bool configRemove(jccl::ConfigChunkPtr chunk)
   { vprASSERT(false); return false; }

protected:
   jccl::ConfigChunkPtr mCurrentConfig;      // config chunk that is current configuring this thingie
};

// Get the properties from the chunks
// XXX: Smart update
// Set the properties
// Load with new settings
bool pfBasicConfigNavApp::configAdd( jccl::ConfigChunkPtr chunk )
{
   int x;

   vprASSERT(std::string("pf_basic_nav_app") == (std::string)chunk->getType());

   // Set the properties based on the chunk values
   std::string app_name = chunk->getProperty("Name");
   std::string file_path = chunk->getProperty("file_path");
   vrj::Vec3 initial_pos;
   initial_pos.set((float)chunk->getProperty("start_location",0),
                   (float)chunk->getProperty("start_location",1),
                   (float)chunk->getProperty("start_location",2));

   ///////////////////////////
   if (chunk->doesPropertyExistFromToken( "enable_nav" ))
   {
      this->enableNav( (bool)chunk->getProperty( "enable_nav" ) );
      vprDEBUG_BEGIN(vprDBG_ALL,0) << "enable_nav: " << (bool)chunk->getProperty( "enable_nav" ) << "\n===========================\n" << vprDEBUG_FLUSH;
   }   
   else
   {
      this->enableNav( true );
   }
   
   if (chunk->doesPropertyExistFromToken( "animation_filename" ))
   {
      std::string a = (std::string)chunk->getProperty( "animation_filename" );
      this->loadAnimation( a.c_str() );
   }
   
   if (chunk->doesPropertyExistFromToken( "animation_play" ))
   {
      if ((bool)chunk->getProperty( "animation_play" ) == true)
      {
         this->keyFramer().play();
      }
      else
      {
         this->keyFramer().stop();
         this->keyFramer().rewind();
      }   
      this->keyFramer().loop( (int)chunk->getProperty( "animation_loops" ) );
   }
   ////////////////////////////
   
   vprDEBUG_BEGIN(vprDBG_ALL,0) << "pfBasicConfigNav::configApp: " << app_name << "===========================\n" << vprDEBUG_FLUSH;

   // models
   mModelList.clear();//start out clean
   for (x = 0; x < chunk->getNum( "Model" ); ++x)
   {
      jccl::ConfigChunkPtr model_chunk = chunk->getProperty( "Model", x );
      Model m;
      m.description = (std::string)model_chunk->getProperty( "Name" );
      m.filename = (std::string)model_chunk->getProperty( "filename" );
      vprDEBUG_NEXT(vprDBG_ALL,0) << "Reading " <<m.filename<<" model from the config file\n"<< vprDEBUG_FLUSH;;
      m.scale = (float)model_chunk->getProperty( "Scale" );
      m.pos.set( (float)model_chunk->getProperty( "x" ),
            (float)model_chunk->getProperty( "y" ),
            (float)model_chunk->getProperty( "z" ) );
      m.rot.set( (float)model_chunk->getProperty( "rotx" ),
            (float)model_chunk->getProperty( "roty" ),
            (float)model_chunk->getProperty( "rotz" ) );
      m.isCollidable = (bool)model_chunk->getProperty( "collidable" );
      addModel( m );
   }

   // sounds
   mSoundList.clear();//start out clean
   for (x = 0; x < chunk->getNum( "Sound" ); ++x)
   {
      jccl::ConfigChunkPtr sound_chunk = chunk->getProperty( "Sound", x );
      Sound s;
      s.name = (std::string)sound_chunk->getProperty( "Name" );
      s.alias = (std::string)sound_chunk->getProperty( "soundAlias" );
      vprDEBUG_NEXT(vprDBG_ALL,0) <<"Reading "<<s.alias<<" sound from the config file\n"<< vprDEBUG_FLUSH;
      s.positional = (bool)sound_chunk->getProperty( "positional" );
      s.pos.set( (float)sound_chunk->getProperty( "x" ),
            (float)sound_chunk->getProperty( "y" ),
            (float)sound_chunk->getProperty( "z" ) );
      mSoundList.push_back( s );
   }

   setFilePath( file_path );
   setInitialNavPos( initial_pos );
   mBoundingSize = chunk->getProperty("bounding_size");

   vprDEBUG_NEXT(vprDBG_ALL,0) << "   filepath: " << file_path << std::endl
                             << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vprDBG_ALL,0) << "initial pos: " << initial_pos << std::endl
                             << vprDEBUG_FLUSH;
   vprDEBUG_NEXT(vprDBG_ALL,0) << " bound size: " << mBoundingSize << std::endl
                             << vprDEBUG_FLUSH;
   vprDEBUG_END(vprDBG_ALL,0) << "========================================\n" << vprDEBUG_FLUSH;

   
   
   // Initialize the models and sounds
   initializeModels();                 // Only actally gets processed if initScene has already run
   initializeSounds();                 // Only actally gets processed if initScene has already run

   return true;
}


#endif
