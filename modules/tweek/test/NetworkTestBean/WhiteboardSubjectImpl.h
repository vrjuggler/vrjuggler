/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _WHITEBOARD_SUBJECT_IMPL_H_
#define _WHITEBOARD_SUBJECT_IMPL_H_

#include <tweek/tweekConfig.h>

#include <string>
#include <vector>

#include <tweek/CORBA/SubjectImpl.h>
#include <WhiteboardSubject.h>


namespace networktest
{

/**
 */
class WhiteboardSubjectImpl : public POA_networktest::WhiteboardSubject,
                              public tweek::SubjectImpl
{
public:
   WhiteboardSubjectImpl()
      : tweek::SubjectImpl()
      , mText("")
      , mLastOffset(0)
      , mLastLength(0)
      , mLastChange("")
   {
      /* Do nothing. */ ;
   }

   virtual ~WhiteboardSubjectImpl()
   {
      /* Do nothing. */ ;
   }

   virtual void insertText(CORBA::Long offset, CORBA::Long length,
                           const char* text)
      throw(CORBA::SystemException);

   virtual void getLastChange(CORBA::Long& offset, CORBA::Long& length,
                              CORBA::String_out text)
      throw(CORBA::SystemException);

   virtual char* getAllText() throw(CORBA::SystemException);

   WhiteboardSubject_ptr _this()
   {
      return POA_networktest::WhiteboardSubject::_this();
   }

private:
   std::string mText;
   CORBA::Long mLastOffset;
   CORBA::Long mLastLength;
   std::string mLastChange;
};

} // End of networktest namespace


#endif /* _WHITEBOARD_SUBJECT_IMPL_H_ */
