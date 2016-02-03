#include <drivers/Open/Trackd/TrackdSensorStandalone.h>
#include <assert.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>


int TrackdSensorStandalone::numSensors()
{
   assert(mTrack  != NULL);
   return trackd_tracker_num_sensors(mTrack);
}

// Return the position of the given sensor
gmtl::Matrix44f TrackdSensorStandalone::getSensorPos(int sensorNum)
{
   assert(mTrack != NULL && "We don't have a valid trackd memory area");
   assert(sensorNum < numSensors() && "Out of bounds request for a sensor");

   CAVE_SENSOR_ST* sensor_val;
   sensor_val = trackd_tracker_sensor(mTrack, sensorNum);

   // For Anthony Steed
   gmtl::Matrix44f ret_val;

   gmtl::Quatf rx,ry,rz,rt;

   gmtl::setRot( rx, gmtl::AxisAnglef(gmtl::Math::deg2Rad(sensor_val->elev), 1.0, 0.0, 0.0));
   gmtl::setRot( ry, gmtl::AxisAnglef(gmtl::Math::deg2Rad(sensor_val->azim), 0.0, 1.0, 0.0));
   gmtl::setRot( rz, gmtl::AxisAnglef(gmtl::Math::deg2Rad(sensor_val->roll), 0.0, 0.0, 1.0));

   rt = ry*rx*rz;

   gmtl::setRot( ret_val, rt );
   gmtl::setTrans( ret_val, gmtl::Vec3f( sensor_val->x, sensor_val->y, sensor_val->z) );

   return ret_val;
}

/**
 * Attaches to the memory segment with key (mShmKey).
 * @post mMem = address of the shared memory area.
 */
void TrackdSensorStandalone::attachToMem()
{
   assert(mShmKey != 0 && "Key was not set correctly");
   assert(mTrack  == NULL && "Already attached");
   mTrack = trackd_tracker_attach(mShmKey);
}

/** Release the memory segment of mMem. */
void TrackdSensorStandalone::releaseMem()
{
   assert(mTrack != NULL && "Trying to release trackd memory that was NULL");
   trackd_tracker_release(mTrack);
   mTrack = NULL;
}
