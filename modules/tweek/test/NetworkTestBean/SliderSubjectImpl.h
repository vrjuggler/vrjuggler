/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _SLIDER_SUBJECT_IMPL_H_
#define _SLIDER_SUBJECT_IMPL_H_

#include <tweek/tweekConfig.h>

#include <vector>

#include <tweek/CORBA/SubjectImpl.h>
#include <SliderSubject.h>


namespace networktest
{

/**
 * This class is an extension to the base Tweek SubjectImpl class.  It uses
 * multiple inheritance with that class and with the generated CORBA class
 * corresponding to the IDL for SliderSubject.
 */
class SliderSubjectImpl : public POA_networktest::SliderSubject,
                          public tweek::SubjectImpl
{
public:
   SliderSubjectImpl()
      : tweek::SubjectImpl()
      , mValue(0)
   {
      /* Do nothing. */ ;
   }

   virtual ~SliderSubjectImpl()
   {
      /* Do nothing. */ ;
   }

   /**
    * Sets this subject's internal value.
    */
   virtual void setValue(CORBA::Long value) throw(CORBA::SystemException);

   /**
    * Returns this subject's internal value.
    */
   virtual CORBA::Long getValue() throw(CORBA::SystemException);

   /**
    * This overriding method is needed so that the correct type is returned
    * when the _this() method is invoked.  Without this method, an object of
    * type tweek::Subject_ptr would be returned.
    *
    * XXX: It may be possible to remove this requirement in the future.
    */
   networktest::SliderSubject_ptr _this()
   {
      return POA_networktest::SliderSubject::_this();
   }

private:
   CORBA::Long mValue;   /**< Our value */
};

} // End of networktest namespace


#endif /* _SLIDER_SUBJECT_IMPL_H_ */
