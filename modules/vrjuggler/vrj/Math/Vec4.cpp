/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vjConfig.h>
#include <vrj/Math/Vec4.h>

namespace vrj
{
   
void Vec4::xform(const Matrix& _m, Vec4 _v)
{
	// Take _v by value in case it is me
	// Treating Vector like a column vector
	// (4x4)(4x1) = (4x1)

   vec[0] = vec[1] = vec[2] = vec[3] = 0.0f;

   //for(int i=0;i<1;i++)
	for(int j=0;j<4;j++)
	   for(int k=0;k<4;k++)
		   vec[j] += (_v[k] * _m[k][j]);
}

inline Vec4 operator *(float _s, const Vec4& _v) {
    return Vec4(_v[0]*_s, _v[1]*_s, _v[2]*_s, _v[3]*_s);
}

inline Vec4 operator *(const Vec4& _v, float _s) {
    return Vec4(_v[0]*_s, _v[1]*_s, _v[2]*_s, _v[3]*_s);
}

inline Vec4 operator /(const Vec4& _v, float _s) {
    _s = 1.0f/_s;
    return Vec4(_v[0]*_s, _v[1]*_s, _v[2]*_s, _v[3]*_s);
}


std::ostream& operator<<(std::ostream& out, Vec4& _v)
{
    for(int j=0;j<4;j++)
    {
    	out << _v.vec[j] << (j < 3 ? ", " : "");
    }

    return out;	
}


};
