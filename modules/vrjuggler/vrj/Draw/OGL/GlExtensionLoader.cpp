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

