#ifndef _A_TRACKD_SENSOR_H_
#define _A_TRACKD_SENSOR_H_

#include <Input/Multi/trackdmem.h>
#include <Math/vjMatrix.h>

class aTrackdSensor
{
public:
   aTrackdSensor(int shmKey)
   {
      mShmKey = shmKey;
      mMem = NULL;

      // Attach immediately
      attachToMem();
   }

   ~aTrackdSensor()
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

