#ifndef _OSG_NAVIGATER_H
#define _OSG_NAVIGATER_H

#include <gmtl/EulerAngle.h>
#include <gmtl/EulerAngleOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>
#include <gmtl/VecOps.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Quat.h>
#include <gmtl/QuatOps.h>
#include <osg/MatrixTransform>
#include <vpr/IO/SerializableObject.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>


class NavData : public vpr::SerializableObject
{
public:
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader)
   {
      float pos_data[16];
      for ( unsigned n=0;n<16;n++ )
      { 
         pos_data[n] = reader->readFloat(); 
      }
      
      //gmtl::Matrix44f temp;
      //temp.set(pos_data);
      //mCurPos = temp;
      mCurPos.set(pos_data);
      return vpr::ReturnStatus::Succeed;
   }

   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer)
   {
      const float* pos_data = mCurPos.getData();
      for ( int n=0;n<16;n++ )
      {
         writer->writeFloat(pos_data[n]); 
      }
      return vpr::ReturnStatus::Succeed;
   }  
public:   
   gmtl::Matrix44f mCurPos;
};

class OsgNavigater
{
   enum NavMode
   {
      WALK,
      FLY
   };
public:
   OsgNavigater() : mActive(false), mMode(FLY)
   {;}

   void init()
   {
      mActive = true;
   }

   void setWalkMode(bool walk_mode)
   {
      if(walk_mode)
      {
         mMode = WALK;
      }
      else
      {
         mMode = FLY;
      }
   }

   void setVelocity(const gmtl::Vec3f& vel)
   {
      mVelocity = vel;
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Trans Vel: " << mVelocity << std::endl << vprDEBUG_FLUSH;
   }
   void setRotationalVelocity(const gmtl::Matrix44f& rot_vel)
   {
      mRotVelocity = rot_vel;
      vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL) << "Trans Vel: " << mRotVelocity << std::endl << vprDEBUG_FLUSH;
   }
   void update(float delta)
   {
      if(delta > 2.0 || !mActive)
      {
         return;
      }
      
      // Erase the translation from the rotation velocity.
      gmtl::Matrix44f rot = mRotVelocity;
      gmtl::setTrans(rot, gmtl::Vec3f(0.0f, 0.0f, 0.0f));
  
      // We now have a pure rotation and need to scale it.
      gmtl::Matrix44f result = scaled_rotate(rot, delta);

      
      if(mMode == WALK)
      {
         gmtl::EulerAngleXYZf euler( 0.0f, gmtl::makeYRot(result), 0.0f );// Only allow Yaw (rot y)
         gmtl::Matrix44f real = gmtl::makeRot<gmtl::Matrix44f>( euler ); 
         gmtl::postMult(mNavData.mCurPos, real);
      }
      else
      {
         gmtl::postMult(mNavData.mCurPos, result);
      }
      
     
      //real *= delta;

      // Do translation.
      gmtl::Vec3f trans_delta;
      if(mMode == WALK)
      {
         gmtl::Vec3f vel(mVelocity[0], 0.0f, mVelocity[2]);
         trans_delta = vel * delta;
      }
      else
      {
         trans_delta =  mVelocity * delta;
      }
      
      // Post multiply the delta translation
      gmtl::Matrix44f trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(trans_delta);

      gmtl::postMult(mNavData.mCurPos, trans_matrix);
            
      //osg::Matrix osg_trans_matrix;
      //osg_trans_matrix.set(trans_matrix.getData());
      
      //mNavTransform->postMult(osg_trans_matrix);
   }

   gmtl::Matrix44f getCurPos()
   {
      if(!mActive)
      {
         return gmtl::Matrix44f();
      }
      else
      {
         return mNavData.mCurPos;
      }
   }
   
   void setCurPos(const gmtl::Matrix44f& pos)
   {
      if(mActive)
      {
         mNavData.mCurPos = pos;
      }
   }
    
   gmtl::Matrix44f scaled_rotate(gmtl::Matrix44f rot_mat, float delta)
   {
      //: Confused by quaternions???
      //  All this does is scale the angle of rotation back by about %4
      //  Quaternions allow you to interpolate between rotations,
      //  thus getting a scale factor of the original (with respect to identity)

      // I should scale the rotation matrix here
      // I should also compute a relative rotation from the head here as well
      gmtl::Matrix44f transform, transformIdent;
      gmtl::Quatf     source_rot, goal_rot, slerp_rot;

      // Create an identity quaternion to rotate from
      gmtl::identity(transformIdent);

      // Create the goal rotation quaternion (the goal is the input matrix)
      gmtl::set(goal_rot, rot_mat);

      // If we don't have two identity matrices, then interpolate between them
      if(transformIdent != rot_mat)
      {
         gmtl::slerp(slerp_rot, /*0.04f **/ delta, source_rot, goal_rot ); // Transform part way there
         gmtl::set( transform, slerp_rot ); // Create the transform matrix to use
      }
      else
      {
         gmtl::identity(transform);
      }
      //navigator::navRotate(transform);                   // update the mCurPos navigation matrix
      return transform;
   }
private:
   //osg::MatrixTransform*   mNavTransform;
   bool                    mActive;
   NavMode                 mMode;
   
   NavData                 mNavData;
      
   //gmtl::Matrix44f         mCurPos;
  
   gmtl::Vec3f             mVelocity;
   gmtl::Matrix44f         mRotVelocity;
};
#endif /* _OSG_NAVIGATE_H */
