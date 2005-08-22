/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

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

#include <vpr/vprConfig.h>

#include <sstream>

#include <vpr/Thread/UncaughtThreadException.h>

namespace vpr
{

UncaughtThreadException::UncaughtThreadException(const std::string& msg,
                                                 const std::string& location)
   throw()
   : Exception(msg, location)
{
   /* Do nothing. */ ;
}

UncaughtThreadException::~UncaughtThreadException() throw()
{
   /* Do nothing. */ ;
}

void UncaughtThreadException::setException(const vpr::Exception& ex)
{
   std::stringstream desc_stream;
   desc_stream << ex.getExceptionName() << ": " + ex.getDescription();

   mDescription = desc_stream.str();
   mLocation = ex.getLocation();
   mStackTrace = ex.getStackTrace();
}

void UncaughtThreadException::setException(const std::exception& ex)
{
   mDescription = ex.what();
   mLocation = "Location not availible with std::exception.";
   mStackTrace = "Stacktrace not availible with std::exception.";
}

}
