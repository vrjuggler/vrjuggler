
#ifndef _VJ_SOUND_MANAGER_H_
#define _VJ_SOUND_MANAGER_H_

#include <Kernel/vjConfigChunkHandler.h>

//: vrjuggler sound manager
//!PUBLIC_API:
class vjSoundManager : public vjConfigChunkHandler
{
public:     // --- Config stuff -- //
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
   vjSoundManager();
};

#endif   /* _VJ_SOUND_MANAGER_H_ */
