#ifndef _VRJ_MATRIX_H_
#define _VRJ_MATRIX_H_

#include <gmtl/Vec.h>
#include <gmtl/Matrix.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/Generate.h>
#include <gmtl/Math.h>
#include <gmtl/Xforms.h>

namespace vrj
{

class Matrix : public gmtl::Matrix44f
{  
public:

   Matrix()
   {
      makeIdent();
   }

   Matrix(const Matrix& m)
   {
      for (int i=0; i < 16; i++)
         mData[i] = m.mData[i];
   }

   Matrix(const gmtl::Matrix44f& m)
   {
      for (int i=0; i < 16; i++)
         mData[i] = m.mData[i];
   }

   //Create Identity Matrix
   inline void makeIdent()
   {
      gmtl::identity( *this );
   }

   //Zero out the matrix
   inline void zero()
   {
      gmtl::zero( *this ); 
   }

   //Make a matrix from 3 Euler angles
   inline void makeXYZEuler(float xRot, float yRot, float zRot)
   {
      *this = gmtl::makeRot<gmtl::Matrix44f>( gmtl::EulerAngleXYZf(gmtl::Math::deg2Rad(xRot), gmtl::Math::deg2Rad(yRot), gmtl::Math::deg2Rad(zRot)) );
   }

   inline void makeZYXEuler(float zRot, float yRot, float xRot)
   {
      *this = gmtl::makeRot<gmtl::Matrix44f>( gmtl::EulerAngleZYXf(gmtl::Math::deg2Rad(zRot), gmtl::Math::deg2Rad(yRot), gmtl::Math::deg2Rad(xRot)) );
   }

   inline void makeZXYEuler(float zRot, float xRot, float yRot)
   {
      *this = gmtl::makeRot<gmtl::Matrix44f>( gmtl::EulerAngleZXYf(gmtl::Math::deg2Rad(zRot), gmtl::Math::deg2Rad(xRot), gmtl::Math::deg2Rad(yRot)) );
   }

   inline void makeXYZEuler(gmtl::Vec3f xyz_vec) //New
   {
      makeXYZEuler( xyz_vec[0], xyz_vec[1], xyz_vec[2] );
   }


   //Make a matrix from a rotation about an axis
   inline void makeRot(float degrees, gmtl::Vec3f axis)
   {
      *this = gmtl::makeRot<gmtl::Matrix44f>( gmtl::AxisAnglef(gmtl::Math::deg2Rad(degrees), gmtl::Vec3f(axis[0], axis[1], axis[2])) );
   }

   //Get Euler angles from a matrix
   inline void getXYZEuler(float& xRot, float& yRot, float& zRot) const
   {
      gmtl::EulerAngleXYZf angles;
      gmtl::setRot( angles, *this  );
      xRot = gmtl::Math::rad2Deg(angles[0]);
      yRot = gmtl::Math::rad2Deg(angles[1]);
      zRot = gmtl::Math::rad2Deg(angles[2]);
   }

   inline void getZYXEuler(float& zRot, float& yRot, float& xRot) const
   {
      gmtl::EulerAngleXYZf angles;
      gmtl::setRot( angles, *this  );
      xRot = gmtl::Math::rad2Deg(angles[0]);
      yRot = gmtl::Math::rad2Deg(angles[1]);
      zRot = gmtl::Math::rad2Deg(angles[2]);
   }

   inline void getZXYEuler( float& zRot, float& xRot, float& yRot ) const
   {
      gmtl::EulerAngleXYZf angles;
      gmtl::setRot( angles, *this  );
      xRot = gmtl::Math::rad2Deg(xRot);
      yRot = gmtl::Math::rad2Deg(yRot);
      zRot = gmtl::Math::rad2Deg(zRot);
   }

   inline gmtl::Vec3f getXYZEuler() const //New
   {
      gmtl::EulerAngleXYZf angles;
      gmtl::setRot( angles, *this  );
      return gmtl::Vec3f( gmtl::Math::rad2Deg(angles[0]), 
                   gmtl::Math::rad2Deg(angles[1]),
                   gmtl::Math::rad2Deg(angles[2]) );
   }


