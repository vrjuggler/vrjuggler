
#ifndef AUDIOJUGGLER_32BIT_4X4_MATRIX
#define AUDIOJUGGLER_32BIT_4X4_MATRIX

namespace aj
{

//: 4x4 matrix oriented in columnar order
class Matrix44
{
// Constructors
public:
   //: Default Constructor
   //  NOTE: does no initialization, call makeIdentity() to init to identity matrix
   //        or use the copy constructor instead with Matrix44::identity()
   Matrix44();

   //: Copy constructor
   Matrix44( const Matrix44& M );

   //: Construct 4x4 matrix from 16 floats
   Matrix44( float a0, float a4, float a8,  float a12,
       float a1, float a5, float a9,  float a13,
       float a2, float a6, float a10, float a14,
       float a3, float a7, float a11, float a15 );

// Matrix methods
public:
   //: get a pointer to the matrix data
   float *            data() { return mData; }

   //: get a const pointer to the matrix data
   const float *      data() const { return mData; }

   //: get the inverse of this matrix
   // TODO: invertAff, invertOrtho, invertOrthoN, invertIdent
   bool         invertFull( const Matrix44& M );
   
   
   // all euler funcs take and return angles in radians
   void         getEulerXYZ( float& xRot, float& yRot, float& zRot ) const;
   void        getEulerZYX(  float& zRot, float& yRot, float& xRot ) const;
   void         makeEulerXYZ( const float& xRot, const float& yRot, const float& zRot );
   void         makeEulerZYX( const float& zRot, const float& yRot, const float& xRot );

   //: get the translation component of this matrix
   void         getTrans( float& tx, float& ty, float& tz ) const;

   //: make matrix an identity matrix
   void         makeIdent();

   //: returns an identity matrix.
   inline static const Matrix44& identity() { static Matrix44 im( 1.0f,0.0f,0.0f,0.0f, 0.0f,1.0f,0.0f,0.0f,
                                                                 0.0f,0.0f,1.0f,0.0f, 0.0f,0.0f,0.0f,1.0f);
                                       return im; }

   //: this = a * b
   // required: c, a, and b must each point to 16 floats
   // WARNING: This function is dangerous since you could pass a bad pointer, use at your own risk
   void             mult( const float* a, const float* b );

   //: this = a * b
   // required: c, a, and b must each point to 16 floats
   void             mult( const Matrix44& a, const Matrix44& b );

   //: this = M * this
   void                    preMult( const Matrix44& M );

   //: this = this * M
   void                    postMult( const Matrix44& M );

   //: returns memory element i(out of [0..15])
   inline float&           operator[]( int i ) { return mData[i]; }

   //: returns memory element i(out of [0..15])
   inline const float&     operator[]( int i ) const { return mData[i]; }

   //: returns element i, j
   inline float&           operator()( const int& i, const int& j ) { return mData[i*4+j]; }

   //: returns element i, j
   inline const float&     operator()( const int& i, const int& j ) const { return mData[i*4+j]; }

   //: this = M
   Matrix44&               operator=( const Matrix44& M );

   //: set the twist about an arbitrary axis.
   // NOTE: this erases any translation in this matrix
   void                    makeRot( const float& rad, const float& x, const float& y, const float& z );
   
   
   void                    setTrans( float x, float y, float z );
   void                    makeTrans( float x, float y, float z );

   //: set the matrix
   void              copy( const Matrix44& M );

   //: set the matrix with a float pointer
   // required: float pointer must point to a user-allocated array of 16 floats
   // WARNING: This function is dangerous since you could pass a bad pointer, use at your own risk
   void              set( const float* mat );
   
   // set each element in the matrix
   void              set( float a0, float a4, float a8,  float a12,
                          float a1, float a5, float a9,  float a13,
                          float a2, float a6, float a10, float a14,
                          float a3, float a7, float a11, float a15 );

//: Data members
private:
   // the matrix data.
   float mData[16];
};

}; // end namespace

#endif
