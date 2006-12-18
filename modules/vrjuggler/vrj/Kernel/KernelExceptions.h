/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VRJ_KERNEL_KERNEL_EXCEPTIONS_H_
#define _VRJ_KERNEL_KERNEL_EXCEPTIONS_H_

#include <vrj/vrjConfig.h>

#include <vpr/Util/Exception.h>


namespace vrj
{

/** \class ManagerException KernelExceptions.h vrj/Kernel/KernelExceptions.h
 *
 * General exception type for managers held by vrj::Kernel.
 */
class VJ_CLASS_API ManagerException : public vpr::Exception
{
public:
   ManagerException(const std::string& msg, const std::string& location = "")
      throw ();

   virtual ~ManagerException() throw ();
};

/** \class DrawMgrException Exceptions.h vrj/Kernel/Exceptions.h
 *
 * Exception type to be used by Draw Managers.
 */
class VJ_CLASS_API DrawMgrException : public ManagerException
{
public:
   DrawMgrException(const std::string& msg, const std::string& location = "")
      throw ();

   virtual ~DrawMgrException() throw ();
};

}

#endif /* _VRJ_KERNEL_KERNEL_EXCEPTIONS_H_ */
