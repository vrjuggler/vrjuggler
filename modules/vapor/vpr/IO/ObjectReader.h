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
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VPR_OBJECT_READER_H
#define _VPR_OBJECT_READER_H

#include <vpr/vprConfig.h>
#include <vpr/Util/AttributeMapBase.h>

#include <vector>

#include <boost/static_assert.hpp>

#include <vpr/Util/ReturnStatus.h>
#include <vpr/vprTypes.h>

namespace vpr
{

/** Interface used to read object data to a stream.
*
*/
class ObjectReader : public AttributeMapBase
{
public:
   ObjectReader()
   {;}

   virtual ~ObjectReader()
   {;}

   /** @name Tag and attribute handling */
   //@{
   /** Starts a new section/element of name tagName.
   */
   virtual vpr::ReturnStatus beginTag(std::string tagName) = 0;

   /** Ends the most recently named tag. */
   virtual vpr::ReturnStatus endTag() = 0;

   /** Starts an attribute of the name attributeName */
   virtual vpr::ReturnStatus beginAttribute(std::string attributeName) = 0;

   /** Ends the most recently named attribute */
   virtual vpr::ReturnStatus endAttribute() = 0;
   //@}

   virtual vpr::Uint8 readUint8() = 0;
   virtual vpr::Uint16 readUint16() = 0;
   virtual vpr::Uint32 readUint32() = 0;
   virtual vpr::Uint64 readUint64() = 0;
   virtual float readFloat() = 0;
   virtual double readDouble() = 0;
   virtual std::string readString() = 0;
   virtual bool readBool() = 0;

   /* Helper methods */
   virtual void readUint8(vpr::Uint8& val)
   { val = this->readUint8(); }
   virtual void readUint16(vpr::Uint16& val)
   { val = this->readUint16(); }
   virtual void readUint32(vpr::Uint32& val)
   { val = this->readUint32(); }
   virtual void readUint64(vpr::Uint64& val)
   { val = this->readUint64(); }
   virtual void readFloat(float& val)
   { val = this->readFloat(); }
   virtual void readDouble(double& val)
   { val = this->readDouble(); }
   virtual void readString(std::string& str)
   { str = this->readString(); }
   virtual void readBool(bool& val)
   { val = this->readBool(); }
};

} // namespace vpr

#endif
