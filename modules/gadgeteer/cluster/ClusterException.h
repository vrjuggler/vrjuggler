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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _CLUSTER_CLUSTER_EXCEPTION_H_
#define _CLUSTER_CLUSTER_EXCEPTION_H_

#include <exception>
#include <string>


namespace cluster
{
/**
 * Basic exception that may be thrown by the ClusterNetwork or any of the Plugins
 */
class ClusterException : public std::exception
{
public:
   ClusterException(const std::string& msg) : mMessage(msg)
   {;}

   virtual ~ClusterException() throw()
   {;}

   const std::string& getMessage() const
   {
      return mMessage;
   }

private:
   std::string mMessage;   /**< Error message contained in exception */
};

} // end namespace gadget

#endif
