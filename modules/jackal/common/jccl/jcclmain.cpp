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

#include <jccl/jcclConfig.h>

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

#if ! defined(WIN32) && ! defined(WIN64)
#  include <dlfcn.h>
#endif

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

#include <vpr/vpr.h>


namespace fs = boost::filesystem;

#if defined(WIN32) || defined(WIN64)
/**
 * Windows DLL entry point function. This ensures that the environment
 * variable \c JCCL_BASE_DIR is set as soon as this DLL is attached to the
 * process. If it is not set, then it sets it based on an assumption about the
 * structure of a JCCL installation. More specifically, an assumption is made
 * that this DLL lives in the \c lib subdirectory of the JCCL installation.
 * Therefore, the root of the JCCL installation is the parent of the directory
 * containing this DLL.
 */
BOOL __stdcall DllMain(HINSTANCE module, DWORD reason, LPVOID reserved)
{
   switch (reason)
   {
      case DLL_PROCESS_ATTACH:
         {
            try
            {
               char tmppath[1024];
               std::memset(tmppath, 0, sizeof(tmppath));
               GetModuleFileName(module, tmppath, sizeof(tmppath));

               const fs::path dll_path(tmppath, fs::native);
               fs::path base_dir = dll_path.branch_path().branch_path();
#if (defined(JUGGLER_DEBUG) || defined(JCCL_DEBUG)) && ! defined(_DEBUG)
               // The debug DLL linked against the release runtime is in
               // <base_dir>\lib\debug.
               base_dir = base_dir.branch_path();
#endif

               char* env_dir(NULL);
#if defined(_MSC_VER) && _MSC_VER >= 1400
               size_t len;
               _dupenv_s(&env_dir, &len, "JCCL_BASE_DIR");
#else
               env_dir = std::getenv("JCCL_BASE_DIR");
#endif

               if ( NULL == env_dir )
               {
                  const std::string base_dir_str =
                     base_dir.native_directory_string();

#if defined(_MSC_VER) && _MSC_VER >= 1400
                  _putenv_s("JCCL_BASE_DIR", base_dir_str.c_str());
#else
                  std::ostringstream env_stream;
                  env_stream << "JCCL_BASE_DIR=" << base_dir_str;
                  putenv(env_stream.str().c_str());
#endif
               }
#if defined(_MSC_VER) && _MSC_VER >= 1400
               else
               {
                  std::free(env_dir);
               }
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400
               _dupenv_s(&env_dir, &len, "JCCL_DATA_DIR");
#else
               env_dir = std::getenv("JCCL_DATA_DIR");
#endif

               if ( NULL == env_dir )
               {
                  fs::path data_dir(base_dir / "share" / "jccl");
                  const std::string data_dir_str =
                     data_dir.native_directory_string();

#if defined(_MSC_VER) && _MSC_VER >= 1400
                  _putenv_s("JCCL_DATA_DIR", data_dir_str.c_str());
#else
                  std::ostringstream env_stream;
                  env_stream << "JCCL_DATA_DIR=" << data_dir_str;
                  putenv(env_stream.str().c_str());
#endif
               }
#if defined(_MSC_VER) && _MSC_VER >= 1400
               else
               {
                  std::free(env_dir);
               }
#endif
            }
            catch (fs::filesystem_error& ex)
            {
               std::cerr << "Automatic assignment of JCCL environment "
                         << "variables failed:\n" << ex.what() << std::endl;
            }
         }
         break;
      default:
         break;
   }

   return TRUE;
}
#else
/**
 * Non-Windows shared library constructor. This ensures that the environment
 * variable \c JCCL_BASE_DIR is set as soon as this shared library is loaded.
 * If it is not set, then it sets it based on an assumption about the
 * structure of a JCCL installation. More specifically, an assumption is made
 * that this shared library lives in the \c lib subdirectory of the JCCL
 * installation. Therefore, the root of the JCCL installation is the parent of
 * the directory containing this shared library.
 */
extern "C" void __attribute ((constructor)) jcclLibraryInit()
{
   Dl_info info;
   info.dli_fname = 0;
   const int result = dladdr(reinterpret_cast<const void*>(&jcclLibraryInit),
                             &info);

   // NOTE: dladdr(3) really does return a non-zero value on success.
   if ( 0 != result )
   {
      try
      {
         fs::path lib_file(info.dli_fname, fs::native);
         lib_file = fs::system_complete(lib_file);

#if defined(VPR_OS_IRIX) && defined(_ABIN32)
         const std::string bit_suffix("32");
#elif defined(VPR_OS_IRIX) && defined(_ABI64) || \
      defined(VPR_OS_Linux) && defined(__x86_64__)
         const std::string bit_suffix("64");
#else
         const std::string bit_suffix("");
#endif

         // Get the directory containing this shared library.
         const fs::path lib_path = lib_file.branch_path();

         // Start the search for the root of the JCCL installation in the
         // parent of the directory containing this shared library.
         fs::path base_dir = lib_path.branch_path();

         // Use the lib subdirectory to figure out when we have found the root
         // of the JCCL installation tree.
         const fs::path lib_subdir(std::string("lib") + bit_suffix);

         bool found(false);
         while ( ! found )
         {
            try
            {
               if ( ! fs::exists(base_dir / lib_subdir) )
               {
                  base_dir = base_dir.branch_path();
               }
               else
               {
                  found = true;
               }
            }
            catch (fs::filesystem_error&)
            {
               base_dir = base_dir.branch_path();
            }
         }

         std::cout << base_dir.native_directory_string() << std::endl;
         const fs::path data_dir = base_dir / JCCL_SHARE_DIR;

         // We use the overwrite value of 0 as a way around testing whether
         // the environment variables are already set.
         setenv("JCCL_BASE_DIR", base_dir.native_directory_string().c_str(),
                0);
         setenv("JCCL_DATA_DIR", data_dir.native_directory_string().c_str(),
                0);
      }
      catch (fs::filesystem_error& ex)
      {
         std::cerr << "Automatic assignment of JCCL environment variables "
                   << "failed:\n" << ex.what() << std::endl;
      }
   }
}
#endif  /* defined(WIN32) || defined(WIN64) */
