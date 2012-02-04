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

#ifndef VPR_FILE_UTILS_H
#define VPR_FILE_UTILS_H

#include <vpr/vprConfig.h>
#include <string>



namespace vpr
{

/**
 * Replace environment variables in the string.
 *
 * @param str      The string in which values will be replaced.
 * @param startStr The string that marks the start of an environment variable.
 * @param endStr   The string that marks the end of an environment variable.
 *
 * @return The expanded version of \p str is returned.
 */
extern VPR_API const std::string replaceEnvVars(const std::string& str,
                                                const std::string& startStr,
                                                const std::string& endStr);

/**
 * Replace all environment variable strings embedded in \p str.
 *
 * Recognizes:
 *  ${VAR}
 *  $(VAR)
 *
 * @return The expanded version of \p str is returned.
 */
extern VPR_API const std::string replaceEnvVars(const std::string& str);

} // namespace vpr


#endif
