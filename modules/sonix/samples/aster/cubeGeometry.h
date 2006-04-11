#ifndef CUBE_GEOM___
#define CUBE_GEOM___

class cubeGeometry
{
public:
   // number of verticies
   // this can be passed to opengl's glDrawArrays( .., .. );
   unsigned int size() const { return cubeGeometry::mSize; }
   
   // format is: T2F_C4F_N3F_V3F
   float* data() { return (float*)cubeGeometry::mData; }
   const float* data() const { return (float*)cubeGeometry::mData; }

   void render() const
   {
      glEnable( GL_VERTEX_ARRAY );
      glInterleavedArrays( GL_T2F_C4F_N3F_V3F, 0, (float*)mData ); //GL_T2F_C4F_N3F_V3F format
      glDrawArrays( GL_TRIANGLES, 0, mSize );
   }
   
private:
   static const unsigned int mSize;
   static const float mData[432];
};

#endif
