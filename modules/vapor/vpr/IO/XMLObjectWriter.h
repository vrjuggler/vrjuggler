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

#ifndef _VPR_XML_OBJECT_WRITER_H
#define _VPR_XML_OBJECT_WRITER_H

#include <vpr/vprConfig.h>

#include <vector>
#include <string>
#include <cppdom/cppdom.h>

#include <vpr/IO/ObjectWriter.h>


namespace vpr
{

/** \class XMLObjectWriter XMLObjectWriter.h vpr/IO/XMLObjectWriter.h
 *
 * Object writer for data buffers.  Writes directly to a data buffer.
 *
 * @todo Add smart buffering for type sizes
 */
class VPR_CLASS_API XMLObjectWriter : public ObjectWriter
{
public:
   XMLObjectWriter()
      : mCurNode(NULL), mCurTarget(CdataTarget)
   {;}

   XMLObjectWriter(cppdom::NodePtr rootNode, cppdom::Node* curNode)
      : mCurTarget(CdataTarget)
   {
      mRootNode = rootNode;
      mCurNode = curNode;
   }

   /**
    * Gets the data buffer.
    * Returns the data buffer representation of the current object tree.
    */
   std::vector<vpr::Uint8> getData();

   cppdom::NodePtr getRootNode()
   {
      return mRootNode;
   }

   /** @name Tag and attribute handling */
   //@{
   /** Starts a new section/element of name \p tagName. */
   virtual void beginTag(const std::string& tagName);

   /** Ends the most recently named tag. */
   virtual void endTag();

   /** Starts an attribute of the name \p attributeName. */
   virtual void beginAttribute(const std::string& attributeName);

   /** Ends the most recently named attribute. */
   virtual void endAttribute();
   //@}

   /**
    * Writes out the single byte.
    * @post data = old(data)+val, \c mCurHeadPos advaced 1.
    */
   virtual void writeUint8(vpr::Uint8 val);
   virtual void writeUint16(vpr::Uint16 val);
   virtual void writeUint32(vpr::Uint32 val);
   virtual void writeUint64(vpr::Uint64 val);
   virtual void writeFloat(float val);
   virtual void writeDouble(double val);
   virtual void writeString(std::string val);
   virtual void writeBool(bool val);

protected:
   enum CurTarget
   {
      AttribTarget, /**< We are currently targetting an attribute */
      CdataTarget   /**< We are currently targetting cdata */
   };

    /** Helper to write the data to the current string. */
    template<class T>
    void writeValueStringRep(const T& val)
    {
       std::ostringstream oss;
       oss << val;
       if(AttribTarget == mCurTarget)
       {
          if(!mCurAttribData.empty())  // Add spacing
          {
             mCurAttribData += ' ';
          }
          mCurAttribData += oss.str();
          //std::cout << "writingValueStringRep: Attrib\nval:" << val << std::endl
          //          << "str rep:" << oss.str() << std::endl
          //          << "new attrib data:" << mCurAttribData << std::endl;
       }
       else
       {
          if(!mCurCData.empty())  // Add spacing
          {
             mCurCData += ' ';
          }
          mCurCData += oss.str();
          //std::cout << "writingValueStringRep: Cdata\nval:" << val << std::endl
          //          << "str rep:" << oss.str() << std::endl
          //          << "new cdata data:" << mCurCData << std::endl;
       }
    }

protected:
   /** @name CppDOM nodes
    * When constructed, these are both null.
    */
   //@{
   cppdom::NodePtr   mRootNode;        /**< Base node of the tree */
   cppdom::Node*     mCurNode;         /**< Element we are currently working with. (ptr since getParent is weak) */
   std::string       mCurCData;        /**< Temporary place to store the value of the current cdata */
   std::string       mCurAttribData;   /**< Temporary place to store the current attribute data */
   std::string       mCurAttribName;   /**< The name of the current attribute we are working on */
   CurTarget         mCurTarget;       /**< Are we currently writing to attributes or cdata */
   //@}
};

} // namespace vpr


#endif
