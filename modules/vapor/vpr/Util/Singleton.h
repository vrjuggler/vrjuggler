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

#ifndef _VPR_SINGLETON_H_
#define _VPR_SINGLETON_H_

#include <vpr/vprConfig.h>

#include <stdlib.h>

#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>

#include <vpr/Util/detail/LifetimeTracker.h>

// Double checked locking version

#define vprSingletonHeader( TYPE )                   \
public:                                              \
   static TYPE* instance();                          \
private:                                             \
   friend struct vpr::detail::Deleter<TYPE>;         \
   static volatile TYPE* sInstance

#define vprSingletonHeaderWithInitFunc( TYPE, INIT_FUNC_NAME ) \
public:                                                        \
   static TYPE* instance();                                    \
private:                                                       \
   friend struct vpr::detail::Deleter<TYPE>;                   \
   static volatile TYPE* sInstance

#define vprSingletonImp(TYPE) vprSingletonImpLifetime(TYPE, 1u)

// NOTE: currently, func is thread safe after first call to instance().
// if first call to instance happens multiple times simultaneously
// then don't be surprised when something dies because of a mutex..
// this bug can be caused by spawning two threads immediately after
// entering main()
#define vprSingletonImpLifetime(TYPE, LONGEVITY)                        \
   TYPE* TYPE::instance()                                               \
   {                                                                    \
      static vpr::Mutex singleton_lock1;                                \
                                                                        \
      if ( sInstance == NULL )                                          \
      {                                                                 \
         vpr::Guard<vpr::Mutex> guard(singleton_lock1);                 \
         if ( sInstance == NULL )                                       \
         {                                                              \
            sInstance = new TYPE;                                       \
            vpr::SetLongevity((TYPE*) sInstance,                        \
                              (unsigned int) LONGEVITY,                 \
                              vpr::detail::Deleter<TYPE>::Delete);      \
         }                                                              \
      }                                                                 \
      return (TYPE*) sInstance;                                         \
   }                                                                    \
                                                                        \
   volatile TYPE* TYPE::sInstance = NULL;

#define vprSingletonImpWithInitFunc(TYPE, INIT_FUNC_NAME)       \
   vprSingletonImpLifetimeWithInitFunc(TYPE, INIT_FUNC_NAME, 1u)

#define vprSingletonImpLifetimeWithInitFunc(TYPE, INIT_FUNC_NAME, LONGEVITY) \
   TYPE* TYPE::instance()                                               \
   {                                                                    \
      static vpr::Mutex singleton_lock2;                                \
                                                                        \
      if ( sInstance == NULL )                                          \
      {                                                                 \
         vpr::Guard<vpr::Mutex> guard(singleton_lock2);                 \
         if ( sInstance == NULL )                                       \
         {                                                              \
            sInstance = new TYPE;                                       \
            ((TYPE*) sInstance)->INIT_FUNC_NAME();                      \
            vpr::SetLongevity((TYPE*) sInstance,                        \
                              (unsigned int) LONGEVITY,                 \
                              vpr::detail::Deleter<TYPE>::Delete);      \
         }                                                              \
      }                                                                 \
      return (TYPE*) sInstance;                                         \
   }                                                                    \
                                                                        \
   volatile TYPE* TYPE::sInstance = NULL;

namespace vpr
{

   /** \class Singleton Singleton.h vpr/Util/Singleton.h
    *
    * You can use this coolio class to make a singleton, just inherit like
    * so:
    *
    * \code
    * class myClass : public vpr::Singleton<myClass>
    * \endcode
    *
    * @param singleClass The type to be used a singleton.
    *
    * @note This class is currently broken under certain circumstances.
    *       Refer to SourceForge bug #'s 729485 and 929003.  Until these bugs
    *       are fixed, use of the preprocessor macros vprSingletonHeader()
    *       et. al. are recommended.
    */
   template<class singleClass>
   class Singleton
   {
   public:
      /**
       * @note Currently, this function is thread safe after first call to
       *       instance().  if first call to instance happens multiple times
       *       simultaneously then don't be surprised when something dies
       *       because of a mutex.  This bug can be caused by spawning two
       *       threads immediately after entering main().
       */
      static singleClass* instance()
      {
         // WARNING! race condition possibility, creation of static vars
         // are not thread safe.  This is only an issue when creating
         // your first thread, since it uses a singleton thread manager,
         // the two threads might both try to call instance at the same time
         // which then the creation of the following mutex would not be certain.
         static vpr::Mutex singleton_lock1;
         static singleClass* the_instance1 = NULL;

         if (the_instance1 == NULL)
         {
            vpr::Guard<vpr::Mutex> guard(singleton_lock1);
            if (the_instance1 == NULL)
            {
               the_instance1 = new singleClass;
            }
         }
         return the_instance1;
      }

   protected:
      /**
       * Don't create a singleton with new!
       * use instance()
       */
      Singleton()
      {
      }

      /// don't delete a singleton!
      virtual ~Singleton()
      {
      }
   };
} // end of namespace vpr


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
