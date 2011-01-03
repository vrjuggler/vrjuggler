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

#ifndef _VPR_ILLEGAL_ARGUMENT_EXCEPTION_H_
#define _VPR_ILLEGAL_ARGUMENT_EXCEPTION_H_

#include <vpr/vprConfig.h>
#include <vpr/Util/Exception.h>


namespace vpr
{

/** \class IllegalArgumentException IllegalArgumentException.h vpr/Util/IllegalArgumentException.h
 *
 * Exception type thrown if an illegal/invalid argument is passed in to a
 * function.
 *
 * @since 1.1.11
 */
class VPR_CLASS_API IllegalArgumentException : public vpr::Exception
{
public:
   IllegalArgumentException(const std::string& msg,
                            const std::string& location = "")
      throw ();

   virtual ~IllegalArgumentException() throw ();

   virtual std::string getExceptionName() const
   {
      return "vpr::IllegalArgumentException";
   }
};

}


#endif /* _VPR_ILLEGAL_ARGUMENT_EXCEPTION_H_ */
