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

   /** Gets the num input values. */
   int numButtons();
   int numValuators();

   /** Returns the value of the button. */
   int getButton(int btnNum);

   float getValuator(int valNum);

protected:
   /**
    * Attaches to the memory segment with key (mShmKey).
    * @post mMem = address of the shared memory area.
    */
   void attachToMem();

   /** Releases the memory segment of mMem. */
   void releaseMem();

private:
   int   mShmKey;       /**< The key to the shared memory area. */
   void* mMem;          /**< The memory area. */
};


#endif
