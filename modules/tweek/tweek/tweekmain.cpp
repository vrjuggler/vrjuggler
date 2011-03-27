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

#include <tweek/tweekConfig.h>

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

#if ! defined(WIN32) && ! defined(WIN64)
#  include <dlfcn.h>
#endif

#include <boost/version.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

#include <vpr/vpr.h>


namespace fs = boost::filesystem;

#if defined(WIN32) || defined(WIN64)
/**
 * Windows DLL entry point function. This ensures that the environment
 * variable \c TWEEK_BASE_DIR is set as soon as this DLL is attached to the
 * process. If it is not set, then it sets it based on an assumption about the
 * structure of a Tweek installation. More specifically, an assumption is made
 * that this DLL lives in the \c lib subdirectory of the Tweek installation.
 * Therefore, the root of the Tweek installation is the parent of the
 * directory containing this DLL.
 */
BOOL __stdcall DllMain(HINSTANCE module, DWORD reason, LPVOID reserved)
{
   switch (reason)
   {
      case DLL_PROCESS_ATTACH:
         {
            char* env_dir(NULL);
#if defined(_MSC_VER) && _MSC_VER >= 1400
            size_t len;
            _dupenv_s(&env_dir, &len, "TWEEK_BASE_DIR");
#else
            env_dir = std::getenv("TWEEK_BASE_DIR");
#endif

            try
            {
               fs::path base_dir;

               // If TWEEK_BASE_DIR is not set, look up the path to this DLL
               // and use it to provide a default setting for that environment
               // variable.
               if ( NULL == env_dir )
               {
                  char tmppath[1024];
                  std::memset(tmppath, 0, sizeof(tmppath));
                  GetModuleFileName(module, tmppath, sizeof(tmppath));

                  const fs::path dll_path(
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
                     tmppath
#else
                     tmppath, fs::native
#endif
                  );
                  base_dir = dll_path.branch_path().branch_path();
#if (defined(JUGGLER_DEBUG) || defined(TWEEK_DEBUG)) && ! defined(_DEBUG)
                  // The debug DLL linked against the release runtime is in
                  // <base_dir>\lib\debug.
                  base_dir = base_dir.branch_path();
#endif

                  const std::string base_dir_str(
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
                     base_dir.string()
#else
                     base_dir.native_directory_string()
#endif
                  );

#if defined(_MSC_VER) && _MSC_VER >= 1400
                  _putenv_s("TWEEK_BASE_DIR", base_dir_str.c_str());
#else
                  std::ostringstream env_stream;
                  env_stream << "TWEEK_BASE_DIR=" << base_dir_str;
                  putenv(env_stream.str().c_str());
#endif
               }
               else
               {
                  base_dir =
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
                     fs::path(env_dir);
#else
                     fs::path(env_dir, fs::native);
#endif
#if defined(_MSC_VER) && _MSC_VER >= 1400
                  std::free(env_dir);
                  env_dir = NULL;
#endif
               }

#if defined(_MSC_VER) && _MSC_VER >= 1400
               _dupenv_s(&env_dir, &len, "TWEEK_DATA_DIR");
#else
               env_dir = std::getenv("TWEEK_DATA_DIR");
#endif

               // If TWEEK_BASE_DIR is not set, set a default relative to
               // base_dir.
               if ( NULL == env_dir )
               {
                  fs::path data_dir(base_dir / "share" / "tweek");
                  const std::string data_dir_str(
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
                     data_dir.string()
#else
                     data_dir.native_directory_string()
#endif
                  );

#if defined(_MSC_VER) && _MSC_VER >= 1400
                  _putenv_s("TWEEK_DATA_DIR", data_dir_str.c_str());
#else
                  std::ostringstream env_stream;
                  env_stream << "TWEEK_DATA_DIR=" << data_dir_str;
                  putenv(env_stream.str().c_str());
#endif
               }
#if defined(_MSC_VER) && _MSC_VER >= 1400
               else
               {
                  std::free(env_dir);
                  env_dir = NULL;
               }
#endif
            }
            catch (fs::filesystem_error& ex)
            {
               std::cerr << "Automatic assignment of Tweek environment "
                         << "variables failed:\n" << ex.what() << std::endl;

#if defined(_MSC_VER) && _MSC_VER >= 1400
               if ( NULL != env_dir )
               {
                  std::free(env_dir);
               }
#endif
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
 * variable \c TWEEK_BASE_DIR is set as soon as this shared library is loaded.
 * If it is not set, then it sets it based on an assumption about the
 * structure of a Tweek installation. More specifically, an assumption is made
 * that this shared library lives in the \c lib subdirectory of the Tweek
 * installation. Therefore, the root of the Tweek installation is the parent
 * of the directory containing this shared library.
 */
extern "C" void __attribute ((constructor)) tweekLibraryInit()
{
   fs::path base_dir;
   const char* env_dir = std::getenv("TWEEK_BASE_DIR");

   // If TWEEK_BASE_DIR is not set, look up the path to this shared library
   // and use it to provide a default setting for that environment variable.
   if ( NULL == env_dir )
   {
      Dl_info info;
      info.dli_fname = 0;
      const int result =
#if defined(__GNUC__) && __GNUC_MAJOR__ < 4
         dladdr((void*) &tweekLibraryInit, &info);
#else
         dladdr(reinterpret_cast<void*>(&tweekLibraryInit), &info);
#endif

      // NOTE: dladdr(3) really does return a non-zero value on success.
      if ( 0 != result )
      {
         try
         {
            fs::path lib_file(
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
               info.dli_fname
#else
               info.dli_fname, fs::native
#endif
            );
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

            // Start the search for the root of the Tweek installation in the
            // parent of the directory containing this shared library.
            base_dir = lib_path.branch_path();

            // Use the lib subdirectory to figure out when we have found the
            // root of the Tweek installation tree.
            const fs::path lib_subdir(std::string("lib") + bit_suffix);

            bool found(false);
            while ( ! found && ! base_dir.empty() )
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

            if ( found )
            {
               setenv("TWEEK_BASE_DIR",
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
                      base_dir.native().c_str(),
#else
                      base_dir.native_directory_string().c_str(),
#endif
                      1);
            }
         }
         catch (fs::filesystem_error& ex)
         {
            std::cerr << "Automatic assignment of TWEEK_BASE_DIR failed:\n"
                      << ex.what() << std::endl;
         }
      }
   }
   else
   {
      try
      {
         base_dir =
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
            fs::path(env_dir);
#else
            fs::path(env_dir, fs::native);
#endif
      }
      catch (fs::filesystem_error& ex)
      {
         std::cerr << "Invalid path set in TWEEK_BASE_DIR environment "
                   << "variable:\n" << ex.what() << std::endl;
      }
   }

   if ( ! base_dir.empty() )
   {
      // If base_dir were empty, this would result in data_dir being relative
      // to the current working directory.
      const fs::path data_dir = base_dir / TWEEK_SHARE_DIR;

      // We use the overwrite value of 0 as a way around testing whether the
      // environment variable is already set.
      setenv("TWEEK_DATA_DIR",
#if BOOST_VERSION >= 104600 && BOOST_FILESYSTEM_VERSION == 3
             data_dir.native().c_str(),
#else
             data_dir.native_directory_string().c_str(),
#endif
             0);
   }
}
#endif  /* defined(WIN32) || defined(WIN64) */
