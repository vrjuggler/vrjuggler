#include <string>
#include "aj/SoundImplementation.h"

#include "aj/StubSoundImplementation.h" // in case lookup fails...

#include "aj/SoundFactory.h" // my header.

namespace aj
{

/**
 * @input name of api to create
 * @output an implementation is returned for you to use
 * @postconditions if apiName is not known, then a stub implementation is returned
 * @semantics factory function used to create an implementation of a sound API 
 */
void SoundFactory::createImplementation( const std::string& apiName,
                           aj::SoundImplementation* &mImplementation )
{
   if (mRegisteredImplementations.count( apiName ) > 0)
   {
      mRegisteredImplementations[apiName]->clone( mImplementation );
   }

   else
   {
      mImplementation = new aj::StubSoundImplementation;
   }
}

}; // end namespace
