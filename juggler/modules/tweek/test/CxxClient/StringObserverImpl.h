/***************** <Tweek heading BEGIN do not edit this line> ****************
 * Tweek
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 ***************** <Tweek heading END do not edit this line> *****************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _STRING_OBSERVER_IMPL_H_
#define _STRING_OBSERVER_IMPL_H_

#include <tweek/idl/Observer.h>

#include <StringSubject.h>


class StringObserverImpl : public POA_tweek::Observer
{
public:
   StringObserverImpl(CxxClientTest::StringSubject_var subject)
      : mSubject(subject)
   {
   }

   // XXX: It would be nice if the work done by detach() could be done in
   // the destructor, but that does not seem to satisfy the POA.  Whatever...
   virtual ~StringObserverImpl()
   {
   }

   virtual void update() throw(CORBA::SystemException);

   void detach()
   {
      mSubject->detach(_this());
   }

private:
   CxxClientTest::StringSubject_var mSubject;
};


#endif /* _STRING_OBSERVER_IMPL_H_ */
