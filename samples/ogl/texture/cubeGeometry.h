#ifndef _CUBE_GEOMETRY_H_
#define _CUBE_GEOMETRY_H_


class cubeGeometry
{
public:
   // number of verticies
   // this can be passed to opengl's glDrawArrays( .., .. );
   unsigned int size() { return cubeGeometry::mSize; }
   
   // format is: T2F_C4F_N3F_V3F
   float* data() { return (float*)cubeGeometry::mData; }

private:
   static const unsigned int mSize;
   static const float mData[432];
};


#endif /* _CUBE_GEOMETRY_H_ */
