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
   initial_pos.set((float)chunk->getProperty("start_location",0),
                   (float)chunk->getProperty("start_location",1),
                   (float)chunk->getProperty("start_location",2));

   vjDEBUG(vjDBG_ALL,0) << "=========================================== " << app_name << " ============\n" << vjDEBUG_FLUSH;
   vjDEBUG(vjDBG_ALL,0) << "pfBasicConfigNav::configApp:  ========= configuration application =========\n" << vjDEBUG_FLUSH;
   
   // models
   mModelList.clear();//start out clean
   for (int x = 0; x < chunk->getNum( "Model" ); ++x)
   {
      vjConfigChunk* model_chunk = chunk->getProperty( "Model", x );
      model m;
      m.description = (std::string)model_chunk->getProperty( "Name" );
      m.filename = (std::string)model_chunk->getProperty( "filename" );
      cout<<"Reading "<<m.filename<<" model from the config file\n"<<flush;
      m.scale = (float)model_chunk->getProperty( "Scale" );
      m.pos.set( (float)model_chunk->getProperty( "x" ),
            (float)model_chunk->getProperty( "y" ),
            (float)model_chunk->getProperty( "z" ) );
      m.rot.set( (float)model_chunk->getProperty( "rotx" ),
            (float)model_chunk->getProperty( "roty" ),
            (float)model_chunk->getProperty( "rotz" ) );
      m.isCollidable = (bool)model_chunk->getProperty( "collidable" );
      mModelList.push_back( m );
   }
   
   // sounds
   mSoundList.clear();//start out clean
   for (x = 0; x < chunk->getNum( "Sound" ); ++x)
   {
      vjConfigChunk* sound_chunk = chunk->getProperty( "Sound", x );
      sound s;
      s.name = (std::string)sound_chunk->getProperty( "Name" );
      s.alias = (std::string)sound_chunk->getProperty( "soundAlias" );
      cout<<"Reading "<<s.alias<<" sound from the config file\n"<<flush;
      s.positional = (bool)sound_chunk->getProperty( "positional" );
      s.pos.set( (float)sound_chunk->getProperty( "x" ),
            (float)sound_chunk->getProperty( "y" ),
            (float)sound_chunk->getProperty( "z" ) );
      mSoundList.push_back( s );
   }
   
   
   
   setFilePath(file_path);
   setInitialNavPos(initial_pos);

   vjDEBUG_CONT(vjDBG_ALL,0) << "filepath: " << file_path << endl << vjDEBUG_FLUSH;
   vjDEBUG_CONT(vjDBG_ALL,0) << "initial pos: " << initial_pos << endl << vjDEBUG_FLUSH;

   // Initialize the models and sounds
   initializeModels();
   initializeSounds();
}


#endif
