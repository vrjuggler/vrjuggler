#ifdef USE_AUDIOJUGGLER
#include <Sound/ajSoundManager.h>
#else
#include <Sound/SoundManager.h>
#endif

#include <Sound/SoundManagerFactory.h>

namespace vrj
{

SoundManager&    SoundManagerFactory::get()
{
   #ifdef USE_AUDIOJUGGLER
    static vrj::ajSoundManager temp;
    return temp;
   #else
    static vrj::SoundManager temp;
    return temp;
   #endif
}

};
