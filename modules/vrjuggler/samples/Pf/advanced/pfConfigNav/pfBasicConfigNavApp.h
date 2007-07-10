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
#include <jccl/Config/ConfigElement.h>
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
   pfBasicConfigNavApp( )// : mCurrentConfig( new jccl::ConfigElement )
   {
   }

public:     // RECONFIG STUFF
   virtual bool configCanHandle(jccl::ConfigElementPtr element)
   {
      return configCanHandleElement(element);
   }

   // This function is so that others can query this object to
   // see if it can be configured with the given information
   static bool configCanHandleElement(jccl::ConfigElementPtr element)
   {
      const std::string element_type(element->getID());

      if(std::string("pf_basic_nav_app") == element_type)
      {
         return true;
      }
      else
      {
         return false;
      }
   }


   //: Are any application dependencies satisfied
   // If the application requires anything special of the system for successful
   // initialization, check it here.
   // If retval == false, then the application will not be started yet
   //    retval == true, application will be allowed to enter the system
   virtual bool depSatisfied()
   { return true; }

protected:
   //! NOTE: Inherited from jccl::ConfigElementHandler
   virtual bool configAdd(jccl::ConfigElementPtr element);

   //! NOTE: INherited from jccl::ConfigElementHandler
   virtual bool configRemove(jccl::ConfigElementPtr element)
   { vprASSERT(false); return false; }

protected:
   //jccl::ConfigElementPtr mCurrentConfig;      // config element that is current configuring this thingie
};

// Get the properties from the elements
// XXX: Smart update
// Set the properties
// Load with new settings
bool pfBasicConfigNavApp::configAdd( jccl::ConfigElementPtr element )
{
   vprASSERT(std::string("pf_basic_nav_app") == element->getID());

   // Set the properties based on the element values
   std::string app_name = element->getName();
   std::string file_path = element->getProperty<std::string>("file_path");
   gmtl::Vec3f initial_pos;
   initial_pos.set(element->getProperty<float>("start_location",0),
                   element->getProperty<float>("start_location",1),
                   element->getProperty<float>("start_location",2));

   ///////////////////////////
   if (element->getNum( "enable_nav" ) > 0)
   {
      this->enableNav( element->getProperty<bool>( "enable_nav" ) );
      vprDEBUG_BEGIN(vprDBG_ALL,0)
         << "enable_nav: " << element->getProperty<bool>( "enable_nav" )
         << "\n===========================\n" << vprDEBUG_FLUSH;
   }
   else
   {
      this->enableNav( true );
   }

   if (element->getNum( "animation_filename" ) > 0)
   {
      std::string a = element->getProperty<std::string>( "animation_filename" );
      this->loadAnimation( a.c_str() );
   }

   if (element->getNum( "animation_play" ) > 0)
   {
      if (element->getProperty<bool>( "animation_play" ) == true)
      {
         this->keyFramer().play();
      }
      else
      {
         this->keyFramer().stop();
         this->keyFramer().rewind();
      }
      this->keyFramer().loop( element->getProperty<int>( "animation_loops" ) );
   }
   ////////////////////////////

   vprDEBUG_BEGIN(vprDBG_ALL,0) << "pfBasicConfigNav::configApp: " << app_name << "===========================\n" << vprDEBUG_FLUSH;

   // models
   mModelList.clear();//start out clean
   for ( unsigned int x = 0; x < element->getNum("models"); ++x )
   {
      jccl::ConfigElementPtr model_element =
         element->getProperty<jccl::ConfigElementPtr>( "models", x );

      Model m;
      m.description = model_element->getName();
      m.filename = model_element->getProperty<std::string>( "filename" );
      vprDEBUG_NEXT(vprDBG_ALL,0) << "Reading " <<m.filename<<" model from the config file\n"<< vprDEBUG_FLUSH;;
      m.scale = model_element->getProperty<float>("scale");
      m.pos.set( model_element->getProperty<float>("start_pos", 0),
                 model_element->getProperty<float>("start_pos", 1),
                 model_element->getProperty<float>("start_pos", 2) );
      m.rot.set( model_element->getProperty<float>("start_rot", 0),
                 model_element->getProperty<float>("start_rot", 1),
                 model_element->getProperty<float>("start_rot", 2) );
      m.isCollidable = model_element->getProperty<bool>( "collidable" );
      addModel( m );
   }

   // sounds
   mSoundList.clear();//start out clean
   for ( unsigned int x = 0; x < element->getNum("sounds"); ++x )
   {
      jccl::ConfigElementPtr sound_element =
         element->getProperty<jccl::ConfigElementPtr>( "sounds", x );

      Sound s;
      s.name = sound_element->getName();
      s.alias = sound_element->getProperty<std::string>( "sound_alias" );
      vprDEBUG_NEXT(vprDBG_ALL,0) <<"Reading "<<s.alias<<" sound from the config file\n"<< vprDEBUG_FLUSH;
      s.positional = sound_element->getProperty<bool>( "positional" );
      s.pos.set( sound_element->getProperty<float>("start_pos", 0),
                 sound_element->getProperty<float>("start_pos", 1),
                 sound_element->getProperty<float>("start_pos", 2) );
      mSoundList.push_back( s );
   }

   setFilePath( file_path );
   setInitialNavPos( initial_pos );
   mBoundingSize = element->getProperty<float>("bounding_size");

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
