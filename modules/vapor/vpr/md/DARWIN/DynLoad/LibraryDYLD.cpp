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
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

/*
Copyright (c) 2002 Peter O'Gorman <ogorman@users.sourceforge.net>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <vpr/vprConfig.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <limits.h>
#include <mach-o/dyld.h>
#include <boost/concept_check.hpp>

#include <vpr/DynLoad/LibraryException.h>
#include <vpr/IO/IOException.h>
#include <vpr/Util/Assert.h>
#include <vpr/Util/Debug.h>
#include <vpr/md/DARWIN/DynLoad/LibraryDYLD.h>


#ifndef _POSIX_SOURCE
/*
 * Structure filled in by dladdr().
 */
typedef struct dl_info {
        const char      *dli_fname;     /* Pathname of shared object */
        void            *dli_fbase;     /* Base address of shared object */
        const char      *dli_sname;     /* Name of nearest symbol */
        void            *dli_saddr;     /* Address of nearest symbol */
} Dl_info;

#endif /* ! _POSIX_SOURCE */

#define RTLD_LAZY   0x1
#define RTLD_NOW    0x2
#define RTLD_LOCAL  0x4
#define RTLD_GLOBAL 0x8

#ifndef _POSIX_SOURCE
#define RTLD_NOLOAD   0x10
#define RTLD_NODELETE 0x80

/*
 * Special handle arguments for dlsym().
 */
#define RTLD_NEXT    ((void *) -1)     /* Search subsequent objects. */
#define RTLD_DEFAULT ((void *) -2)     /* Use default search algorithm. */
#endif /* ! _POSIX_SOURCE */

namespace
{

static const unsigned int ERR_STR_LEN(256);

// Copied from error() found in dlfcn_simple.c from dlcompat.
static const char* error(int setget, const char *str, ...)
{
   static char errstr[ERR_STR_LEN];
   static int err_filled = 0;
   const char *retval;
   NSLinkEditErrors ler;
   int lerno;
   const char *dylderrstr;
   const char *file;
   va_list arg;
   if (setget <= 0)
   {
      va_start(arg, str);
      strncpy(errstr, "dlsimple: ", ERR_STR_LEN);
      vsnprintf(errstr + 10, ERR_STR_LEN - 10, str, arg);
      va_end(arg);
   /* We prefer to use the dyld error string if setget is 0 */
      if (setget == 0) {
         NSLinkEditError(&ler, &lerno, &file, &dylderrstr);
         fprintf(stderr,"dyld: %s\n",dylderrstr);
         if (dylderrstr && strlen(dylderrstr))
            strncpy(errstr,dylderrstr,ERR_STR_LEN);
      }      
      err_filled = 1;
      retval = NULL;
   }
   else
   {
      if (!err_filled)
         retval = NULL;
      else
         retval = errstr;
      err_filled = 0;
   }
   return retval;
}

}

namespace vpr
{

LibraryDYLD::~LibraryDYLD()
{
   if ( NULL != mLibrary )
   {
      try
      {
         unload();
      }
      catch (vpr::IOException& ex)
      {
         vprDEBUG(vprDBG_ALL, vprDBG_WARNING_LVL)
            << clrOutBOLD(clrYELLOW, "WARNING")
            << ": Failed to unload library during destruction" << std::endl
            << ex.what() << std::endl << vprDEBUG_FLUSH;
      }
   }
}

void LibraryDYLD::load()
{
   if ( ! mName.empty() )
   {
      mLibrary = internalDlopen(mName.c_str(), RTLD_NOW | RTLD_GLOBAL);
   }
   else
   {
      mLibrary = internalDlopen(NULL, RTLD_NOW | RTLD_GLOBAL);
   }

   if ( NULL == mLibrary )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to load '" << mName << "': " << ();
      throw vpr::IOException(msg_stream.str(), VPR_LOCATION);
   }
}

void LibraryDYLD::unload()
{
   if ( NULL == mLibrary )
   {
      throw vpr::IOException("No library to unload", VPR_LOCATION);
   }

   if ( internalDlclose(mLibrary) != 0 )
   {
      std::ostringstream msg_stream;
      msg_stream << "Failed to unload '" << mName << "': "
                 << internalDlerror();
      throw vpr::IOException(msg_stream.str(), VPR_LOCATION);
   }
   else
   {
      mLibrary = NULL;
   }
}

void* LibraryDYLD::findSymbol(const std::string& symbolName)
{
   // If the library has not been loaded yet, throw an exception.
   if ( NULL == mLibrary )
   {
      std::ostringstream msg_stream;
      msg_stream << "Library not loaded: '" << mName << "'";
      throw vpr::LibraryException(msg_stream.str(), VPR_LOCATION);
   }

   return internalDlsym(mLibrary, symbolName.c_str());
}

void* LibraryDYLD::findSymbolAndLibrary(const std::string& symbolName,
                                        LibraryDYLD& lib)
{
   boost::ignore_unused_variable_warning(symbolName);
   boost::ignore_unused_variable_warning(lib);
   vprASSERT(false && "Not implemented yet");
   return NULL;
}

