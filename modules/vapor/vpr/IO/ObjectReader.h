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
 * VR Juggler is (C) Copyright 1998-2006 by Iowa State University
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

#ifndef _VPR_OBJECT_READER_H
#define _VPR_OBJECT_READER_H

#include <vpr/vprConfig.h>

#include <vpr/IO/IOException.h>
#include <vpr/Util/AttributeMapBase.h>
#include <vpr/vprTypes.h>


namespace vpr
{

/** \class ObjectReader ObjectReader.h vpr/IO/ObjectReader.h
 *
 * Interface used to read object data from a stream.
 */
class VPR_CLASS_API ObjectReader : public AttributeMapBase
{
protected:
   ObjectReader();

   ObjectReader(const ObjectReader& o);

public:
   virtual ~ObjectReader();

   /**
    * Returns true if this writer is using a binary-based format.
    * This can be used to choose whether to use human-readable forms of
    * serialization.
    */
   bool isBinary()
   {
      return mIsBinary;
   }

   /**
    * @name Tag and attribute handling.
    *
    * ObjectReader and ObjectWriter support an interface that allows for using
    * tags and attributes in the written output data.  This allows support
    * for formats such as XML where there is a logical grouping of the data.
    *
    * Tags are similar to the XML concept of elements.  They are used to
    * deliniate a hierarchical group in the structure of the data.  Attributes
    * are similar to XML attributes in that they are properties of the most
    * recently started tag.
    *
    * The structure looks something like like the following (based on XML):
    *
\verbatim
<tag1>
   <tag2 attrib1="XXX">
   </tag2>
</tag1>
\endverbatim
    */
   //@{
   /**
    * Starts a new section/element of name \p tagName.
    *
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void beginTag(const std::string& tagName) = 0;

   /**
    * Ends the most recently named tag.
    *
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void endTag() = 0;

   /**
    * Starts an attribute of the name \p attributeName.
    *
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void beginAttribute(const std::string& attributeName) = 0;

   /**
    * Ends the most recently named attribute.
    *
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void endAttribute() = 0;
   //@}

   /**
    * Resets the reading to the initial reading state.
    *
    * @post The reader can be reused and will function as if it were just
    *       initialized.
    */
   virtual void resetReading() = 0;

   /**
    * The following methods allow users to push/pop the active state of
    * reading.  This can be used to move back to previous reading states if
    * needed.
    */
   virtual void pushState() = 0;
   virtual void popState() = 0;

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual vpr::Uint8 readUint8() = 0;

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual vpr::Uint16 readUint16() = 0;

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual vpr::Uint32 readUint32() = 0;

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual vpr::Uint64 readUint64() = 0;

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual float readFloat() = 0;

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual double readDouble() = 0;

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual std::string readString() = 0;

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual bool readBool() = 0;

   /** @name Helper methods */
   //@{

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint8(vpr::Uint8& val)
   {
      val = this->readUint8();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint16(vpr::Uint16& val)
   {
      val = this->readUint16();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint32(vpr::Uint32& val)
   {
      val = this->readUint32();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint64(vpr::Uint64& val)
   {
      val = this->readUint64();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readFloat(float& val)
   {
      val = this->readFloat();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readDouble(double& val)
   {
      val = this->readDouble();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readString(std::string& str)
   {
      str = this->readString();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readBool(bool& val)
   {
      val = this->readBool();
   }
   //@}

protected:
   bool mIsBinary;   /**< Is this a binary serializer? */
};

} // namespace vpr

#endif
