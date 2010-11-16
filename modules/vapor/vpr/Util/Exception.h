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

#ifndef _VPR_EXCEPTION_H_
#define _VPR_EXCEPTION_H_

#include <vpr/vprConfig.h>

#include <string>
#include <stdexcept>

//#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

#define VPR_LOCATION std::string(__FILE__) + std::string(":") + \
           std::string(BOOST_PP_STRINGIZE(__LINE__))

/**
 * @example "Example of using VPR exceptions"
 *
 * All VPR exceptions derive from vpr::Exception, and its constructor takes
 * two parameters: a description of the error and an optional string
 * describing the location in the code at which the error occurred. The
 * easiest way to get the location information is to use the preprocessor
 * symbol \c VPR_LOCATION.
 *
 * \code
 * throw vpr::Exception("An error occurred", VPR_LOCATION);
 * \endcode
 */

// Exception areas
// - I/O loading/saving issues
// - Property access errors
// - Invalid data type errors

namespace vpr
{

/** \class Exception Exception.h vpr/Util/Exception.h
 *
 * Base exception for all VPR exceptions.
 *
 * @since 1.1.5
 */
class VPR_CLASS_API Exception : public std::runtime_error
{
public:
   Exception(const std::string& desc, const std::string& location) throw();
   virtual ~Exception() throw();

   virtual const char* what() const throw();

   virtual std::string getExceptionName() const;

   const std::string& getDescription() const;
   void setDescription(const std::string& desc);

   const std::string& getLocation() const;
   const std::string& getStackTrace() const;

   /** Slightly longer description */
   virtual std::string getExtendedDescription() const;

   /** Description with everything we know */
   virtual std::string getFullDescription() const;

protected:
   std::string mDescription;
   std::string mLocation;
   std::string mStackTrace;

   mutable std::string m_full_desc;    /**< Temporary string to return as char* where needed */
};

}


#endif
