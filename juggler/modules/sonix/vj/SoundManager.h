
#ifndef _VRJUGGLER_SOUND_MANAGER_
#define _VRJUGGLER_SOUND_MANAGER_

#include <Kernel/vjConfigChunkHandler.h>
#include <Utils/vjSingleton.h>

namespace vrj
{

   //: vrjuggler sound manager
   //!PUBLIC_API:
   class SoundManager : public vjConfigChunkHandler, public vjSingleton<vrj::SoundManager>
   {
   public:
      //: Add the chunk to the configuration
      //! PRE: configCanHandle(chunk) == true
      virtual bool configAdd( vjConfigChunk* chunk );

      //: Remove the chunk from the current configuration
      //! PRE: configCanHandle(chunk) == true
      virtual bool configRemove( vjConfigChunk* chunk );

      //: Can the handler handle the given chunk?
      //! RETURNS: true - Can handle it
      //+          false - Can't handle it
      virtual bool configCanHandle( vjConfigChunk* chunk );

   public:
      //: Enable a frame to be drawn
      virtual void update();

      //: Blocks until the end of the frame
      //! POST: The frame has been rendered
      virtual void sync();

   protected:
      //: Constructor.
      SoundManager();
   };
}; // end namespace

#endif   /* _VRJUGGLER_SOUND_MANAGER_ */
