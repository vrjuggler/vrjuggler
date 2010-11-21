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

#include <vrj/Draw/OpenGL/Config.h>

#include <vpr/vpr.h>

#if defined(VPR_OS_Darwin) && defined(VRJ_USE_COCOA)
#  include <OpenGL/gl.h>
#else
#  include <GL/gl.h>
#endif

#if defined(VRJ_USE_X11)
#  include <GL/glx.h>
#endif

// On Mac OS X 10.3 and older, use the less portable symbol lookup mechanism.
#if defined(VPR_OS_Darwin) && VPR_OS_RELEASE_MAJOR < 8
#  include <mach-o/dyld.h>
#elif ! defined(VPR_OS_Windows)
#  include <dlfcn.h>
#endif

#include <vpr/Util/Debug.h>
#include <vrj/Draw/OpenGL/ExtensionLoader.h>


namespace vrj
{

namespace opengl
{

ExtensionLoader::VoidExtFunc
ExtensionLoader::getFunctionByName(const char* name)
{
   VoidExtFunc found_func(NULL);

   // On Mac OS X 10.3 and older, use the less portable symbol lookup
   // mechanism.
#if defined(VPR_OS_Darwin) && VPR_OS_RELEASE_MAJOR < 8
   if ( NSIsSymbolNameDefined(name) )
   {
      NSSymbol symbol = NSLookupAndBindSymbol(name);
      if (0 != symbol)
      {
         found_func = VoidExtFunc(NSAddressOfSymbol(symbol)); 
      }
   }
#elif defined(VPR_OS_Windows)

   found_func = (void(__cdecl*)(void)) wglGetProcAddress(name);

   // On UNIX variants including Mac OS X 10.4 and newer, use dlopen(3).
#elif defined(VPR_OS_IRIX) || defined(VPR_OS_HPUX) || \
      defined(VPR_OS_Linux) || defined(VPR_OS_FreeBSD) || \
      defined(VPR_OS_Solaris) || defined(VPR_OS_Darwin)

   // Workaround for multiple nVidia/Linux installation bugs, based on code in
   // OpenSG.
   static void (*(*__GetProcAddress)(const GLubyte *))(void) = NULL; 

   static void* lib_handle(NULL); 

   if (lib_handle == NULL)
   {
      lib_handle = dlopen(NULL, RTLD_NOW | RTLD_LOCAL); 

      if (!lib_handle)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << "Error in dlopen(3):" << dlerror() << vprDEBUG_FLUSH;         
      }
   }

   // We only have glX*() functions when we are using X11.
#if defined(VRJ_USE_X11)
   if (__GetProcAddress == NULL)
   {
      __GetProcAddress =
         (void (*(*)(const GLubyte*))()) dlsym(lib_handle,
                                               "glXGetProcAddressARB"); 

      if (__GetProcAddress == NULL)
      {
         __GetProcAddress =
            (void (*(*)(const GLubyte*))()) dlsym(lib_handle,
                                                  "glXGetProcAddress"); 

         if (__GetProcAddress == NULL)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
               << "WARNING: Neither glXGetProcAddress nor "
               << "glXGetProcAddressARB found! Disabling all extensions "
               << "lookups.\n" << vprDEBUG_FLUSH;                   
         }
         else
         {
            vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
               << "Using glXGetProcAddress for GL extension handling.\n"
               << vprDEBUG_FLUSH;
         }
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL)
            << "Using glXGetProcAddressARB for GL extension handling.\n"
            << vprDEBUG_FLUSH;
      }
   }
#endif

   if ( __GetProcAddress != NULL )
   {
      found_func =
         reinterpret_cast<VoidExtFunc>(__GetProcAddress((const GLubyte*) name));
   }
   else
   {
      found_func = (VoidExtFunc) dlsym(lib_handle, name); 
   } 

#else
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) 
      << "Warnining: Missing implementation for extension registration on this platform.\n"
      << "OpenGL extensions will not be available to VR Juggler.\n" << vprDEBUG_FLUSH;
#endif

   if ( found_func == NULL )
   {
      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "[vrj::opengl::ExtensionLoader::getFunctionByName()] Couldn't "
         << "find function '" << name << "'" << std::endl << vprDEBUG_FLUSH;
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL)
         << "[vrj::opengl::ExtensionLoader::getFunctionByName()] Found "
         << "function '" << name << "'" << std::endl << vprDEBUG_FLUSH;          
   }

   return found_func;
}

} // namespace opengl

} // namespace vrj
