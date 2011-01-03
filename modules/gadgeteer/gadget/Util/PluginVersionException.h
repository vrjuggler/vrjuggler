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

#ifndef _GADGET_PLUGIN_VERSION_EXCEPTION_H_
#define _GADGET_PLUGIN_VERSION_EXCEPTION_H_

#include <gadget/gadgetConfig.h>

#include <vpr/Util/Exception.h>


namespace gadget
{

/**
 * Exception type thrown when a plug-in interface version check fails.
 *
 * @since 1.1.6
 */
class PluginVersionException : public vpr::Exception
{
public:
   PluginVersionException(const std::string& msg,
                          const std::string& location = "")
      throw ();

   virtual ~PluginVersionException() throw ();

   virtual std::string getExceptionName() const
   {
      return "gadget::PluginVersionException";
   }
};

}


#endif /* _GADGET_PLUGIN_VERSION_EXCEPTION_H_ */
