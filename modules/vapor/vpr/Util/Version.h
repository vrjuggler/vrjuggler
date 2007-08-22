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

#ifndef _VPR_VERSION_H_
#define _VPR_VERSION_H_

#include <vpr/vprConfig.h>

#include <string>

#include <vpr/vprTypes.h>


namespace vpr
{

/**
 * This is the "human-readable" VPR version \em string. The full form includes
 * the version number (major.minor.patch-build), the canonical name of the
 * release, the threading subsystem, and the build date. The major version
 * number is updated when major architectural changes are made; the minor
 * when there are minor API changes; and the patch when there are bug fixes.
 * The build number is usually representative of the nightly build process
 * relative to a given version x.y.z.
 *
 * @see vpr::getVersionString()
 */
extern VPR_DATA_API(const char*) VPR_VERSION;

/**
 * Retruns the "human-readable" VPR version \em string. The full form includes
 * the version number (major.minor.patch-build), the canonical name of the
 * release, the threading subsystem, and the build date. The major version
 * number is updated when major architectural changes are made; the minor when
 * there are minor API changes; and the patch when there are bug fixes. The
 * build number is usually representative of the nightly build process
 * relative to a given version x.y.z.
 *
 * @see vpr::VPR_VERSION
 */
extern VPR_API(std::string) getVersionString();

/**
 * Returns the 9-digit VPR version integer. This form provides three digits
 * for each of the major, minor, and patch numbers with no leading zeros.
 *
 * @note The number returned is the value at run time. For a compile-time
 *       version identifier, use \c __VPR_version. The value is the same as
 *       what is returned by this function.
 */
extern VPR_API(vpr::Uint32) getVersionNumber();

/**
 * Returns the name of the threading subsystem used to compile VPR. It will be
 * one of \c "POSIX" or \c "NSPR". This identifier is part of the full version
 * string returned by vpr::getVersionString().
 *
 * @see vpr::getVersionString()
 */
extern VPR_API(std::string) getSubsystem();

}


#endif /* _VPR_VERSION_H_ */