   //Extract individual axis rotation info from the matrix
   inline float getXRot() const
   {
      gmtl::EulerAngleXYZf angles;
      gmtl::setRot( angles, *this  );
      return gmtl::Math::rad2Deg( angles[0] );
   }

   inline float getPitch() const
   {
      return getXRot();
   }

   inline float getYRot() const
   {
      gmtl::EulerAngleXYZf angles;
      gmtl::setRot( angles, *this  );
      return gmtl::Math::rad2Deg( angles[1] );
   }

   inline float getYaw() const
   {
      return getYRot();
   }

   inline float getZRot() const
   {
      gmtl::EulerAngleXYZf angles;
      gmtl::setRot( angles, *this );
      return gmtl::Math::rad2Deg( angles[2] );
   }

   inline float getRoll() const
   {
      return getZRot();
   }


   //Make a matrix from 3 translation values
   inline void makeTrans(float x, float y, float z)
   {
      *this = gmtl::makeTrans<gmtl::Matrix44f>( gmtl::Vec3f(x, y, z) );
   }

   inline void makeTrans(const gmtl::Vec3f& trans)
   {
      *this = gmtl::makeTrans<gmtl::Matrix44f>( trans ); 
   }

   //Set given translation to current matrix and keep existing rotation
   inline void setTrans(float x, float y, float z)
   {
      gmtl::setTrans( *this, gmtl::Vec3f(x, y, z) );
   }

   inline void setTrans(const gmtl::Vec3f& trans)
   {
      gmtl::setTrans( *this, trans );
   }


   //Get the translation portion of the matrix
   inline void getTrans(float& x, float& y, float& z) const
   {
      gmtl::Vec3f trans;
      gmtl::setTrans( trans, *this );
      x = trans[0]; y = trans[1]; z = trans[2];
   }

   inline gmtl::Vec3f getTrans() const
   {
      gmtl::Vec3f trans;
      gmtl::setTrans( trans, *this );
      return trans;
   }


   //Make a scaling matrix
   inline void makeScale(float x, float y, float z)
   {
      *this = gmtl::makeScale<gmtl::Matrix44f>( gmtl::Vec3f(x, y, z) );
   }

   inline void makeScale(float scale)
   {
      makeScale( scale, scale, scale );
   }

   //Copy another matrix
   inline void copy( const Matrix& m )
   {
      *this = m;
   }

   //Compare matrix for equality
   inline bool equal(const Matrix& m) const
   {
      return (*this == m);
   }

   //Clamps the values of the matrix to zero
   inline void zeroClamp()
   {
      for (int i=0; i < 16; i++)
      {
         mData[i] = gmtl::Math::zeroClamp( mData[i] );
      }
   }


   //Set to the transpose of the matrix
   inline void transpose(Matrix& m)
   {
      gmtl::transpose( *this, m );
   }

   inline void transpose() //New
   {
      gmtl::transpose( *this );
   }

   //Multiply 2 matrices        
   inline void mult(const Matrix& m1, const Matrix& m2)
   {
      gmtl::mult( *this, m1, m2 );
   }

   //Add 2 matrices
   inline void add(const Matrix& m1, const Matrix& m2)
   {
      gmtl::add( *this, m1, m2 );
   }

   //Subtract a matrix
   inline void sub(const Matrix& m1, const Matrix& m2)
   {
      gmtl::sub( *this, m1, m2 );
   }

   //Scale a matrix by a scalar
   inline void scale(float s, const Matrix& m)
   {
      *this = m;
      gmtl::mult( *this, s );
   }

   //Post multiply by a matrix
   inline void postMult(const Matrix& m)
   {
      gmtl::postMult( *this, m );
   }

   //Pre multiply by a matrix
   inline void preMult(const Matrix& m)
   {
      gmtl::preMult( *this, m );
   }

   //Find inverse of a matrix
   inline void invert(const Matrix& m)
   {
      gmtl::invert( *this, m );
   }

