#ifndef _PF_BASIC_CONFIG_NAV_
#define _PF_BASIC_CONFIG_NAV_

#include <iostream.h>
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
#include <Kernel/vjKernel.h>
#include <Kernel/Pf/vjPfApp.h>    // the performer application base type
#include <Kernel/vjDebug.h>
#include <Kernel/vjProjection.h>  // for setNearFar (for setting clipping planes)
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
   pfBasicConfigNavApp( )
   {
     mCurrentConfig = NULL;
   }

   void configApp(vjConfigChunk* chunk);

public:     // RECONFIG STUFF
   virtual bool configCanHandle(vjConfigChunk* chunk)
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
   //! NOTE: Inherited from vjConfigChunkHandler
   virtual bool configAdd(vjConfigChunk* chunk)
   {
      configApp(chunk);
      return true;
   }

   //! NOTE: INherited from vjConfigChunkHandler
   virtual bool configRemove(vjConfigChunk* chunk)
   { vjASSERT(false); return false; }

protected:
   vjConfigChunk* mCurrentConfig;      // config chunk that is current configuring this thingie
};

// Get the properties from the chunks
// XXX: Smart update
// Set the properties
// Load with new settings
void pfBasicConfigNavApp::configApp(vjConfigChunk* chunk)
{
   vjASSERT(std::string("pf_basic_nav_app") == (std::string)chunk->getType());

   // Set the properties based on the chunk values
   std::string app_name = chunk->getProperty("Name");
   std::string file_path = chunk->getProperty("file_path");
   vjVec3 initial_pos;
   initial_pos.set(chunk->getProperty("start_location",0),
                   chunk->getProperty("start_location",1),
                   chunk->getProperty("start_location",2));

   // sounds
   for (int x = 0; x < chunk->getNum( "Sounds" ); ++x)
   {
      vjConfigChunk* sound_chunk = chunk->getProperty( "Sound", x );
      sound s;
      s.name = sound_chunk->getProperty( "Name" );
      s.alias = sound_chunk->getProperty( "soundAlias" );
      s.positional = sound_chunk->getProperty( "positional" );
      s.pos.set( sound_chunk->getProperty( "x" ),
            sound_chunk->getProperty( "y" ),
            sound_chunk->getProperty( "z" ) );
      mSoundList.push_back( s );
   }
   
   // models
   for (int x = 0; x < chunk->getNum( "Models" ); ++x)
   {
      vjConfigChunk* model_chunk = chunk->getProperty( "Model", x );
      model m;
      m.description = model_chunk->getProperty( "Name" );
      m.filename = model_chunk->getProperty( "filename" );
      m.scale = model_chunk->getProperty( "Scale" );
      m.pos.set( model_chunk->getProperty( "x" ),
            model_chunk->getProperty( "y" ),
            model_chunk->getProperty( "z" ) );
      m.rot.set( model_chunk->getProperty( "rotx" ),
            model_chunk->getProperty( "roty" ),
            model_chunk->getProperty( "rotz" ) );
      mModelList.push_back( s );
   }
   
   setFilePath(file_path);
   setInitialNavPos(initial_pos);

   vjDEBUG(vjDBG_ALL,0) << "\_\_\_\_\_\_\_\_\_\_\_ " << app_name << " \_\_\_\_\_\_\_\_\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_ALL,0) << "pfBasicConfigNav::configApp:  ========= configuration application =========\n" << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_ALL,0) << "filename: " << file_name << endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_ALL,0) << "filepath: " << file_path << endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_ALL,0) << "world scale: " << world_scale << endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_ALL,0) << "initial pos: " << initial_pos << endl << vjDEBUG_FLUSH;

   // Initialize the models and sounds
   initializeModels();
   initializeSounds();
}


#endif
