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

#include <vrj/Draw/OGL/GlExtensionLoader.h>
#include <vpr/vprConfig.h>
#include <vpr/Util/Debug.h>

#include <GL/gl.h>

#if !defined(WIN32) && !defined(VPR_OS_Darwin)
#include <GL/glx.h>
#endif

#if defined(VPR_OS_Darwin)
#include <mach-o/dyld.h>
#elif !defined(WIN32)
#include <dlfcn.h>
#endif


namespace vrj
{

GlExtensionLoader::VoidExtFunc GlExtensionLoader::getFunctionByName(const char* name)
{
   VoidExtFunc found_func(NULL);

#if defined(VPR_OS_Darwin)
   if (NSIsSymbolNameDefined(name))
   {
      NSSymbol symbol = NSLookupAndBindSymbol(name);
      if (0 != symbol)
      {
         found_func = VoidExtFunc(NSAddressOfSymbol(symbol)); 
      }
   }
#elif defined(WIN32)

   found_func = (void(__cdecl*)(void)) wglGetProcAddress(name);

#elif defined(VPR_OS_IRIX) || defined(VPR_OS_HPUX) || \
   defined(VPR_OS_Linux) || defined(VPR_OS_FreeBSD) || defined(VPR_OS_Solaris)

   /* Workaround for multiple nVidia/Linux installation bugs, based on code in OpenSG */      
   static void (*(*__GetProcAddress)(const GLubyte *))(void) = NULL; 

   static void* lib_handle(NULL); 

   if (lib_handle == NULL)
   {
      lib_handle = dlopen(NULL, RTLD_NOW | RTLD_LOCAL); 

      if (!lib_handle)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "Error in dlopen:" << dlerror() << vprDEBUG_FLUSH;         
      }
   }

   if (__GetProcAddress == NULL)
   {
      __GetProcAddress = (void (*(*)(const GLubyte*))()) dlsym(lib_handle, "glXGetProcAddressARB"); 

      if (__GetProcAddress == NULL)
      {
         __GetProcAddress = (void (*(*)(const GLubyte*))()) dlsym(lib_handle, "glXGetProcAddress"); 

         if (__GetProcAddress == NULL)
         {
            vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) << "WARNING: Neither glXGetProcAddress nor "
                     "glXGetProcAddressARB found! Disabling all "
                     " extensions lookups.\n" << vprDEBUG_FLUSH;                   
         }
         else
         {
            vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Using glXGetProcAddress for GL extension handling.\n" << vprDEBUG_FLUSH;
         }
      }
      else
      {
         vprDEBUG(vprDBG_ALL, vprDBG_STATE_LVL) << "Using glXGetProcAddressARB for GL extension handling.\n" << vprDEBUG_FLUSH;
      }
   }

   if (__GetProcAddress != NULL)
   {
      found_func = reinterpret_cast<VoidExtFunc>(__GetProcAddress((const GLubyte*)name)); 
   }
   else
   {
      found_func = (VoidExtFunc)(dlsym(lib_handle, name)); 
   } 

#else
   vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL) 
      << "Warnining: Missing implementation for extension registration on this platform.\n"
      << "OpenGL extensions will not be available to VR Juggler.\n" << vprDEBUG_FLUSH;
#endif

   if (found_func == NULL)
   {
      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL) << "GlExtensionLoader:: Couldn't find function " << name << std::endl << vprDEBUG_FLUSH;          
   }
   else
   {
      vprDEBUG(vprDBG_ALL, vprDBG_HVERB_LVL) << "GlExtensionLoader:: Found function " << name << std::endl << vprDEBUG_FLUSH;          
   }

   return found_func;
}

} // namespace vrj

