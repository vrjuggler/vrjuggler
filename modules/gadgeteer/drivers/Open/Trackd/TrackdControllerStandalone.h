#ifndef _GADGET_TRACKD_CONTROLLER_STANDALONE_H_
#define _GADGET_TRACKD_CONTROLLER_STANDALONE_H_

#include <stdlib.h>
#include <gadget/Devices/Open/Trackd/trackdmem.h>

class TrackdControllerStandalone
{
public:
   TrackdControllerStandalone(int shmKey)
   {
      mShmKey = shmKey;
      //mMem = NULL;

      // Attach immediately
      attachToMem();
   }

   ~TrackdControllerStandalone()
   {
      releaseMem();
   }

   //: Get the num input values
   int numButtons();
   int numValuators();

   // Return the value of the button
   int getButton(int btnNum);

   float getValuator(int valNum);

protected:
   //: Attach to the memory segment with key (mShmKey)
   //! POST: mMem = address of the shared memory area
   void attachToMem();

   //: Release the memory segment of mMem
   void releaseMem();

private:
   int   mShmKey;       // The key to the shared memory area
   void* mMem;          // The memory area
};

#endif

