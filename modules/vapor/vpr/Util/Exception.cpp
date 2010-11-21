/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <VPR heading END do not edit this line> ******************/

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

#include <iostream>
#include <vpr/System.h>
#include <vpr/Util/Exception.h>

namespace vpr
{

Exception::Exception(const std::string& desc, const std::string& location)
   throw()
   : std::runtime_error(desc)
   , mDescription(desc)
   , mLocation(location)
{
   mStackTrace = vpr::System::getCallStack();
}

Exception::~Exception() throw()
{
   /* Do nothing. */ ;
}

const char* Exception::what() const throw()
{
   m_full_desc = getFullDescription();
   return m_full_desc.c_str();
}

std::string Exception::getExceptionName() const
{
   return std::string("vpr::Exception");
}

const std::string& Exception::getDescription() const
{
   return mDescription;
}

void Exception::setDescription(const std::string& desc)
{
   mDescription = desc;
}

const std::string& Exception::getLocation() const
{
   return mLocation;
}

const std::string& Exception::getStackTrace() const
{
   return mStackTrace;
}

std::string Exception::getExtendedDescription() const
{
   return this->getExceptionName() + std::string(": ") + getDescription();
}

std::string Exception::getFullDescription() const
{
   return getExtendedDescription() + std::string("  ") + mLocation +
             std::string("\n") + mStackTrace;
}

}