// Copied from dlopen() found in dlfcn_simple.c from dlcompat.
void* LibraryDYLD::internalDlopen(const char* path, int mode)
{
   void *module = 0;
   NSObjectFileImage ofi = 0;
   NSObjectFileImageReturnCode ofirc;
   static int (*make_private_module_public) (NSModule module) = 0;
   unsigned int flags =  NSLINKMODULE_OPTION_RETURN_ON_ERROR | NSLINKMODULE_OPTION_PRIVATE;

   /* If we got no path, the app wants the global namespace, use -1 as the marker
      in this case */
   if (!path)
      return (void *)-1;

   /* Create the object file image, works for things linked with the -bundle arg to ld */
   ofirc = NSCreateObjectFileImageFromFile(path, &ofi);
   switch (ofirc)
   {
      case NSObjectFileImageSuccess:
         /* It was okay, so use NSLinkModule to link in the image */
         if (!(mode & RTLD_LAZY)) flags += NSLINKMODULE_OPTION_BINDNOW;
         module = NSLinkModule(ofi, path,flags);
         /* Don't forget to destroy the object file image, unless you like leaks */
         NSDestroyObjectFileImage(ofi);
         /* If the mode was global, then change the module, this avoids
            multiply defined symbol errors to first load private then make
            global. Silly, isn't it. */
         if ((mode & RTLD_GLOBAL))
         {
           if (!make_private_module_public)
           {
             _dyld_func_lookup("__dyld_NSMakePrivateModulePublic", 
            (unsigned long *)&make_private_module_public);
           }
           make_private_module_public(module);
         }
         break;
      case NSObjectFileImageInappropriateFile:
         /* It may have been a dynamic library rather than a bundle, try to load it */
         module = (void *)NSAddImage(path, NSADDIMAGE_OPTION_RETURN_ON_ERROR);
         break;
      case NSObjectFileImageFailure:
         error(0,"Object file setup failure :  \"%s\"", path);
         return 0;
      case NSObjectFileImageArch:
         error(0,"No object for this architecture :  \"%s\"", path);
         return 0;
      case NSObjectFileImageFormat:
         error(0,"Bad object file format :  \"%s\"", path);
         return 0;
      case NSObjectFileImageAccess:
         error(0,"Can't read object file :  \"%s\"", path);
         return 0;      
   }
   if (!module)
      error(0, "Can not open \"%s\"", path);
   return module;
}

// Copied from dlerror() found in dlfcn_simple.c from dlcompat.
const char* LibraryDYLD::internalDlerror()
{
   return error(1, (char *)NULL);
}

// Copied from dlclose() found in dlfcn_simple.c from dlcompat.
int LibraryDYLD::internalDlclose(void* handle)
{
   if ((((struct mach_header *)handle)->magic == MH_MAGIC) ||
      (((struct mach_header *)handle)->magic == MH_CIGAM))
   {
      error(-1, "Can't remove dynamic libraries on darwin");
      return 0;
   }
   if (!NSUnLinkModule(handle, 0))
   {
      error(0, "unable to unlink module %s", NSNameOfModule(handle));
      return 1;
   }
   return 0;
}

// Copied from dlsym found in dlfcn_simple.c from dlcompat.
void* LibraryDYLD::internalDlsym(void* handle, const char* symbol)
{
   int sym_len = strlen(symbol);
   void *value = NULL;
   char *malloc_sym = NULL;
   NSSymbol *nssym = 0;
   malloc_sym = malloc(sym_len + 2);
   if (malloc_sym)
   {
      sprintf(malloc_sym, "_%s", symbol);
      /* If the handle is -1, if is the app global context */
      if (handle == (void *)-1)
      {
         /* Global context, use NSLookupAndBindSymbol */
         if (NSIsSymbolNameDefined(malloc_sym))
         {
            nssym = NSLookupAndBindSymbol(malloc_sym);
         }
      }
      /* Now see if the handle is a struch mach_header* or not, use NSLookupSymbol in image
         for libraries, and NSLookupSymbolInModule for bundles */
      else
      {
         /* Check for both possible magic numbers depending on x86/ppc byte order */
         if ((((struct mach_header *)handle)->magic == MH_MAGIC) ||
            (((struct mach_header *)handle)->magic == MH_CIGAM))
         {
            if (NSIsSymbolNameDefinedInImage((struct mach_header *)handle, malloc_sym))
            {
               nssym = NSLookupSymbolInImage((struct mach_header *)handle,
                                      malloc_sym,
                                      NSLOOKUPSYMBOLINIMAGE_OPTION_BIND
                                      | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR);
            }
   
         }
         else
         {
            nssym = NSLookupSymbolInModule(handle, malloc_sym);
         }
      }
      if (!nssym)
      {
         error(0, "Symbol \"%s\" Not found", symbol);
      }
      value = NSAddressOfSymbol(nssym);
      free(malloc_sym);
   }
   else
   {
      error(-1, "Unable to allocate memory");
   }
   return value;
}

} // End of vpr namespace
