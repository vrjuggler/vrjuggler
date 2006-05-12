/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _STRING_SUBJECT_IMPL_H_
#define _STRING_SUBJECT_IMPL_H_

#include <tweek/tweekConfig.h>

#include <string>

#include <vpr/vpr.h>
#include <vpr/Sync/Mutex.h>
#include <tweek/CORBA/SubjectImpl.h>
#include <StringSubject.h>


namespace CxxClientTest
{

/**
 * This class is an extension to the base Tweek SubjectImpl class.  It uses
 * multiple inheritance with that class and with the generated CORBA class
 * corresponding to the IDL for StringSubject.
 */
class StringSubjectImpl : public POA_CxxClientTest::StringSubject,
                          public tweek::SubjectImpl
{
public:
   StringSubjectImpl()
      : tweek::SubjectImpl()
      , mValue("")
   {
      /* Do nothing. */ ;
   }

   virtual ~StringSubjectImpl()
   {
      /* Do nothing. */ ;
   }

   /**
    * Sets this subject's internal value.
    */
   virtual void setValue(const char* value) throw(CORBA::SystemException);

   /**
    * Returns this subject's internal value.
    */
   virtual char* getValue() throw(CORBA::SystemException);

   /**
    * This overriding method is needed so that the correct type is returned
    * when the _this() method is invoked.  Without this method, an object of
    * type tweek::Subject_ptr would be returned.
    *
    * XXX: It may be possible to remove this requirement in the future.
    */
   CxxClientTest::StringSubject_ptr _this()
   {
      return POA_CxxClientTest::StringSubject::_this();
   }

private:
   std::string mValue;      /**< Our value */
   vpr::Mutex  mValueLock;  /**< A mutex to protect mValue accesses */
};

} // End of CxxClientTest namespace


#endif /* _STRING_SUBJECT_IMPL_H_ */
