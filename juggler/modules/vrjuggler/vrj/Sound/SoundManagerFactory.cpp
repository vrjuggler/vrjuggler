#include <vrj/vrjConfig.h>

#ifdef USE_SONIX
#include <vrj/Sound/SoundManagerSonix.h>
#else
#include <vrj/Sound/SoundManager.h>
#endif

#include <vrj/Sound/SoundManagerFactory.h>

namespace vrj
{

   SoundManager& SoundManagerFactory::get()
   {
#     ifdef USE_SONIX
         static vrj::SoundManagerSonix temp;
         return temp;
#     else
         static SoundManager temp;
         return temp;
#     endif
   }

};
