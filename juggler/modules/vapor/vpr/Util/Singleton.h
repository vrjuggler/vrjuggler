/****************** <VPR heading BEGIN do not edit this line> *****************
 *
 * VR Juggler Portable Runtime
 *
 * Original Authors:
 *   Allen Bierbaum, Patrick Hartling, Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <VPR heading END do not edit this line> ******************/

/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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
#include <vpr/Sync/Mutex.h>
#include <vpr/Sync/Guard.h>

// Double checked locking version

#define vprSingletonHeader( TYPE )                   \
public:                                              \
   static TYPE* instance( void )

#define vprSingletonHeaderWithInitFunc( TYPE, INIT_FUNC_NAME ) \
public:                                                        \
   static TYPE* instance( void )

// NOTE: currently, func is thread safe after first call to instance().
// if first call to instance happens multiple times simultaneously
// then don't be surprised when something dies because of a mutex..
// this bug can be caused by spawning two threads immediately after 
// entering main()
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

namespace vpr
{
   
   /**
    * You can use this coolio class to make a singleton, just inherit like
    * so...
    *
    * class myClass : public vpr::Singleton<myClass>
    *
    * @author Kevin Meinert
    */
   template< class singleClass >
   class VPR_CLASS_API Singleton
   {
   public:
      /**
       * Use the macro from above... kludgy yeah, but...
       * NOTE: it might not be good to have the imp inline 
       *       in the header???<br>
       * NOTE: currently, func is thread safe after first call to instance().
       * if first call to instance happens multiple times simultaneously
       * then don't be surprised when something dies because of a mutex..
       * this bug can be caused by spawning two threads immediately after 
       * entering main()
       */
      inline static singleClass* instance( void )        
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
            vpr::Guard<vpr::Mutex> guard( singleton_lock1 );
            if (the_instance1 == NULL)        
            { the_instance1 = new singleClass; }         
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
}; // end of namespace vpr


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
