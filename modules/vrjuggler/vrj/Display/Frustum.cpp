/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>

#include <vrj/Display/Frustum.h>

namespace vrj
{

Frustum::Frustum()
   : mFrust(6, 0.0f)
{
   /* Do nothing. */ ;
}

void Frustum::setBottomLeftTopRight(const float bottomVal,
                                    const float leftVal, const float topVal,
                                    const float rightVal)
{
   mFrust[VJ_BOTTOM] = bottomVal;
   mFrust[VJ_LEFT]   = leftVal;
   mFrust[VJ_TOP]    = topVal;
   mFrust[VJ_RIGHT]  = rightVal;
}

void Frustum::set(const float leftVal, const float rightVal,
                  const float bottomVal, const float topVal,
                  const float nearVal, const float farVal)
{
   mFrust[VJ_LEFT]   = leftVal;
   mFrust[VJ_RIGHT]  = rightVal;
   mFrust[VJ_BOTTOM] = bottomVal;
   mFrust[VJ_TOP]    = topVal;
   mFrust[VJ_NEAR]   = nearVal;
   mFrust[VJ_FAR]    = farVal;
}

// ---- FRIEND FUNCTIONS ---- //
std::ostream& operator<<(std::ostream& out, const Frustum& frust)
{
   out << "l: " << frust[Frustum::VJ_LEFT] << " "
       << "r: " << frust[Frustum::VJ_RIGHT] << " "
       << "b: " << frust[Frustum::VJ_BOTTOM] << " "
       << "t: " << frust[Frustum::VJ_TOP] << " "
       << "n: " << frust[Frustum::VJ_NEAR] << " "
       << "f: " << frust[Frustum::VJ_FAR];

   return out;
}

}
