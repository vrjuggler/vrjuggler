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

#ifndef _VPR_OBJECT_WRITER_H
#define _VPR_OBJECT_WRITER_H

#include <vpr/vprConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/Util/AttributeMapBase.h>
#include <vpr/Util/ReturnStatus.h>


namespace vpr
{

/** \class ObjectWriter ObjectWriter.h vpr/IO/ObjectWriter.h
 *
 * Interface used to write object data to a stream.
 *
 * @todo Add smart buffering for type sizes.
 */
class ObjectWriter : public AttributeMapBase
{
protected:
   ObjectWriter();

   ObjectWriter(const ObjectWriter& o);

public:
   virtual ~ObjectWriter();

   /**
    * Returns true if the writer is using a binary based format.
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
    * tags and attributes in the written output data.  This allows support for
    * formats such as XML where there is a logical grouping of the data.
    *
    * Tags are similar to the XML concept of elements.  They are used to
    * deliniate a hierarchical group in the structure of the data.  Attributes
    * are similar to XML attributes in that they are properties of the most
    * recently started tag.
    *
    * The structure looks something like the following (based on XML):
    *
\verbatim
<tag1>
   <tag2 attrib1="XXX">
   </tag2>
</tag1>
\endverbatim
    */
   //@{
   /** Starts a new section/element of name tagName. */
   virtual vpr::ReturnStatus beginTag(const std::string& tagName) = 0;

   /** Ends the most recently named tag. */
   virtual vpr::ReturnStatus endTag() = 0;

   /** Starts an attribute of the name attributeName. */
   virtual vpr::ReturnStatus beginAttribute(const std::string& attributeName) = 0;

   /** Ends the most recently named attribute. */
   virtual vpr::ReturnStatus endAttribute() = 0;
   //@}

   virtual vpr::ReturnStatus writeUint8(vpr::Uint8 val) = 0;
   virtual vpr::ReturnStatus writeUint16(vpr::Uint16 val) = 0;
   virtual vpr::ReturnStatus writeUint32(vpr::Uint32 val) = 0;
   virtual vpr::ReturnStatus writeUint64(vpr::Uint64 val) = 0;
   virtual vpr::ReturnStatus writeFloat(float val) = 0;
   virtual vpr::ReturnStatus writeDouble(double val) = 0;
   virtual vpr::ReturnStatus writeString(std::string val) = 0;
   virtual vpr::ReturnStatus writeBool(bool val) = 0;

protected:
   bool mIsBinary;   /**< Is this a binary serializer? */
};

} // namespace vpr

#endif

