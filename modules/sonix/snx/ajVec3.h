
#ifndef VEC3__INCLUDED
#define VEC3__INCLUDED

class ajVec3
{
public:
   ajVec3(){ mData[0] = mData[1] = mData[2] = 0.0f; }
   ajVec3( const ajVec3& vec );
   ajVec3( const float& v0, const float& v1, const float& v2 );

   void            copy( const ajVec3& vec );
   void            set( const float* vec );
   void            set( const float& v0, const float& v1, const float& v2 );
   void            get( float &v0, float &v1, float &v2 ) const;

   ajVec3            cross( const ajVec3& vec ) const;
   float             dot( const ajVec3& vec ) const;
   float             lengthSquared() const;
   float             length() const;
   float             normalize();
   
   ajVec3&             operator*=( const float& value );
   ajVec3&             operator/=( const float& value );
   ajVec3&             operator+=( const ajVec3& vec );
   ajVec3&             operator-=( const ajVec3& vec );
   ajVec3&             operator=( const ajVec3& vec );
   void                negate();

   inline float*             data() { return mData; }
   inline const float*       data() const { return mData; }
   
   inline float &            operator[]( int i ) { return mData[i]; }
   inline const float &      operator[]( int i ) const { return mData[i]; }
   
protected:
   float mData[3];
};





//  Set the value.
inline void ajVec3::set( const float& v0, const float& v1, const float& v2 )
{
   mData[0] = v0;
   mData[1] = v1;
   mData[2] = v2;
}

//  Set the value.
inline void ajVec3::set( const float* v )
{
   this->set( v[0], v[1], v[2] );
}

//  Get the value.
inline void ajVec3::get( float &v0, float &v1, float &v2 ) const
{
   v0 = mData[0];
   v1 = mData[1];
   v2 = mData[2];
}

//  Set the value.
inline void ajVec3::copy( const ajVec3& vec )
{
   this->set( vec.mData );
}

//  Constructor.
inline ajVec3::ajVec3( const ajVec3& vec )
{
   this->copy( vec );
}

//  Constructor.
inline ajVec3::ajVec3( const float& v0, const float& v1, const float& v2 )
{
   this->set( v0, v1, v2 );
}


//  Return the cross product.
inline ajVec3 ajVec3::cross( const ajVec3& vec ) const
{
   return ajVec3 ( mData[1] * vec[2] - mData[2] * vec[1],
                   mData[2] * vec[0] - mData[0] * vec[2],
                   mData[0] * vec[1] - mData[1] * vec[0] );
}

//  Return the dot product.
inline float ajVec3::dot ( const ajVec3& vec ) const
{
   return mData[0] * vec[0] +
          mData[1] * vec[1] +
          mData[2] * vec[2];
}

//  Return the length squared. (same as dot product)
inline float ajVec3::lengthSquared() const
{
   return mData[0] * mData[0] + 
          mData[1] * mData[1] + 
          mData[2] * mData[2];
}

//  Return the length. (kev::SQRT of dot product)
inline float ajVec3::length() const
{
   return kev::SQRT( this->lengthSquared() );
}

//  Normalize, return the length prior to normalization.
inline float ajVec3::normalize()
{
   float invLength = 1.0f / this->length();

   mData[0] *= invLength;
   mData[1] *= invLength;
   mData[2] *= invLength;

   return length;
}

//  Multiply all the components by the value.
inline ajVec3& ajVec3::operator*=( const float& value )
{
   mData[0] *= value;
   mData[1] *= value;
   mData[2] *= value;

   return *this;
}

//  Divide all the components by the value.
inline ajVec3& ajVec3::operator/=( const float& value )
{
   float invValue = 1.0f / value;

   mData[0] *= invValue;
   mData[1] *= invValue;
   mData[2] *= invValue;

   return *this;
}

//  Add the vector to this one.
inline ajVec3& ajVec3::operator+=( const ajVec3& vec )
{
   mData[0] += vec[0];
   mData[1] += vec[1];
   mData[2] += vec[2];

   return *this;
}

//  Subtract the vector from this one.
inline ajVec3& ajVec3::operator-=( const ajVec3& vec )
{
   mData[0] -= vec[0];
   mData[1] -= vec[1];
   mData[2] -= vec[2];

   return *this;
}

//  Assign this vector.
inline ajVec3& ajVec3::operator=( const ajVec3& vec )
{
   this->copy( vec );
   return *this;
}

//  make the vec it's negative (flip 180 about origin)
inline void ajVec3::negate()
{
   mData[0] = -mData[0];
   mData[1] = -mData[1];
   mData[2] = -mData[2];
}

#endif
