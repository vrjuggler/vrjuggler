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

#include <WhiteboardSubjectImpl.h>


namespace networktest
{

void WhiteboardSubjectImpl::insertText(CORBA::Long offset, CORBA::Long length,
                                       const char* text)
   throw(CORBA::SystemException)
{
   mText.insert(offset, text);
   mLastOffset = offset;
   mLastLength = length;
   mLastChange = text;
   notify();
}

void WhiteboardSubjectImpl::getLastChange(CORBA::Long& offset,
                                          CORBA::Long& length,
                                          CORBA::String_out text)
   throw(CORBA::SystemException)
{
   offset = mLastOffset;
   length = mLastLength;
   text   = CORBA::string_dup(mLastChange.c_str());
}

char* WhiteboardSubjectImpl::getAllText()
   throw(CORBA::SystemException)
{
   // XXX: The client is responsible for releasing this memory!
   return CORBA::string_dup(mText.c_str());
}

} // End networktest namespace
