#ifndef _VRJ_SOUNDMANAGER_FACTORY_H_
#define _VRJ_SOUNDMANAGER_FACTORY_H_

#include <vrj/vjConfig.h>

#include <vrj/Sound/SoundManager.h>

namespace vrj
{

class VJ_CLASS_API SoundManagerFactory
{
public:
   // singleton function to get an instance of a soundmanager
   // returns whichever was configured at build time.
   static SoundManager& get();
};

};

#endif
