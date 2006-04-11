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

#ifndef _GADGET_TWEEK_POSITION_SUBJECT_IMPL_H_
#define _GADGET_TWEEK_POSITION_SUBJECT_IMPL_H_

#include <gadget/gadgetConfig.h>

#include <gmtl/Matrix.h>
#include <vpr/vpr.h>
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>
#include <tweek/CORBA/SubjectImpl.h>

#include <gadget/Devices/Tweek/TweekPositionSubject.h>


namespace gadget
{

class TweekGadget;

/**
 */
class TweekPositionSubjectImpl : public POA_gadget::TweekPositionSubject,
                                 public tweek::SubjectImpl
{
public:
   /**
    * Default constructor.
    */
   TweekPositionSubjectImpl(TweekGadget* device) : mMyDev(device)
   {
      ;
   }

   /**
    * Default destructor.
    */
   virtual ~TweekPositionSubjectImpl()
   {
      ;
   }

   virtual void setTranslation(CORBA::Float xTrans, CORBA::Float yTrans,
                               CORBA::Float zTrans);

   virtual void getTranslation(CORBA::Float& xTrans, CORBA::Float& yTrans,
                               CORBA::Float& zTrans);

   virtual void setOrientation(CORBA::Float xOrient, CORBA::Float yOrient,
                               CORBA::Float zOrient);

   virtual void getOrientation(CORBA::Float& xOrient, CORBA::Float& yOrient,
                               CORBA::Float& zOrient);

   gadget::TweekPositionSubject_ptr _this()
   {
      return POA_gadget::TweekPositionSubject::_this();
   }

   /**
    * Returns a copy of the current transformation matrix for this subject.
    */
   gmtl::Matrix<CORBA::Float, 4, 4> getTransformMatrix()
   {
      vpr::Guard<vpr::Mutex> guard(mMatrixLock);
      return mMatrix;
   }

private:
   // These two have to be here because Visual C++ will try to make them
   // exported public symbols.  This causes problems because copying
   // vpr::Mutex objects is not allowed.
   TweekPositionSubjectImpl(const TweekPositionSubjectImpl& subj)
   {
      /* Do nothing. */ ;
   }

   void operator=(const TweekPositionSubjectImpl& subj)
   {
      /* Do nothing. */ ;
   }

   TweekGadget* mMyDev;

   /** */
   gmtl::Matrix<CORBA::Float, 4, 4> mMatrix;
   vpr::Mutex mMatrixLock;
};

} // End of gadget namespace


#endif /* _GADGET_TWEEK_POSITION_SUBJECT_IMPL_H_ */
