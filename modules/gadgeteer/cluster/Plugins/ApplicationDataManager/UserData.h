/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _CLUSTER_USER_DATA_H
#define _CLUSTER_USER_DATA_H

#include <cluster/Plugins/PluginConfig.h>

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

#include <cluster/Plugins/ApplicationDataManager/ApplicationData.h>
#include <cluster/SerializableData.h>
#include <vpr/IO/SerializableObject.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>
#include <vpr/Util/ReturnStatus.h>


namespace cluster
{
/* 
- If MyType all ready derives from vpr::SerializableObject you can use the following:

               cluster::UserData<MyType> mMyData;

- Otherwise you can use the SerializableObjectMixing and templete specializations to
  add the readObject() and writeObject() functions to use for your datatype.

         cluster::UserData<vpr::SerializableObjectMixin<MyType> > mMyData;
         
         template<class MyType> 
         vpr::ReturnStatus 
         vpr::SerializableObjectMixin<MyType>::writeObject(vpr::ObjectWriter* writer) 2 
         {
            writer->writeUint16(something); 
            writer->writeBool(drawBool); 
         }
          
         template<class MyType> 
         vpr::ReturnStatus
         vpr::SerializableObjectMixin<MyType>::readObject(vpr::ObjectReader* reader)3 
         { 
            something = reader->readUint16();
            drawBool = reader->readBool(); 
         }
- For more detailed documentation refer to the VR Juggler documentation at http://www.vrjuggler.org/
*/
template<class BASE>
class AppDataMixin : public BASE, public ApplicationData
{
public:
   AppDataMixin(const vpr::GUID& id, const std::string& host_name) : ApplicationData(id, host_name)
   {;}
   
   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer)
   {
      return BASE::writeObject(writer);
   }
   
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader)
   {
      return BASE::readObject(reader);
   }
      
#ifndef _MSC_VER
private:
   // Make sure that we are not already deriving from ApplicationData
   BOOST_STATIC_ASSERT(::boost::type_traits::ice_not< 
                        (::boost::is_base_and_derived<cluster::ApplicationData,BASE>::value) 
                        >::value);
   
   // Make sure that we are deriving from a SerializableObject class
   BOOST_STATIC_ASSERT((::boost::is_base_and_derived<vpr::SerializableObject,BASE>::value));
#endif
};

/* Allows the user to have a copy constructor
 * and also not have to know the exact type
 */
template <class TYPE>
class UserData
{
public:
   typedef TYPE Type;
   
   UserData()
   {
      mAppData = NULL;
   }
   
   /** Copy Constructor */
   UserData(UserData& userdata) : mAppData(userdata.mAppData)
   {
//      *this = userdata;
   }
   
   bool operator== (const UserData& userdata) const
   {
      // Make sure user_data is valid also
      if (NULL == mAppData)
      {
         return false;
      }  // Not valid
      return (mAppData == userdata.mAppData);
   }
   
   bool operator!= (const UserData& userdata) const
   {
      return !(*this == userdata);
   }

   UserData& operator=(const UserData& userdata)
   {
      //if (! (this == &userdata) )    // Different objects
      //{
         mAppData = userdata.mAppData;
      //}
   
      return *this;
   }


   void init(vpr::GUID id, std::string host_name = std::string(""))
   {
      mAppData = new AppDataMixin<TYPE>(id, host_name);
   }
   
   virtual ~UserData()
   {
      delete mAppData;
   }
   
   bool isLocal()
   {
      return(mAppData->isLocal());
   }
   
   TYPE* operator->()
   { return mAppData; }
   
   TYPE& operator*()
   { return *(mAppData); }
#ifndef _MSC_VER
private:      
   // Make sure that we are wrapping a type that is derived from ApplicationData
   BOOST_STATIC_ASSERT((::boost::is_base_and_derived<vpr::SerializableObject,TYPE>::value));
#endif
private:
   cluster::AppDataMixin<TYPE>*    mAppData;
};

   
} // end namespace gadget

#endif
