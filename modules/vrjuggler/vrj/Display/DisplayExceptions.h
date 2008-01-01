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

#ifndef _VRJ_DISPLAY_DISPLAY_EXCEPTIONS_H_
#define _VRJ_DISPLAY_DISPLAY_EXCEPTIONS_H_

#include <vrj/vrjConfig.h>

#include <vpr/Util/Exception.h>


namespace vrj
{

/** \class InvalidSurfaceException DisplayExceptions.h vrj/Display/DisplayExceptions.h
 *
 * The exception type thrown when the corners of a surface projection do not
 * form a valid surface.
 *
 * @since 2.1.12
 */
class VJ_CLASS_API InvalidSurfaceException : public vpr::Exception
{
public:
   InvalidSurfaceException(const std::string& msg,
                           const std::string& location = "")
      throw ();

   virtual ~InvalidSurfaceException() throw ();
};

}


#endif /* _VRJ_DISPLAY_DISPLAY_EXCEPTIONS_H_ */
