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
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _VPR_ERROR_IMPL_POSIX_H_
#define _VPR_ERROR_IMPL_POSIX_H_

#include <vpr/vprConfig.h>
#include <iostream>
#include <string>
#include <vpr/Util/ErrorBase.h>


namespace vpr
{

/**
 * POSIX Implementation of ---> Cross-platform Error reporting class.
 */
class VPR_CLASS_API ErrorImplPosix : public ErrorBase
{
public:
   
   static void outputCurrentError(std::ostream& out, const std::string& prefix);

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


#endif  /* _VPR_ERROR_IMPL_POSIX_H_ */
