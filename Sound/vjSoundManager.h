#include <Sound/vjSoundEngine.h>
#include <Sound/vjSound.h>
#include <Kernel/vjConfigChunkHandler.h>

class vjSoundManager : public vjConfigChunkHandler
{
public:     // --- Config stuff -- //
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   virtual bool configAdd(vjConfigChunk* chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   virtual bool configRemove(vjConfigChunk* chunk);

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(vjConfigChunk* chunk);

public:
   //: Enable a frame to be drawn
   virtual void update();

   //: Blocks until the end of the frame
   //! POST: The frame has been drawn
   virtual void sync();
  
   // given an alias, return the handle.
   // TODO: if called twice with name alias, should return same pointer.
   // memory managed by engine...
   // returns NULL if invalid name.
   vjSound* getHandle( const char* const alias );
   
   //: Factory function to create a new sound.
   // memory managed by engine
   virtual vjSound* newSound();
   
   vjSoundEngine* mSoundEngine;
};
