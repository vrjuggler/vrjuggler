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

#include <vjVec3.h>
#include <vjMatrix.h>
#include <vjCoord.h>


#ifndef _NAVIGATOR_H_
#define _NAVIGATOR_H_

//: Base class for navigation
class navigator
{
public:
   navigator() : mAllowRot( true ), mAllowTrans( true )
   {
      allowAxis[0] = allowAxis[1] = allowAxis[2] = true;    // Initialize to rot in all axes
   }

   virtual void update() = 0;
   void setRotAxis(bool allowX, bool allowY, bool allowZ);

   void allowTrans( const bool& state = true ){mAllowTrans = state;}
   void allowRot( const bool& state = true ){mAllowRot = state;}
   
   virtual void heading( float& pitch, float& yaw, float& roll )
   {
      mCurPos.getXYZEuler( pitch, yaw, roll );
      pitch = -pitch;
      yaw = -yaw;
      roll = -roll;
   }   

   vjMatrix getCurPos()
   { return mCurPos; }

   void setCurPos(vjMatrix pos)
   { mCurPos = pos; }

protected:
      
   // give a translation vector in local coordinates
   // i.e. if you want to move forward in the scene, just give (0,0,-1).
   // NOTE: the direction "forward" would be relative to current rotation
   bool navTranslate( vjVec3 trans );
   void navRotate( vjMatrix rot_mat );

protected:
   bool mAllowRot, mAllowTrans;
   vjMatrix    mCurPos;       // The current position or the user- In Juggler coords
   bool        allowAxis[3];  // The collider to use
};

bool navigator::navTranslate(vjVec3 trans)
{
   bool ret_val(false);
   vjCoord cur_pos(mCurPos);
   //cerr << "Cur P: " << cur_pos.pos << endl;
   //cerr << "Trans: " << trans << endl;
   //cerr << "    =: " << cur_pos.pos+trans << endl;

   // Pre mult cur_mat by the trans we need to do
   //trans *= -1;
   vjMatrix trans_mat;  
   trans_mat.makeTrans(trans[0],trans[1],trans[2]);
   mCurPos.postMult(trans_mat);
   return ret_val;
}



void navigator::navRotate( vjMatrix rot_mat )
{  
   mCurPos.postMult( rot_mat );
   mCurPos.constrainRotAxis( allowAxis[0], allowAxis[1], allowAxis[2], mCurPos );
}


void navigator::setRotAxis(bool allowX, bool allowY, bool allowZ)
{
   allowAxis[0] = allowX;
   allowAxis[1] = allowY;
   allowAxis[2] = allowZ;
}

#endif
