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

#include <gadget/gadgetConfig.h>
#include <float.h>
#include <vrj/Util/Debug.h>
#include <jccl/Config/ConfigChunk.h>
#include <gadget/Type/Position.h>

namespace gadget
{

// Set up the transformation information
bool Position::config(jccl::ConfigChunkPtr c)
{
  //vprDEBUG(vprDBG_ALL,0) << "vjPosition::config(jccl::ConfigChunkPtr)" << vprDEBUG_FLUSH;
  if ((c->getNum("translate") == 3) && (c->getNum("rotate") == 3))
  {
    // These are the transforms from the base tracker coord system

    float xt, yt, zt;
    xt = c->getProperty("translate",0);
    yt = c->getProperty("translate",1);
    zt = c->getProperty("translate",2);


    float xr, yr, zr;
    xr = c->getProperty("rotate",0);
    yr = c->getProperty("rotate",1);
    zr = c->getProperty("rotate",2);

      // This makes a rotation matrix that moves a pt in
      // the device's coord system to the vj coord system.
      // ==> world_M_transmitter
    rotMat.makeXYZEuler(xr, yr, zr);

    xformMat.makeTrans(xt, yt, zt);
    xformMat.postMult(rotMat);         // xformMat = T*R
  }

  return true;
}

jccl::TimeStamp* Position::getPosUpdateTime (int devNum)
{
    //vprDEBUG(vrjDBG_PERFORMANCE,2) << "vjPosition::getPosUpdateTime: Get update time function not implemented for this class: " << typeid(this).name() << std::endl << vprDEBUG_FLUSH;
   return NULL;
}

Position::Position()
{
  //vprDEBUG(vprDBG_ALL,0) << "vjPosition::Position()" << vprDEBUG_FLUSH;
  theData = NULL;
  mDataTimes = NULL;
  xformMat.makeIdent();
}

Position::~Position()
{
    /*  I didn't allocate it, so I should not deallocate it
    if (theData != NULL)
       getMyMemPool()->deallocate((void*)theData);
    */
    if (mDataTimes != NULL)
   delete mDataTimes;
}

};
