#ifndef _VJ_SOUND_ENGINE_CONSTRUCTOR_H_
#define _VJ_SOUND_ENGINE_CONSTRUCTOR_H_

#include <Sound/vjSoundEngine.h>
#include <string>
#include <Config/vjConfigChunk.h>
#include <Kernel/vjDebug.h>

//: Base class for virtual construction of sound engines
// Implementations of this class are registered with the device factory
// for each device in the system
class vjSoundEngineConstructorBase
{
public:
   //: Constructor
   //! POST: Device is registered
   vjSoundEngineConstructorBase() {;}

   //: Create the device
   virtual vjSoundEngine* createEngine(vjConfigChunk* chunk)
   {
      vjDEBUG(vjDBG_ALL,0) << "ERROR: vjSoundEngineConstructorBase::createEngine: Should never be called" << vjDEBUG_FLUSH;
      return NULL;
   }

   //: Get the string desc of the type of chunk we can create
   virtual std::string    getChunkType()
   { return std::string("BaseConstructor: Invalid type"); }
};


template <class ENGINE>
class vjSoundEngineConstructor : public vjSoundEngineConstructorBase
{
public:
   vjSoundEngineConstructor();

   virtual vjSoundEngine* createEngine(vjConfigChunk* chunk)
   {
      ENGINE* new_engine = new ENGINE;
      bool success = new_engine->config(chunk);
      if(success)
      {
         return new_engine;
      } else {
         delete new_engine;
         return NULL;
      }
   }

   virtual std::string getChunkType()
   { return ENGINE::getChunkType(); }
};


template <class ENGINE>
vjSoundEngineConstructor<ENGINE>::vjSoundEngineConstructor()
{
   //vjASSERT(vjSoundFactory::instance() != NULL);
   vjSoundFactory::registerCreator()->registerDevice(this);
}

#endif
