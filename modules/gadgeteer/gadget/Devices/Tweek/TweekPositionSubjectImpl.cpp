/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#include <gmtl/Vec.h>
#include <gmtl/EulerAngle.h>
#include <gmtl/Generate.h>
#include <gmtl/Output.h>

#include <vpr/vpr.h>
#include <vpr/Sync/Guard.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>

#include <gadget/Util/Debug.h>
#include <gadget/Devices/Tweek/TweekGadget.h>
#include <gadget/Devices/Tweek/TweekPositionSubjectImpl.h>


namespace gadget
{

void TweekPositionSubjectImpl::setTranslation(CORBA::Float xTrans,
                                              CORBA::Float yTrans,
                                              CORBA::Float zTrans)
{
   const gmtl::Vec<CORBA::Float, 3> trans_vec(xTrans, yTrans, zTrans);

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "TweekPositionSubjectImpl::setTranslation(): Setting translation to "
      << trans_vec << std::endl << vprDEBUG_FLUSH;

   mMatrixLock.acquire();
   {
      gmtl::setTrans(mMatrix, trans_vec);
   }
   mMatrixLock.release();

   mMyDev->notifySample();

   tweek::SubjectImpl::notify();
}

void TweekPositionSubjectImpl::getTranslation(CORBA::Float& xTrans,
                                              CORBA::Float& yTrans,
                                              CORBA::Float& zTrans)
{
   gmtl::Vec<CORBA::Float, 3> trans_vec;

   mMatrixLock.acquire();
   {
      gmtl::setTrans(trans_vec, mMatrix);
   }
   mMatrixLock.release();

   xTrans = trans_vec[0];
   yTrans = trans_vec[1];
   zTrans = trans_vec[2];
}

void TweekPositionSubjectImpl::setOrientation(CORBA::Float xOrient,
                                              CORBA::Float yOrient,
                                              CORBA::Float zOrient)
{
   gmtl::EulerAngleXYZf angle(xOrient, yOrient, zOrient);

   vprDEBUG(gadgetDBG_INPUT_MGR, vprDBG_STATE_LVL)
      << "TweekPositionSubjectImpl::setOrientation(): Setting rotation to "
      << angle << std::endl << vprDEBUG_FLUSH;

   mMatrixLock.acquire();
   {
      gmtl::setRot(mMatrix, angle);
   }
   mMatrixLock.release();

   mMyDev->notifySample();

   tweek::SubjectImpl::notify();
}

void TweekPositionSubjectImpl::getOrientation(CORBA::Float& xOrient,
                                              CORBA::Float& yOrient,
                                              CORBA::Float& zOrient)
{
   vpr::Guard<vpr::Mutex> guard(mMatrixLock);
}

} // End of gadget namespace
