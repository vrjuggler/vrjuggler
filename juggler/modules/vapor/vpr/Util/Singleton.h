/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_SINGLETON_H_
#define _VPR_SINGLETON_H_

#include <vprConfig.h>
#include <Sync/Mutex.h>
#include <Sync/Guard.h>

// Double checked locking version

#define vprSingletonHeader( TYPE )                   \
public:                                              \
   static TYPE* instance( void )

#define vprSingletonHeaderWithInitFunc( TYPE, INIT_FUNC_NAME ) \
public:                                                        \
   static TYPE* instance( void )

#define vprSingletonImp( TYPE )                           \
   TYPE* TYPE::instance( void )                           \
   {                                                      \
      static vpr::Mutex singleton_lock1;                  \
      static TYPE* the_instance1 = NULL;                  \
                                                          \
      if (the_instance1 == NULL)                          \
      {                                                   \
         vpr::Guard<vpr::Mutex> guard( singleton_lock1 ); \
         if (the_instance1 == NULL)                       \
         { the_instance1 = new TYPE; }                    \
      }                                                   \
      return the_instance1;                               \
   }

#define vprSingletonImpWithInitFunc( TYPE, INIT_FUNC_NAME )   \
   TYPE* TYPE::instance( void )                               \
   {                                                          \
      static vpr::Mutex singleton_lock2;                      \
      static TYPE* the_instance2 = NULL;                      \
                                                              \
      if (the_instance2 == NULL)                              \
      {                                                       \
         vpr::Guard<vpr::Mutex> guard( singleton_lock2 );     \
         if (the_instance2 == NULL)                           \
         {                                                    \
            the_instance2 = new TYPE;                         \
            the_instance2->INIT_FUNC_NAME();                  \
         }                                                    \
      }                                                       \
      return the_instance2;                                   \
   }


//
// Non-locking version
//
/*
#define vprSingletonHeader( TYPE )                               \
public:                                                          \
   static TYPE* instance( void )                                 \
   {                                                             \
      static TYPE* the_instance = NULL;                          \
      if (the_instance == NULL)                                  \
      {                                                          \
         std::cout << "Creating instance of: TYPE" << std::endl; \
         the_instance = new TYPE;                                \
      }                                                          \
      return the_instance;                                       \
   }

#define vprSingletonHeaderWithInitFunc( TYPE, INIT_FUNC_NAME )   \
public:                                                          \
   static TYPE* instance( void )                                 \
   {                                                             \
      static TYPE* the_instance = NULL;                          \
      if (the_instance == NULL)                                  \
      {                                                          \
         std::cout << "Creating instance of: TYPE" << std::endl; \
         the_instance = new TYPE;                                \
         the_instance->INIT_FUNC_NAME();                         \
      }                                                          \
      return the_instance;                                       \
   }

#define vprSingletonImp( TYPE ) ;
*/

#endif
