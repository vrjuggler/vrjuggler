#ifndef _GADGET_TRACKD_SENSOR_STANDALONE_H_
#define _GADGET_TRACKD_SENSOR_STANDALONE_H_

#include <gadget/Devices/Open/Trackd/trackdmem.h>
#include <vrj/Math/Matrix.h>

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

   //: Get the number of sensors
   int numSensors();

   // Return the position of the given sensor
   vrj::Matrix getSensorPos(int sensorNum);

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

