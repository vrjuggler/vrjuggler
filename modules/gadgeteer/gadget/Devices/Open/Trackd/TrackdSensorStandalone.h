#ifndef _GADGET_TRACKD_SENSOR_STANDALONE_H_
#define _GADGET_TRACKD_SENSOR_STANDALONE_H_

#include <gadget/Devices/Open/Trackd/trackdmem.h>
#include <gmtl/Matrix.h>

class TrackdSensorStandalone
{
public:
   TrackdSensorStandalone(int shmKey)
   {
      mShmKey = shmKey;
      mMem = NULL;

      // Attach immediately
      attachToMem();
   }

   ~TrackdSensorStandalone()
   {
      releaseMem();
   }

   /** Gets the number of sensors. */
   int numSensors();

   /** Returns the position of the given sensor. */
   gmtl::Matrix44f getSensorPos(int sensorNum);

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

