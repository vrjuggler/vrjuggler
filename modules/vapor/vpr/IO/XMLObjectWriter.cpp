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

#include <vpr/vprConfig.h>

#include <sstream>

#include <vpr/Util/Assert.h>
#include <vpr/IO/XMLObjectWriter.h>


namespace vpr
{

std::vector<vpr::Uint8> XMLObjectWriter::getData()
{
   vprASSERT(mRootNode.get() != NULL);
   std::stringstream oss;
   mRootNode->save(oss, 0);
   std::string str_rep = oss.str();
   std::vector<vpr::Uint8> ret_buffer(str_rep.begin(), str_rep.end());
   return ret_buffer;
}

// Starts a new section/element of name tagName.
// When mCurNode is Null, then we need to allocated a new node in its place.
// And also check the Root node to set it to (this is the first call to
// beginTag).
vpr::ReturnStatus XMLObjectWriter::beginTag(const std::string& tagName)
{
   cppdom::NodePtr new_node;

   if(mCurNode == NULL)    // First node created, so initialize everything
   {
      cppdom::ContextPtr cxt(new cppdom::Context);
      new_node = cppdom::NodePtr(new cppdom::Node(tagName, cxt));
      vprASSERT(mRootNode.get() == NULL && "Tried to root the writer twice");
      mRootNode = new_node;
      mCurNode = new_node.get();
   }
   else                          // Create new child of current node
   {
      // Check for pending cdata to write
      // - If there is some, then write it out
      if(!mCurCData.empty())
      {
         mCurNode->setCdata(mCurCData);
         mCurCData.clear();
      }

      // Add new child node
      new_node = cppdom::NodePtr(new cppdom::Node(tagName, mRootNode->getContext()));
      mCurNode->addChild(new_node);
      mCurNode = new_node.get();
   }
   return vpr::ReturnStatus::Succeed;
}

// Ends the most recently named tag.
// Close off the current node and set current to its parent.
vpr::ReturnStatus XMLObjectWriter::endTag()
{
   vprASSERT(mCurNode != NULL);
   //vprASSERT(mCurNode->getParent() != NULL);

   // Write out the cdata
   if(!mCurCData.empty())
   {
      mCurNode->setCdata(mCurCData);
      mCurCData.clear();
   }

   cppdom::Node* cur_parent = mCurNode->getParent();     // Get cur node's parent node
   mCurNode = cur_parent;                                // Set to new current node

   // If it had some cdata, then get it to initialize the current cdata
   // so we can write more data to it
   if(NULL != mCurNode)
   {
      mCurCData = mCurNode->getCdata();
   }

   return vpr::ReturnStatus::Succeed;
}

// Starts an attribute of the name attributeName.
vpr::ReturnStatus XMLObjectWriter::beginAttribute(const std::string& attributeName)
{
   // Make sure that we have not called beginAttribute without an endAttribute
   vprASSERT(mCurAttribName.empty() && "Didn't close previous attribute");
   vprASSERT(mCurAttribData.empty() && "There should not be any attribute data now.  It should have been flushed");   

   mCurAttribName = attributeName;
   mCurTarget = AttribTarget;
   return vpr::ReturnStatus::Succeed;
}

// Ends the most recently named attribute.
vpr::ReturnStatus XMLObjectWriter::endAttribute()
{
   vprASSERT(AttribTarget == mCurTarget);

   mCurNode->setAttribute(mCurAttribName, mCurAttribData);
   //std::cout << "Setting attrib:[" << mCurAttribName << "] value:[" << mCurAttribData << "]\n";

   mCurAttribName.clear();
   mCurAttribData.clear();

   mCurTarget = CdataTarget;

   return vpr::ReturnStatus::Succeed;
}

vpr::ReturnStatus XMLObjectWriter::writeUint8(vpr::Uint8 val)
{
   // Cast to uint16 so it doesn't get written as a char
   return writeValueStringRep(vpr::Uint16(val));
}

vpr::ReturnStatus XMLObjectWriter::writeUint16(vpr::Uint16 val)
{
   return writeValueStringRep(val);
}

vpr::ReturnStatus XMLObjectWriter::writeUint32(vpr::Uint32 val)
{
   return writeValueStringRep(val);
}

vpr::ReturnStatus XMLObjectWriter::writeUint64(vpr::Uint64 val)
{
   return writeValueStringRep(val);
}

vpr::ReturnStatus XMLObjectWriter::writeFloat(float val)
{
   return writeValueStringRep(val);
}

vpr::ReturnStatus XMLObjectWriter::writeDouble(double val)
{
   return writeValueStringRep(val);
}

vpr::ReturnStatus XMLObjectWriter::writeString(std::string val)
{
   if(AttribTarget == mCurTarget)
   {
      vprASSERT(mCurAttribData.empty() && "Can't add string to non-empty attribute");
      mCurAttribData = val;
   }
   else
   {
      if(!mCurCData.empty())  // Add spacing
         mCurCData += ' ';
      mCurCData += '"' + val + '"';    // Add string in quotes
   }

   return vpr::ReturnStatus::Succeed;
}

vpr::ReturnStatus XMLObjectWriter::writeBool(bool val)
{
   return writeValueStringRep(val);
}

} // End of vpr namespace
