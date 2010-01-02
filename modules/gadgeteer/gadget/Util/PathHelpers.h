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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_PATH_HELPERS_H_
#define _GADGET_PATH_HELPERS_H_

#include <gadget/gadgetConfig.h>

#include <boost/filesystem/path.hpp>


namespace gadget
{

/**
 * Retrieves the default directory under which the standard set of Gadgeteer
 * cluster and driver plug-ins will be installed. Calling code can then
 * append further structure to the returned path to distinguish between
 * specific types of plug-ins.
 *
 * @return A boost::filesystem::path providing the absolute path to the root
 *         directory where the base set of Gadgeteer plug-ins will be found.
 *
 * @since 1.1.11
 */
boost::filesystem::path getDefaultPluginRoot();

}

#endif /* _GADGET_PATH_HELPERS_H_ */
