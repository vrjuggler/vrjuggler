/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#include <Math/vjVec3.h>
#include <Math/vjMatrix.h>
#include <Math/vjCoord.h>


#ifndef _NAVIGATOR_H_
#define _NAVIGATOR_H_

//: Base class for navigation
class navigator
{
public:
   navigator()
   {
      allowAxis[0] = allowAxis[1] = allowAxis[2] = true;    // Initialize to rot in all axes
   }

   virtual void update(vjMatrix mat, bool allowTrans, bool allowRot) = 0;
   void setRotAxis(bool allowX, bool allowY, bool allowZ);

   vjMatrix getCurPos()
   { return mCurPos; }

   void setCurPos(vjMatrix pos)
   { mCurPos = pos; }

protected:
   bool navTranslate(vjVec3 trans);
   void navRotate(vjMatrix rot_mat);

protected:
   vjMatrix    mCurPos;       // The current position or the user- In Juggler coords
   bool        allowAxis[3];  // The collidor to use
};

bool navigator::navTranslate(vjVec3 trans)
{
   bool ret_val(false);
   vjCoord cur_pos(mCurPos);
   cerr << "Cur P: " << cur_pos.pos << endl;
   cerr << "Trans: " << trans << endl;
   cerr << "    =: " << (cur_pos.pos+trans) << endl;

   // Pre mult cur_mat by the trans we need to do
   //trans *= -1;
   vjMatrix trans_mat;  trans_mat.makeTrans(trans[0],trans[1],trans[2]);
   mCurPos.postMult(trans_mat);
   return ret_val;
}

void navigator::navRotate(vjMatrix rot_mat)
{
   // Restrict the rotation to only the axis specified
   float xRot, yRot, zRot;
   vjVec3 xAxis(1,0,0),yAxis(0,1,0),zAxis(0,0,1);

   if(!allowAxis[0]) // We don't want X
   {
      vjMatrix mx,mxInv;
      rot_mat.getXYZEuler(xRot,yRot,zRot);
      mx.makeRot(xRot,xAxis);
      mxInv.invert(mx);
      rot_mat.preMult(mxInv);                // Now we don't have X???
   }
   if(!allowAxis[1])    // We don't want Y
   {
      vjMatrix my,myInv;
      rot_mat.getZXYEuler(zRot,xRot,yRot);
      my.makeRot(yRot,yAxis);
      myInv.invert(my);
      rot_mat.postMult(myInv);                // Now we don't have Y???
   }
   if(!allowAxis[2])    // We don't want Z
   {
      vjMatrix mz,mzInv;
      rot_mat.getZYXEuler(zRot,yRot,xRot);
      mz.makeRot(zRot,zAxis);
      mzInv.invert(mz);
      rot_mat.preMult(mzInv);                // Now we don't have Z???
   }

   // Transform by inverse because we want to move the world, not us
   //vjMatrix inv_rot_mat;
   //inv_rot_mat.invert(rot_mat);
   mCurPos.postMult(rot_mat);
}


void navigator::setRotAxis(bool allowX, bool allowY, bool allowZ)
{
   allowAxis[0] = allowX;
   allowAxis[1] = allowY;
   allowAxis[2] = allowZ;
}

#endif
