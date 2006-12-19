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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date: 2005-01-16 17:51:42 -0600 (Sun, 16 Jan 2005) $
 * Version:       $Revision: 16627 $
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_DISPLAY_DISPLAY_EXCEPTIONS_H_
#define _VRJ_DISPLAY_DISPLAY_EXCEPTIONS_H_

#include <vrj/vrjConfig.h>

#include <exception>
#include <string>


namespace vrj
{

/** \class InvalidSurfaceException DisplayExceptions.h vrj/Display/DisplayExceptions.h
 *
 * The exception type thrown when the corners of a surface projection do not
 * form a valid surface.
 *
 * @since 2.0.2
 */
class VJ_CLASS_API InvalidSurfaceException : public std::exception
{
public:
   InvalidSurfaceException(const std::string& msg) throw ();

   virtual ~InvalidSurfaceException() throw ();

   virtual const char* what() const throw ();

protected:
   std::string mMessage;
};

}


#endif /* _VRJ_DISPLAY_DISPLAY_EXCEPTIONS_H_ */
