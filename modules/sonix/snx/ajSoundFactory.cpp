#include <string>
#include "ajSoundImplementation.h"

#include "ajStubSoundImplementation.h" // in case lookup fails...

#include "ajSoundFactory.h" // my header.

/**
 * @input name of api to create
 * @output an implementation is returned for you to use
 * @postconditions if apiName is not known, then a stub implementation is returned
 * @semantics factory function used to create an implementation of a sound API 
 */
void ajSoundFactory::createImplementation( const std::string& apiName,
                           ajSoundImplementation* &mImplementation )
{
   if (mRegisteredImplementations.count( apiName ) > 0)
   {
      mRegisteredImplementations[apiName]->clone( mImplementation );
   }

   else
   {
      mImplementation = new ajStubSoundImplementation;
   }
}