   inline void invert() //New
   {
      gmtl::invert( *this );
   }

   //Full inverse
   inline void invertFull(const Matrix& m)
   {
      invert( m );
   }

   inline void invertFull()
   {
      invert();
   }

   //Pre translate a matrix
   inline void preTrans(float x, float y, float z, const Matrix& m)
   {
      preTrans( x, y, z, m);
   }

   inline void preTrans(gmtl::Vec3f& trans, const Matrix& m)
   {
      *this = m;
      preMult( gmtl::makeTrans<gmtl::Matrix44f>( trans ) );
   }


   //Post translate a matrix        
   inline void postTrans(const Matrix& m, float x, float y, float z)
   {
      postTrans( m, x, y, z);
   }

   inline void postTrans(const Matrix& m, gmtl::Vec3f& trans)
   {
      *this = m;
      postMult( gmtl::makeTrans<gmtl::Matrix44f>( trans ) );
   }

   //Pre rotate a matrix
   inline void preRot(const float& degrees, const gmtl::Vec3f& axis, const Matrix& m)
   {
      *this = m;
      preMult( gmtl::makeRot<gmtl::Matrix44f>( gmtl::AxisAnglef(gmtl::Math::deg2Rad(degrees), gmtl::Vec3f(axis[0], axis[1], axis[2]))) ); 
   }

   inline void preXYZEuler( float xRot, float yRot, float zRot, const Matrix& m )
   {
      *this = m;
      preMult( gmtl::makeRot<gmtl::Matrix44f>( gmtl::EulerAngleXYZf(gmtl::Math::deg2Rad(xRot), gmtl::Math::deg2Rad(yRot), gmtl::Math::deg2Rad(zRot)) ));
   }

   //Post rotate a matrix
   inline void postRot( const Matrix& m, const float& degrees, const gmtl::Vec3f& axis )
   {
      *this = m;
      postMult( gmtl::makeRot<gmtl::Matrix44f>( gmtl::AxisAnglef(gmtl::Math::deg2Rad(degrees), gmtl::Vec3f(axis[0], axis[1], axis[2]))) );
   }

   inline void postXYZEuler( const Matrix& m, float xRot, float yRot, float zRot )
   {
      *this = m;
      postMult( gmtl::makeRot<gmtl::Matrix44f>( gmtl::EulerAngleXYZf(gmtl::Math::deg2Rad(xRot), gmtl::Math::deg2Rad(yRot), gmtl::Math::deg2Rad(zRot)) ));
   }


   //Pre scale a matrix
   inline void preScale( float xs, float ys, float zs, const Matrix& m )
   {
      *this = m;
      preMult( gmtl::makeScale<gmtl::Matrix44f>( gmtl::Vec3f(xs, ys, zs))); 
   }

   //Post scale a matrix
   inline void postScale( const Matrix& m, float xs, float ys, float zs )
   {
      *this = m;
      postMult( gmtl::makeScale<gmtl::Matrix44f>( gmtl::Vec3f(xs, ys, zs))); 
   }

   inline const float* getFloatPtr() const
   {
      return getData();
   }

   //New copy operators to go from Matrix44f to vrj::Matrix and vice versa
   inline Matrix& operator=( const gmtl::Matrix44f& m )
   {
      for (int i=0; i < 16; i++)
         mData[i] = m.mData[i];

      return *this;
   }

   //Make a matrix using direction cosines
   inline void makeDirCos( gmtl::Vec3f secXAxis, gmtl::Vec3f secYAxis, gmtl::Vec3f secZAxis )
   {
      *this = gmtl::makeDirCos<gmtl::Matrix44f>( secXAxis, secYAxis, secZAxis );
   }

   //Make matrix from given quaternion
   inline void makeQuaternion( const gmtl::Quatf& q )
   {
      gmtl::AxisAnglef rot;
      gmtl::setRot( rot, q );
      *this = gmtl::makeRot<gmtl::Matrix44f>( rot );
   }
        
}; //End of class

}; //End of namespace

#endif
