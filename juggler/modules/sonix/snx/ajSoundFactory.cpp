#include <string>
#include "ajSoundImplementation.h"

// hard coded for now... TODO: registration system...
#include "ajOpenALSoundImplementation.h"
#include "ajAudioWorksSoundImplementation.h"
#include "ajStubSoundImplementation.h"

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
   // hard coded for now... TODO: registration system...
   if (apiName == "OpenAL")
   {
      mImplementation = new ajOpenALSoundImplementation;
   }
   else if (apiName == "Audioworks")
   {
      mImplementation = new ajAudioWorksSoundImplementation;
   }
   else
   {
      mImplementation = new ajStubSoundImplementation;
   }
}
