/*******************************************************************************
 *
 * xdl - cross platform dynamic library loader
 * xdl is Copyright (c) 2002 Kevin Meinert.
 * this borrowed from the http://isugamedev.sf.net cvs repository...
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later
 * version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 *
 * -----------------------------------------------------------------------------
 * File:          $RCSfile$
 * Last modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------------------
 *
 ******************************************************************************/
#ifndef XDL_XDL_H
#define XDL_XDL_H

#include <iostream>  // for debug output..
#include <string>    // for std::string
#include <memory.h>  // for NULL

#ifdef WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#else
#ifdef __APPLE__

#else
#  include <dlfcn.h>   // for dlopen functions...
#endif
#endif

// for libraries: UNIX needs -ldl, Win32 needs Kernel32.lib

/**
 * Macros for declaring whether we are exporting or importing symbols from a
 * library when we compile. This is really only needed for Win32 libraries.
 */
#ifdef WIN32
#  define XDL_EXPORT __declspec( dllexport )
#  define XDL_IMPORT __declspec( dllimport )
#else
#  define XDL_EXPORT
#  define XDL_IMPORT
#endif

/**
 * Specify our calling convention for win32 DLLs.
 */
#ifdef WIN32
#  define XDL_CALL __stdcall
#else
#  define XDL_CALL
#endif

/**
 * Macros for defining in a cross platform way, symbols that may be exported
 * from a library, imported from a library or none of the above, depending on
 * the compile-time defines.
 *
 * The macros:
 *    - XDL_CLASS should be used when defining classes
 *    - XDL_STRUCT should be used when defining structs
 *    - XDL_FUNC should be used when defining functions
 *    - XDL_DATA should be used when defining static data
 *
 * Using these macros:
 * - define XDL_BUILD_DLL when you are building a new dll
 * - define XDL_USE_DLL when you are using a dll in your app
 * - don't define either and the export/import code will not be added in
 */
#ifdef XDL_BUILD_DLL
#  define XDL_CLASS     class    XDL_EXPORT
#  define XDL_STRUCT    struct   XDL_EXPORT
#  define XDL_FUNC               XDL_EXPORT
#  define XDL_DATA               XDL_EXPORT
#elif defined( XDL_USE_DLL )
#  define XDL_CLASS     class    XDL_IMPORT
#  define XDL_STRUCT    struct   XDL_IMPORT
#  define XDL_FUNC               XDL_IMPORT
#  define XDL_DATA               XDL_IMPORT
#else
#  define XDL_CLASS
#  define XDL_STRUCT
#  define XDL_FUNC
#  define XDL_DATA
#endif

/** XDL == cross (X) platform Dynamic Library */
namespace xdl
{
   enum Flag
   {
      LAZY, NOW
   };

   /**
    * The interface of xdl::Library. It works very much like Unix's dlopen.
    * Example usage:
    *
    * typedef int (*myfuncptr_t)( const char* );
    *
    * xdl::Library lib;
    * if (lib.open( "test.so" ))
    * {
    *    myfuncptr_t func = (myfuncptr_t)lib.lookup( "createInterface" );
    *    if (func != NULL)
    *    {
    *       int i = (*func)( "What is the meaning of life?" );
    *    }
    *    lib.close();
    * }
    */
   class ILibrary
   {
   public:
      virtual ~ILibrary() {}

      /**
       * Open the library contained in the given file.
       *
       * @param libraryFile   the path to the file containing the library
       * @param flag          specifies when to resolve undefined symbols in
       *                      in the library (now or as needed).
       *
       * @return  true if successful, false otherwise
       */
      virtual bool open( const char* libraryFile, const Flag& flag = NOW ) = 0;

      /**
       * Attempts to resolve the given symbol in the library and determine its
       * address in memory. Results are undefined if the library is not open.
       *
       * @param symbol        the symbol to resolve
       *
       * @return  a pointer to the symbol if successful, NULL otherwise
       */
      virtual void* lookup( const char* symbol ) = 0;

      /**
       * Close the library. Results are undefined if the library is not open.
       *
       * @return  true if successful, false otherwise
       */
      virtual bool close() = 0;
   };

#ifdef WIN32
   /**
    * Win32 implementation of xdl::ILibrary.
    */
   class LibraryWin32 : public ILibrary
   {
   public:
      LibraryWin32() : mLibHandle( NULL ) {}

      virtual ~LibraryWin32()
      {
         if ( mLibHandle != NULL )
            this->close();
      }

      /**
       * Open the library contained in the given file.
       *
       * @param libraryFile   the path to the file containing the library
       * @param flag          specifies when to resolve undefined symbols in
       *                      in the library (now or as needed).
       *
       * @return  true if successful, false otherwise
       */
      virtual bool open( const char* libraryFile, const Flag& flag = NOW )
      {
         DWORD f = 0;
         switch (flag)
         {
            case LAZY: f = DONT_RESOLVE_DLL_REFERENCES; break; // WinNT/2000/XP Only
            case NOW: break;
         }

         mLibHandle = ::LoadLibraryEx( libraryFile, NULL, f );
         if ( mLibHandle != NULL )
            return true;
         else
            return false;
      }

