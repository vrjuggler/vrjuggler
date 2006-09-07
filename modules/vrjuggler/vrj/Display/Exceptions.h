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

#ifndef _VRJ_DISPLAY_EXCEPTIONS_H_
#define _VRJ_DISPLAY_EXCEPTIONS_H_

#include <exception>
#include <string>


namespace vrj
{

/**
 * The exception type thrown when the corners of a surface projection do not
 * form a valid surface.
 *
 * @since 2.0.2
 */
class InvalidSurfaceException : public std::exception
{
public:
   InvalidSurfaceException(const std::string& msg) : mMessage(msg)
   {
      ;
   }

   virtual ~InvalidSurfaceException() throw ()
   {
      ;
   }

   virtual const char* what() const throw ()
   {
      return mMessage.c_str();
   }

protected:
   std::string mMessage;
};

}


#endif /* _VRJ_DISPLAY_EXCEPTIONS_H_ */
