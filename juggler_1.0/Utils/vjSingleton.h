#ifndef _SINGLETON_TEMPLATE_H_
#define _SINGLETON_TEMPLATE_H_

#include <vjConfig.h>
#include <Sync/vjMutex.h>
#include <Sync/vjGuard.h>
#include <iostream>

//
// Double checked locking version
//

/*
#define vjSingletonHeader(TYPE) \
public:                                        \
   static TYPE* instance(void)                 \
   {                                           \
      if(_instance == NULL)                    \
      {                                        \
         vjGuard<vjMutex> guard(_inst_lock);   \
         if (_instance == NULL)                \
            _instance = new TYPE;              \
      }                                        \
      return _instance;                        \
   }                                           \
private:                                       \
   static vjMutex _inst_lock;                  \
   static TYPE* _instance;

#define vjSingletonImp(TYPE) \
            TYPE* TYPE::_instance = NULL; \
            vjMutex  TYPE::_inst_lock;
*/

//
// Non-locking version
//

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
   static TYPE* _instance;

#define vjSingletonImp(TYPE) \
            TYPE* TYPE::_instance = NULL;

#endif
