#include <vrj/vjConfig.h>

#ifdef USE_AUDIOJUGGLER
#include <vrj/Sound/ajSoundManager.h>
#else
#include <vrj/Sound/SoundManager.h>
#endif

#include <vrj/Sound/SoundManagerFactory.h>

namespace vrj
{

SoundManager&    SoundManagerFactory::get()
{
#ifdef USE_AUDIOJUGGLER
    static vrj::ajSoundManager temp;
    return temp;
#else
    static SoundManager temp;
    return temp;
#endif
}

};
