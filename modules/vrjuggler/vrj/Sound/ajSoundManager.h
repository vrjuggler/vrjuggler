
#ifndef _VRJUGGLER_AUDIO_JUGGLER_SOUND_MANAGER_
#define _VRJUGGLER_AUDIO_JUGGLER_SOUND_MANAGER_

#include <Sound/SoundManager.h>

namespace vrj
{
   //: vrjuggler sound manager
   //!PUBLIC_API:
   class ajSoundManager : public SoundManager
   {
   public:
      //: Add the chunk to the configuration
      //! PRE: configCanHandle(chunk) == true
      virtual bool configAdd( ConfigChunk* chunk );

      //: Remove the chunk from the current configuration
      //! PRE: configCanHandle(chunk) == true
      virtual bool configRemove( ConfigChunk* chunk );

      //: Can the handler handle the given chunk?
      //! RETURNS: true - Can handle it
      //+          false - Can't handle it
      virtual bool configCanHandle( ConfigChunk* chunk );

   public:
      //: Enable a frame to be drawn
      virtual void update();

      //: Blocks until the end of the frame
      //! POST: The frame has been rendered
      virtual void sync();
   };
}; // end namespace

#endif   /* _VRJUGGLER_SOUND_MANAGER_ */
