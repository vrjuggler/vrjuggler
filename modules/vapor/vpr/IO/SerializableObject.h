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
 * VR Juggler is (C) Copyright 1998-2005 by Iowa State University
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

#ifndef _VPR_SERIALIZABLE_OBJ_H
#define _VPR_SERIALIZABLE_OBJ_H

#include <vpr/vprConfig.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

class ObjectWriter;
class ObjectReader;

/** \class WriteableObject SerializableObject.h vpr/IO/SerializableObject.h
 *
 * The abstract base class for all types that can be written to a stream.
 * Instances of this type can be serialized to a stream (also known as
 * marshalling).
 */
class WriteableObject
{
public:
   virtual ~WriteableObject()
   {;}

   /**
    * Template method for writing object this object to the given stream.
    *
    * @post All object data is written to the writer.
    *
    * @param writer The object writer to which the data for this object
    *               will be written to allow serialization of this object.
    */
   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer) = 0;

protected:
   WriteableObject()
   {;}

   WriteableObject(const WriteableObject&)
   {;}

private:
   /** @link dependency
    * @stereotype use*/
   /*#  ObjectWriter lnkObjectWriter; */
};

/** \class ReadableObject SerializableObject.h vpr/IO/SerializableObject.h
 *
 * The abstract base class for all types that can be read from a stream.
 * Instances of this type can be de-serialized from a stream (also known as
 * de-marshalling).
 */
class ReadableObject
{
public:
   virtual ~ReadableObject()
   {;}

   /**
    * Template method for reading data into this object from the given stream.
    *
    * @post All object data is read from the reader.
    *
    * @param reader The object reader from which the data for this object
    *               can be read to allow de-serialization of this object.
    */
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader) = 0;

protected:
   ReadableObject()
   {;}

   ReadableObject(const ReadableObject&)
   {;}

private:
   /** @link dependency
    * @stereotype use*/
   /*#  ObjectReader lnkObjectReader; */
};

/** \class SerializableObject SerializableObject.h vpr/IO/SerializableObject.h
 *
 * The abstract base class for all types that support serialization.
 * Instances of this type can be serialized to and de-serialized from data
 * streams.  This is based on a concept that is very similar to the notion
 * of serializable objects in the Java programming langauge (classes that
 * derive from \c java.io.Serializable).
 */
class SerializableObject : public WriteableObject, public ReadableObject
{
protected:

   SerializableObject()
      : WriteableObject(), ReadableObject()
   {;}

   SerializableObject(const SerializableObject& o)
      : WriteableObject(o), ReadableObject(o)
   {;}
};

/** \class SerializableObjectMixin SerializableObject.h vpr/IO/SerializableObject.h
 *
 * Mix-in type to add serialization capabilities to an existing type, usually
 * one that is defined in third-party code and therefore cannot have its base
 * type list modified.  Users can add serialization capabilities to an
 * existing type by defining just two methods.
 *
 * The following demonstrates the way in which this class is specialized for
 * an existing type (for example, \c MyType):
 *
 * \code
 * vpr::ReturnStatus
 * vpr::SerializableObjectMixin<MyType>::writeObject(vpr::ObjectWriter* writer)
 * { ... }
 *
 * vpr::ReturnStatus
 * vpr::SerializableObjectMixin<MyType>::readObject(vpr::ObjectReader* writer)
 * { ... }
 * \endcode
 *
 * A serializable instance of \c MyType can then be declared as follows:
 *
 * \code
 * vpr::SerializableObjectMixin<MyType> mSerializableMyType;
 * \endcode
 *
 * @see vpr::ObjectWriter, vpr::ObjectReader
 */
template<class BASE>
class SerializableObjectMixin : public SerializableObject, public BASE
{
public:
   virtual vpr::ReturnStatus writeObject(vpr::ObjectWriter* writer);
   virtual vpr::ReturnStatus readObject(vpr::ObjectReader* reader);
};

} // namespace vpr

#endif
