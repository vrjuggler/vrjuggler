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
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _VPR_ERROR_BASE_H_
#define _VPR_ERROR_BASE_H_

#include <vpr/vprConfig.h>
#include <iostream>
#include <string>
#include <boost/concept_check.hpp>


namespace vpr
{

/** \class ErrorBase ErrorBase.h vpr/Util/ErrorBase.h
 *
 * Common base for platform-specific error management.
 */
class VPR_CLASS_API ErrorBase
{
public:
   enum ErrorType
   {
      NoError = 0
      /* Fill in common error types */
   };

   /** @since 1.1.6 */
   static std::string getCurrentErrorMsg()
   {
      return std::string("");
   }

   static void outputCurrentError(std::ostream& out, const std::string& prefix)
   {
      boost::ignore_unused_variable_warning(out);
      boost::ignore_unused_variable_warning(prefix);
   }

   static ErrorType getCurrentError()
   {
      return NoError;
   }
};

} // End of vpr namespace


#endif  /* _VPR_SELECTOR_BASE_H_ */
