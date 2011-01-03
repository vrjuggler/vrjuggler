/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2011 by Iowa State University
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

#ifndef _CLUSTER_USER_DATA_H
#define _CLUSTER_USER_DATA_H

#include <cluster/PluginConfig.h>

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/smart_ptr.hpp>

#include <plugins/ApplicationDataManager/ApplicationData.h>
#include <vpr/IO/SerializableObject.h>
#include <vpr/IO/ObjectReader.h>
#include <vpr/IO/ObjectWriter.h>


/**
 * @example "Example of using cluster::UserData<T>"
 *
 * If MyType all ready derives from vpr::SerializableObject, use the following:
 *
 * \code
 * cluster::UserData<MyType> mMyData;
 * \endcode
 *
 * This will be the usage when MyType is defined as part of the application.
 *
 * When it is necessary to add serialization capabilities to a third-party
 * type that does not derive from vpr::SerializableObject, the generic type
 * vpr::SerializableObjectMixin<T> must be used.  In this case, specializations
 * of the member functions readObject() and writeObject() must be written.
 *
 * For example, assume that MyType is defined in a third-party library as
 * follows:
 *
 * \code
 * struct MyType
 * {
 *    unsigned short something;
 *    bool drawBool;
 * };
 * \endcode
 *
 * Specialization of the methods of vpr::SerializableObjectMixin<T> would
 * then be written as follows:
 *
 * \code
 * template<>
 * void
 * vpr::SerializableObjectMixin<MyType>::writeObject(vpr::ObjectWriter* writer)
 * {
 *    writer->writeUint16(something);
 *    writer->writeBool(drawBool);
 * }
 *
 * template<>
 * void
 * vpr::SerializableObjectMixin<MyType>::readObject(vpr::ObjectReader* reader)
 * {
 *    something = reader->readUint16();
 *    drawBool  = reader->readBool();
 * }
 * \endcode
 *
 * Note that the method implementations have direct access to the data
 * members of MyType.  More generally, the implementations have access to
 * all public and protected members of the type used for specialization.
 *
 * The instantiation of cluster::UserData<T> would then appear as follows:
 *
 * \code
 * cluster::UserData<vpr::SerializableObjectMixin<MyType> > mMyData;
 * \endcode
 *
 * Access to the shared data cannot occur until after the method
 * cluster::UserData<T>::init() is invoked.  Any access attempts prior to
 * invoking this method will result in a NULL pointer dereference.
 *
 * For more detailed documentation, refer to the VR Juggler 2.0 Programmer's
 * Guide, found under http://www.vrjuggler.org/vrjuggler/docs.php
 *
 * For more information about object serialization and GUID objexts, refer to
 * the VR Juggler Portable Runtime Programmer's Guide and Reference Manual,
 * both of which are found under http://www.vrjuggler.org/vapor/docs.php
 */

namespace cluster
{

/**
 * Helper type used by cluster::UserData<T>.  This is the type of the object
 * held by an instance of cluster::UserData<T> (T == BASE).
 */
template<class BASE>
class AppDataMixin : public BASE, public ApplicationData
{
public:
   AppDataMixin()
   {;}

   ~AppDataMixin()
   {
      /* Do nothing. */ ;
   }

   /**
    * Invokes BASE::writeOjbect() to serialize this object into the given
    * object writer.
    *
    * @param writer The object writer to use for serializing this object.
    *
    * @throw vpr::IOException will be thrown if object serialization fails.
    */
   virtual void writeObject(vpr::ObjectWriter* writer)
   {
      BASE::writeObject(writer);
   }

   /**
    * Invokes BASE::readOjbect() to de-serialize this object from the data
    * held in the given object reader.
    *
    * @param reader The object reader to use for de-serializing this object.
    *
    * @throw vpr::IOException will be thrown if object de-serialization fails.
    */
   virtual void readObject(vpr::ObjectReader* reader)
   {
      BASE::readObject(reader);
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

/**
 * The object type for application-specific shared data.  By making use of
 * this type, applications can share arbitrary serializable data structures
 * among the nodes of o cluster.  It allows the user to have a copy constructor
 * and also not have to know the exact type.
 */
template <class TYPE>
class UserData
{
public:
   typedef TYPE Type;
   typedef boost::shared_ptr< AppDataMixin<TYPE> > AppDataPtr;

   /** Default constructor. */
   UserData()
   {;}

   /** Copy Constructor. */
   UserData(const UserData& userdata) : mAppData(userdata.mAppData)
   {;}

   UserData& operator=(const UserData& userdata)
   {
      if (this == &userdata)
      {
         return *this;
      }

      mAppData = AppDataPtr(userdata.mAppData);
      return *this;
   }

   /**
    * Initializes this application-specific shared data object.  This method
    * must be invoked before the contained shared data can be accessed via the
    * smart pointer semantics.
    *
    * @post The constained shared data object is ready to be accessed.
    *
    * @param id         Unique ID associated with this application-specific
    *                   shared data object.  This ID must not be used for any
    *                   other shared data instance.
    * @param writerAddr Network address (host name or IP address) of the
    *                   cluster node responsible for updating this
    *                   application-specific shared data object.  This
    *                   parameter is optional.  The data writer host can be
    *                   determined using a configuration element of type
    *                   application_data.
    */
   void init(const vpr::GUID& id)
   {
      mAppData = AppDataPtr(new AppDataMixin<TYPE>());
      mAppData->init(id);
   }

   virtual ~UserData()
   {;}

   /**
    * Returns whether this cluster node is responsible for updating
    * this application-specific shared data object.  This determination is
    * based on which node was identified as the writer when init() was
    * called.
    *
    * @pre init() has been invoked.
    *
    * @see init()
    */
   bool isLocal()
   {
      return(mAppData->isLocal());
   }

   /**
    * @name Smart pointer operator overloads.
    *
    * Application-specific shared data makes use of the Smart Pointer
    * design pattern.  Access to the contained data structure must occur
    * through one of these two operators.
    */
   //@{
   /**
    * Member selection (via pointer) operator overload.  Use it as follows:
    *
    * \code
    * struct MyType : vpr::SerializableObject
    * {
    *    void writeObject(vpr::ObjectWriter* writer);
    *    void readObject(vpr::ObjectWriter* reader);
    *    bool boolVal;
    * };
    *
    * cluster::UserData<MyType> cluster_data;
    * cluster_data->boolVal;
    * \endcode
    *
    * @pre init() has been invoked.
    *
    * @see init()
    */
   TYPE* operator->()
   {
      return mAppData.operator->();
   }

   /**
    * Dereference operator overload.  Use it as follows:
    *
    * \code
    * struct MyType : vpr::SerializableObject
    * {
    *    void writeObject(vpr::ObjectWriter* writer);
    *    void readObject(vpr::ObjectWriter* reader);
    *    bool boolVal;
    * };
    *
    * cluster::UserData<MyType> cluster_data;
    * (*cluster_data).boolVal;
    * \endcode
    *
    * @pre init() has been invoked.
    *
    * @see init()
    */
   TYPE& operator*()
   {
      return mAppData.operator*();
   }
   //@}

#ifndef _MSC_VER
private:
   // Make sure that we are wrapping a type that is derived from SerializableObject.
   BOOST_STATIC_ASSERT((::boost::is_base_and_derived<vpr::SerializableObject,TYPE>::value));
#endif

private:
   AppDataPtr mAppData;
};

} // end namespace cluster

#endif
