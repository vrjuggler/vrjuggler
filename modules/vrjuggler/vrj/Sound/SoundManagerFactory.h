#ifndef SOUNDMANAGER_FACTORY
#define SOUNDMANAGER_FACTORY

#include <vjConfig.h>

#include <Sound/SoundManager.h>

namespace vrj
{

class VJ_CLASS_API SoundManagerFactory
{
public:
   // singleton function to get an instance of a soundmanager
   // returns whichever was configured at build time.
   static vrj::SoundManager& get();
};

};

#endif
