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

#ifndef _SINGLETON_TEMPLATE_H_
#define _SINGLETON_TEMPLATE_H_

#include <vjConfig.h>
#include <Sync/vjMutex.h>
#include <Sync/vjGuard.h>
#include <iostream>

//
// Double checked locking version
//

///*
#define vjSingletonHeader(TYPE) \
public:                                        \
   static TYPE* instance(void)                 \
   {                                           \
      if(_instance == NULL)                    \
      {                                        \
         vjGuard<vjMutex> guard(_inst_lock);   \
         if (_instance == NULL)                \
         { _instance = new TYPE; }             \
      }                                        \
      return _instance;                        \
   }                                           \
private:                                       \
   static vjMutex _inst_lock;                  \
   static TYPE* _instance

#define vjSingletonHeaderWithInitFunc(TYPE, INIT_FUNC_NAME) \
public:                                        \
   static TYPE* instance(void)                 \
   {                                           \
      if(_instance == NULL)                    \
      {                                        \
         vjGuard<vjMutex> guard(_inst_lock);   \
         if (_instance == NULL)                \
         {                                     \
            _instance = new TYPE;              \
            _instance->INIT_FUNC_NAME();       \
         }                                     \
      }                                        \
      return _instance;                        \
   }                                           \
private:                                       \
   static vjMutex _inst_lock;                  \
   static TYPE* _instance


#define vjSingletonImp(TYPE) \
            TYPE* TYPE::_instance = NULL; \
            vjMutex  TYPE::_inst_lock
//*/

//
// Non-locking version
//
/*
#define vjSingletonHeader(TYPE) \
public:                                        \
   static TYPE* instance(void)                 \
   {                                           \
      if (_instance == NULL)                   \
      {                                        \
         std::cout << "Creating instance of: TYPE" << std::endl; \
         _instance = new TYPE;                 \
      }                                        \
      return _instance;                        \
   }                                           \
private:                                       \
   static TYPE* _instance

#define vjSingletonHeaderWithInitFunc(TYPE, INIT_FUNC_NAME) \
public:                                        \
   static TYPE* instance(void)                 \
   {                                           \
      if (_instance == NULL)                   \
      {                                        \
         std::cout << "Creating instance of: TYPE" << std::endl; \
         _instance = new TYPE;                 \
         _instance->INIT_FUNC_NAME();          \
      }                                        \
      return _instance;                        \
   }                                           \
private:                                       \
   static TYPE* _instance

#define vjSingletonImp(TYPE) \
            TYPE* TYPE::_instance = NULL
*/

#endif
