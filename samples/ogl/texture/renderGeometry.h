#ifndef _RENDER_GEOMETRY_H_
#define _RENDER_GEOMETRY_H_


class geom
{
public:
   //: render an interleaved vertex array of triangles with
   //  data format: GL_T2F_C4F_N3F_V3F
   //
   //  data: pointer to GL_T2F_C4F_N3F_V3F format geometry data
   //  size: number of verticies in the data...
   static void renderVertexArray( float* data, int size )
   {
      glEnable( GL_VERTEX_ARRAY );
      glInterleavedArrays( GL_T2F_C4F_N3F_V3F, 0, data ); //GL_T2F_C4F_N3F_V3F format
      glDrawArrays( GL_TRIANGLES, 0, size );
   }
};


#endif /* _RENDER_GEOMETRY_H_ */
