/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

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
