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

#include <vjConfig.h>
#include <Math/vjVec3.h>
#include <Math/vjMatrix.h>
#include <Math/vjVec4.h>


void vjVec3::xformFull(const vjMatrix& _m, const vjVec3& _v)
{
    vjVec4 tempVec(_v[0], _v[1], _v[2], 1.0f);
    tempVec.xform(_m, tempVec);

    float w_coord_div = 1.0f / tempVec.vec[3];

    vec[0] = (tempVec.vec[0]* w_coord_div);
    vec[1] = (tempVec.vec[1]* w_coord_div);
    vec[2] = (tempVec.vec[2]* w_coord_div);
}

void vjVec3::xformVec(const vjMatrix& _m, const vjVec3& _v)
{
    vjVec4 tempVec(_v[0], _v[1], _v[2], 0.0f);
    tempVec.xform(_m, tempVec);

    vec[0] = tempVec.vec[0];
    vec[1] = tempVec.vec[1];
    vec[2] = tempVec.vec[2];
}

VJ_IMPLEMENT(vjVec3) operator *(float _s, const vjVec3& _v) {
    return vjVec3(_v[0]*_s, _v[1]*_s, _v[2]*_s);
}

VJ_IMPLEMENT(vjVec3) operator *(const vjVec3& _v, float _s) {
    return vjVec3(_v[0]*_s, _v[1]*_s, _v[2]*_s);
}

VJ_IMPLEMENT(vjVec3) operator /(const vjVec3& _v, float _s) {
    _s = 1.0f/_s;
    return vjVec3(_v[0]*_s, _v[1]*_s, _v[2]*_s);
}

//vjVec3 operator *(const vjVec3& _v, const vjMatrix&  _m) {
//    // transform as point (w=1), assuming affine transformation
//    // i.e. does not use slower dst.xformFullPt().
//    vjVec3 dst; dst.xformPt(_v, _m); return dst;
//}

VJ_IMPLEMENT(std::ostream&) operator<<( std::ostream& out, const vjVec3& _v )
{
    for(int j=0;j<3;j++)
    {
    	out << _v.vec[j] << (j < 2 ? ", " : "");
    }

    return out;	
}
