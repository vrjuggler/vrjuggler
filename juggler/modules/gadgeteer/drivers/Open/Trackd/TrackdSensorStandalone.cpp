#include <Input/Multi/aTrackdSensor.h>
#include <assert.h>

int aTrackdSensor::numSensors()
{
   assert(mMem != NULL);
   assert(mShmKey != 0);
   return trackd_num_sensors(mMem);
}

// Return the position of the given sensor
vrj::Matrix aTrackdSensor::getSensorPos(int sensorNum)
{
   assert(mMem != NULL && "We don't have a valid trackd memory area");
   assert(sensorNum < numSensors() && "Out of bounds request for a sensor");

   CAVE_SENSOR_ST* sensor_val;
   sensor_val = trackd_sensor(mMem, sensorNum);

   // XXX: This is untested and is probably wrong. :(
   vrj::Matrix ret_val;
   ret_val.makeXYZEuler(sensor_val->elev, sensor_val->azim, sensor_val->roll);
   ret_val.setTrans(sensor_val->x, sensor_val->y, sensor_val->z);

   return ret_val;
}

//: Attach to the memory segment with key (mShmKey)
//! POST: mMem = address of the shared memory area
void aTrackdSensor::attachToMem()
{
   assert(mShmKey != 0 && "Key was not set correctly");
   mMem = trackd_attach_tracker_mem(mShmKey);
}

//: Release the memory segment of mMem
void aTrackdSensor::releaseMem()
{
   assert(mMem != NULL && "Trying to release trackd memory that was NULL");
   trackd_release_tracker_mem(mMem);
}

