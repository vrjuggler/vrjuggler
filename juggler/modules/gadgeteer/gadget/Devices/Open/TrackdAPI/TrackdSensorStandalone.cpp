#include <gadget/Devices/Open/Trackd/TrackdSensorStandalone.h>
#include <assert.h>

#include <gmtl/Matrix.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/Quat.h>
#include <gmtl/AxisAngle.h>


int TrackdSensorStandalone::numSensors()
{
   assert(mMem != NULL);
   assert(mShmKey != 0);
#ifdef GADGET_HAVE_TRACKDAPI
   return mMem->trackdGetNumberOfSensors();
#else 
   return trackd_num_sensors(mMem);
#endif
}

// Return the position of the given sensor
gmtl::Matrix44f TrackdSensorStandalone::getSensorPos(int sensorNum, float scale)
{
   assert(mMem != NULL && "We don't have a valid trackd memory area");
   assert(sensorNum < numSensors() && "Out of bounds request for a sensor");

#ifdef GADGET_HAVE_TRACKDAPI
   gmtl::Matrix44f ret_val;
   float tmp[4][4]; 
   
   mMem->trackdGetMatrix(sensorNum, tmp);

   // AJS - some obvious set/get functions seem to be missing in GMTL
   ret_val[0]=tmp[0][0];
   ret_val[1]=tmp[0][1];
   ret_val[2]=tmp[0][2];
   ret_val[3]=tmp[0][3];

   ret_val[4]=tmp[1][0];
   ret_val[5]=tmp[1][1];
   ret_val[6]=tmp[1][2];
   ret_val[7]=tmp[1][3];

   ret_val[8]=tmp[2][0];
   ret_val[9]=tmp[2][1];
   ret_val[10]=tmp[2][2];
   ret_val[11]=tmp[2][3];

   ret_val[12]=tmp[3][0]*scale;
   ret_val[13]=tmp[3][1]*scale;
   ret_val[14]=tmp[3][2]*scale;
   ret_val[15]=tmp[3][3];

#else 

   CAVE_SENSOR_ST* sensor_val;
   sensor_val = trackd_sensor(mMem, sensorNum);

   gmtl::Matrix44f ret_val;

   gmtl::Quatf rx,ry,rz,rt;

   gmtl::setRot( rx, gmtl::AxisAnglef(gmtl::Math::deg2Rad(sensor_val->elev), 
				      1.0, 0.0, 0.0));
   gmtl::setRot( ry, gmtl::AxisAnglef(gmtl::Math::deg2Rad(sensor_val->azim), 
				      0.0, 1.0, 0.0));
   gmtl::setRot( rz, gmtl::AxisAnglef(gmtl::Math::deg2Rad(sensor_val->roll), 
				      0.0, 0.0, 1.0));

   rt = ry*rx*rz;

   gmtl::setRot( ret_val, rt );
   gmtl::setTrans( ret_val, gmtl::Vec3f( sensor_val->x*scale, sensor_val->y*scale, sensor_val->z*scale) );

#endif

   return ret_val;
}

//: Attach to the memory segment with key (mShmKey)
//! POST: mMem = address of the shared memory area
void TrackdSensorStandalone::attachToMem()
{
   assert(mShmKey != 0 && "Key was not set correctly");
#ifdef GADGET_HAVE_TRACKDAPI
   mMem = new TrackerReader(mShmKey);
#else 
   mMem = trackd_attach_tracker_mem(mShmKey);
#endif
}

//: Release the memory segment of mMem
void TrackdSensorStandalone::releaseMem()
{
   assert(mMem != NULL && "Trying to release trackd memory that was NULL");
#ifdef GADGET_HAVE_TRACKDAPI
   delete mMem;
#else 
   trackd_release_tracker_mem(mMem);
#endif
}

