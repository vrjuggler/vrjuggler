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

#ifndef _VRJ_GL_CONTEXT_DATA_H_
#define _VRJ_GL_CONTEXT_DATA_H_
//#pragma once

#include <vrj/Draw/OGL/Config.h>
#include <vector>
#include <vrj/Draw/OGL/GlDrawManager.h>
#include <vrj/Util/Debug.h>
#include <vpr/Thread/TSObjectProxy.h>

namespace vrj
{

/** \class GlContextDataBase GlContextData.h vrj/Draw/OGL/GlContextData.h
 *
 * This class is needed as a base class for GlContextData
 * because of dificulties making friends with a template.
 *
 * @note This class for internal use only.
 */
class GlContextDataBase
{
protected:
   int getCurContext()
   {
      return GlDrawManager::instance()->getCurrentContext();
   }
};


/** \class GlContextData GlContextData.h vrj/Draw/OGL/GlContextData.h
 *
 * OpenGL helper class that has templated user context-specific data.
 *
 * This class allows the user to specify a data type that contains
 * data that needs to have a context-specific copy.  This means that there
 * is a unique copy of the data structure for each OpenGL context in
 * the current environment.  Juggler will take care of the data copies
 * transparently for the user so the user never has to know about the
 * current configuration.
 *
 * An example use would be a struct full of display list id's.
 * The user passes their user-defined data structure as the template parameter.
 * The resulting object will be a "smart" pointer to the context-specific
 * data to use.
 *
 * @example "Example of using GL context-specific data"
 * \code
 * struct myStruct
 * {
 *    GLuint listId;
 * };
 *
 * void MyApp::contextInit()
 * {
 *    vrj::GlContextData<myStruct> myData;
 *    myData->listId = glGenList(1);
 *    // Compile display list...
 * }
 *
 * void MyApp::draw()
 * {
 *    glCallList(myData->listId);
 * }
 * \endcode
 *
 * @note Requires that the type of the context-specific data provide a default
 *       constructor used to initialize all of the copies of the data.
 */
template<class ContextDataType = int>
class GlContextData : private GlContextDataBase
{
public:
   GlContextData()
   {;}

   /**
    * Returns reference to user data for the current context.
    *
    * @pre We are in a draw process.
    * @note Should only be called from the draw function.
    *        Results are un-defined for other functions.
    */
   ContextDataType& operator*()
   {
      return (*getPtrToCur());
   }

   /**
    * Returns reference to user data for the current context.
    *
    * @pre We are in a draw process.
    * @note Should only be called from the draw function.
    *       Results are un-defined for other functions.
    */
   ContextDataType* operator->()
   {
      return getPtrToCur();
   }

   /**
    * This function gives exclusive access to ALL copies of the
    * context-specific data for contexts controlled by the current thread.
    *
    * @note THIS CAN NOT BE USED IN A DRAW PROCESS OR VERY BAD THINGS WILL
    *       HAPPEN.  Only for EXPERT use.
    */
   std::vector<ContextDataType*>* getDataVector()
   {
      return &(mThreadSpecificContextData->mContextDataVector);
   }

protected:
   /** Container for the thread-specific context-specific data. */
   template<class DATA_TYPE>
   struct ThreadContextData
   {
   public:
      typedef std::vector<DATA_TYPE*> data_list_t;

      ThreadContextData()
         : mContextDataVector()
      {;}

      ~ThreadContextData()
      {
         for (typename data_list_t::iterator itr = mContextDataVector.begin();
              mContextDataVector.end() != itr; itr++)
         {
            delete *itr;
            (*itr) = NULL;
         }
         mContextDataVector.clear();
      }

      /* Makes sure that the vector is at least requiredSize large */
      void checkSize(unsigned requiredSize)
      {
         if(requiredSize > mContextDataVector.size())
         {
            mContextDataVector.reserve(requiredSize);          // Resize smartly
            while(mContextDataVector.size() < requiredSize)    // Add any new items needed
            {
               mContextDataVector.push_back(new DATA_TYPE());
            }
         }
      }

      data_list_t mContextDataVector;   /**< Vector of user data */
   };

   /**
    * Returns a pointer to the correct data element in the current context.
    *
    * @pre We are in the draw function.
    * @post Synchronized.
    * @note ASSERT: Same context is rendered by same thread each time.
    */
   ContextDataType* getPtrToCur()
   {
      // Get current context
      int context_id = getCurContext();

      // Cache ref for better performance
      ThreadContextData<ContextDataType>* thread_specific_context_data =
         &(*mThreadSpecificContextData);

      thread_specific_context_data->checkSize(context_id+1);     // Make sure we are large enough (+1 since we have index)

      return thread_specific_context_data->mContextDataVector[context_id];
   }

private:
   vpr::TSObjectProxy<ThreadContextData<ContextDataType> >  mThreadSpecificContextData;
};

}

#endif
