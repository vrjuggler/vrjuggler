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

#include <gadget/gadgetConfig.h>

#include <string>

#include <vpr/vpr.h>
#include <vpr/System.h>

#include <gadget/Util/PathHelpers.h>


namespace fs = boost::filesystem;

namespace gadget
{

boost::filesystem::path getDefaultPluginRoot()
{
   // Append a default driver search path to search_path.
   const std::string gadget_base_dir("GADGET_BASE_DIR");
   const std::string vj_base_dir("VJ_BASE_DIR");
   std::string base_dir;

   // Try get to the value of $GADGET_BASE_DIR first.  If that fails,
   // fall back on $VJ_BASE_DIR.
   if ( ! vpr::System::getenv(gadget_base_dir, base_dir) )
   {
      if ( ! vpr::System::getenv(vj_base_dir, base_dir) )
      {
         // If neither $GADGET_BASE_DIR nor $VJ_BASE_DIR is set, then
         // we can append a default driver search path.
         base_dir = GADGET_ROOT_DIR;
      }
   }

#if defined(VPR_OS_IRIX) && defined(_ABIN32)
   const std::string bit_suffix("32");
#elif defined(VPR_OS_IRIX) && defined(_ABI64) || \
   defined(VPR_OS_Linux) && defined(__x86_64__)
   const std::string bit_suffix("64");
#else
   const std::string bit_suffix("");
#endif

   const std::string gadgeteer_subdir_base("gadgeteer");

   // If versioning is enabled, then the name of the directory containing the
   // Gadgeteer plug-ins must contain version information.
#if defined(GADGET_USE_VERSIONING)
   std::string gadgeteer_ver_str;
   const std::string gadget_version("GADGET_VERSION");

   // If $GADGET_VERSION is set, use the value of that environment variable
   // as the version component of the plug-in subdirectory name. Otherwise,
   // use the compile-time value provided by GADGET_VERSION_DOT.
   if ( ! vpr::System::getenv(gadget_version, gadgeteer_ver_str) )
   {
      gadgeteer_ver_str = GADGET_VERSION_DOT;
   }

   fs::path gadgeteer_subdir(gadgeteer_subdir_base + std::string("-") +
                             gadgeteer_ver_str);

   // If versioning is not enabled, then the directory containing the
   // Gadgeteer plug-ins will not incorporate version information.
#else
   fs::path gadgeteer_subdir(gadgeteer_subdir_base);
#endif

   fs::path default_plugin_dir =
      fs::path(base_dir, fs::native) / (std::string("lib") + bit_suffix) /
         gadgeteer_subdir;

   return default_plugin_dir;
}

}
