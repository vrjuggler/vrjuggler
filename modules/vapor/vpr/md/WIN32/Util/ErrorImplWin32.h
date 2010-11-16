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

#ifndef _VPR_ERROR_IMPL_WIN32_H_
#define _VPR_ERROR_IMPL_WIN32_H_

#include <vpr/vprConfig.h>
#include <iostream>
#include <string>
#include <vpr/Util/ErrorBase.h>


namespace vpr
{

/** \class ErrorImplWin32 ErrorImplWin32.h vpr/Util/Error.h
 *
 * Win32 implementation of cross-platform error reporting class.
 *
 * @since 1.1.36
 */
class VPR_CLASS_API ErrorImplWin32 : public ErrorBase
{
public:
   /**
    * Returns the current error message from the OS for a failed operation.
    * If the current error code is \c ERROR_SUCCESS or there is no message for
    * the current error, then an empty string is returned.
    */
   static std::string getCurrentErrorMsg();

   static void outputCurrentError(std::ostream& out,
                                  const std::string& prefix);

   static ErrorType getCurrentError()
   {
      return NoError;
   }

protected:
/*
   static int convertErrorVprToNspr(ErrorType mask)
   {
      return 0;
   }

   static ErrorType convertErrorNsprToVpr(int mask)
   {
      return NoError;
   }
*/
};

} // End of vpr namespace


#endif  /* _VPR_ERROR_IMPL_WIN32_H_ */