      /**
       * Attempts to resolve the given symbol in the library and determine its
       * address in memory. Results are undefined if the library is not open.
       *
       * @param symbol        the symbol to resolve
       *
       * @return  a pointer to the symbol if successful, NULL otherwise
       */
      virtual void* lookup( const char* symbol )
      {
         if ( mLibHandle == NULL ) return NULL;
         return (void*)::GetProcAddress( mLibHandle, symbol );
      }

      /**
       * Close the library. Results are undefined if the library is not open.
       *
       * @return  true if successful, false otherwise
       */
      virtual bool close()
      {
         HMODULE handle = mLibHandle;
         mLibHandle = NULL;
         if ( ::FreeLibrary( handle ) != 0 )
            return true;
         else
            return false;
      }

   private:
      HMODULE mLibHandle;
   };

   typedef LibraryWin32 Library; 

#else

#ifdef __APPLE__
    /**
    * OS X implementation of xdl::ILibrary
    * Stubbed out for now
    */
   class LibraryOSX : public ILibrary
   {
   public:
      LibraryOSX() : mLibHandle( NULL ) {}

      virtual ~LibraryOSX()
      {
         if ( mLibHandle != NULL )
            this->close();
      }

      /**
       * Open the library contained in the given file.
       *
       * @param libraryFile   the path to the file containing the library
       * @param flag          specifies when to resolve undefined symbols in
       *                      in the library (now or as needed).
       *
       * @return  true if successful, false otherwise
       */
      virtual bool open( const char *libraryFile, const Flag& flag = NOW )
      {
         return false; //Stub
         /*
         int f;
         switch (flag)
         {
            case LAZY: f = RTLD_LAZY; break;
            case NOW: f = RTLD_NOW; break;
         }
         mLibHandle = ::dlopen( libraryFile, f );
         if ( mLibHandle != NULL )
            return true;
         else
         {
            //std::cout<< "[xdl] " << ::dlerror()<<"\n"<<std::flush;
            return false;
         }
         */
      }

      /**
       * Attempts to resolve the given symbol in the library and determine its
       * address in memory. Results are undefined if the library is not open.
       *
       * @param symbol        the symbol to resolve
       *
       * @return  a pointer to the symbol if successful, NULL otherwise
       */
      virtual void* lookup( const char* symbol )
      {
         return NULL; //Stub
         /*
         if ( mLibHandle == NULL ) return NULL;
         return ::dlsym( mLibHandle, symbol );
         */
      }

      /**
       * Close the library. Results are undefined if the library is not open.
       *
       * @return  true if successful, false otherwise
       */
      virtual bool close()
      {
         return false;  //stub
         /*
         void* handle = mLibHandle;
         mLibHandle = NULL;
         if ( ::dlclose( handle ) == 0 )
            return true;
         else
            return false;
        */
      }

   private:
      void* mLibHandle;
   };

   typedef LibraryOSX Library;
    
    
    
    
#else

   /**
    * Unix implementation of xdl::ILibrary
    */
   class LibraryUnix : public ILibrary
   {
   public:
      LibraryUnix() : mLibHandle( NULL ) {}

      virtual ~LibraryUnix()
      {
         if ( mLibHandle != NULL )
            this->close();
      }

      /**
       * Open the library contained in the given file.
       *
       * @param libraryFile   the path to the file containing the library
       * @param flag          specifies when to resolve undefined symbols in
       *                      in the library (now or as needed).
       *
       * @return  true if successful, false otherwise
       */
      virtual bool open( const char *libraryFile, const Flag& flag = NOW )
      {
         int f;
         switch (flag)
         {
            case LAZY: f = RTLD_LAZY; break;
            case NOW: f = RTLD_NOW; break;
         }
         mLibHandle = ::dlopen( libraryFile, f );
         if ( mLibHandle != NULL )
            return true;
         else
         {
            //std::cout<< "[xdl] " << ::dlerror()<<"\n"<<std::flush;
            return false;
         }
      }

      /**
       * Attempts to resolve the given symbol in the library and determine its
       * address in memory. Results are undefined if the library is not open.
       *
       * @param symbol        the symbol to resolve
       *
       * @return  a pointer to the symbol if successful, NULL otherwise
       */
      virtual void* lookup( const char* symbol )
      {
         if ( mLibHandle == NULL ) return NULL;
         return ::dlsym( mLibHandle, symbol );
      }

      /**
       * Close the library. Results are undefined if the library is not open.
       *
       * @return  true if successful, false otherwise
       */
      virtual bool close()
      {
         void* handle = mLibHandle;
         mLibHandle = NULL;
         if ( ::dlclose( handle ) == 0 )
            return true;
         else
            return false;
      }

   private:
      void* mLibHandle;
   };

   typedef LibraryUnix Library;
#endif
#endif

} // namespace xdl

#endif
