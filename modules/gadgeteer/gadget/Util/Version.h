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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_VERSION_H_
#define _GADGET_VERSION_H_

#include <gadget/gadgetConfig.h>

#include <string>
#include <vpr/vprTypes.h>


namespace gadget
{

/**
 * Retruns the "human-readable" Gadgeteer version \em string. The full form
 * includes the version number (<major>.<minor>.<patch>-<build>), the canonical
 * name of the release, the threading subsystem, and the build date. The major
 * version number is updated when major architectural changes are made; the
 * minor when there are minor API changes; and the patch when there are bug
 * fixes. The build number is usually representative of the nightly build
 * process relative to a given version x.y.z.
 *
 * @see gadget::GADGET_VERSION
 */
extern GADGET_API(std::string) getVersionString();

/**
 * Returns the 9-digit Gadgeteer version integer. This form provides three
 * digits for each of the major, minor, and patch numbers with no leading
 * zeros.
 *
 * @note The number returned is the value at run time. For a compile-time
 *       version identifier, use \c __GADGET_version. The value is the same as
 *       what is returned by this function.
 */
extern GADGET_API(vpr::Uint32) getVersionNumber();

}


#endif /* _GADGET_VERSION_H_ */
