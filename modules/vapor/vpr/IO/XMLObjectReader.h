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
 * VR Juggler is (C) Copyright 1998-2002 by Iowa State University
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

#ifndef _VPR_XML_OBJECT_READER_H
#define _VPR_XML_OBJECT_READER_H

#include <vpr/vprConfig.h>

#include <vector>

#include <boost/static_assert.hpp>
#include <vpr/IO/ObjectReader.h>

#include <cppdom/cppdom.h>
#include <string>
#include <sstream>


namespace vpr
{

/** Object reader that reads out of a data buffer
*
* @todo: Add smart buffering for type sizes
*/
class XMLObjectReader : public ObjectReader
{
public:
   XMLObjectReader(std::vector<vpr::Uint8> data)
      : mCurSource(CdataSource)
   {
      initCppDomTree(data);
   }

   XMLObjectReader(cppdom::NodePtr rootNode)
      : mCurSource(CdataSource)
   {
      mRootNode = rootNode;
   }

   /** @name Tag and attribute handling */
   //@{
   /** Starts a new section/element of name tagName.
   */
   virtual vpr::ReturnStatus beginTag(std::string tagName);

   /** Ends the most recently named tag. */
   virtual vpr::ReturnStatus endTag();

   /** Starts an attribute of the name attributeName */
   virtual vpr::ReturnStatus beginAttribute(std::string attributeName);

   /** Ends the most recently named attribute */
   virtual vpr::ReturnStatus endAttribute();
   //@}

   virtual vpr::Uint8 readUint8();
   virtual vpr::Uint16 readUint16();
   virtual vpr::Uint32 readUint32();
   virtual vpr::Uint64 readUint64();
   virtual float readFloat();
   virtual double readDouble();
   virtual std::string readString(unsigned len);
   virtual bool readBool();

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
   virtual void readString(std::string& str, unsigned len)
   { str = this->readString(len); }
   virtual void readBool(bool& val)
   { val = this->readBool(); }


protected:
   enum CurSource
   { AttribSource, /**< We are currently targetting an attribute */
     CdataSource /**< We are currently targetting cdata */
    };

    /** Helper to write the data to the current string */
    template<class T>
    vpr::ReturnStatus readValueStringRep(T& val)
    {
       std::stringstream* in_stream(NULL);

       if(AttribSource == mCurSource)
       { in_stream = &mAttribSource; }
       else
       { in_stream = &(mCurNodeStack.back().cdataSource); }

       (*in_stream) >> val;

       return vpr::ReturnStatus::Succeed;
    }

    /** Initialize the members based on a serialized version of something in the data buffer */
    void initCppDomTree(std::vector<vpr::Uint8> data);

protected:
   /** State to keep at each level of recursion */
   struct NodeState
   {
      NodeState(cppdom::Node* cur_node=NULL)
         : node(cur_node)
      {
         nextChild = node->getChildren().begin();     // Initialize to the first child
         cdataSource.str(node->getFullCdata());
      }

      NodeState(const NodeState& rhs)
      {
         node = rhs.node;
         nextChild = rhs.nextChild;
         cdataSource.str(rhs.cdataSource.str());
      }

      NodeState& operator=(const NodeState& rhs)
      {
         node = rhs.node;
         nextChild = rhs.nextChild;
         cdataSource.str(rhs.cdataSource.str());
         return *this;
      }

      cppdom::Node*              node;          /**< The node that this state is for */
      cppdom::NodeList::iterator nextChild;      /**< Index of the current child we are examining */
      std::stringstream          cdataSource;   /**< The current cdata input source stream */
   };

protected:
   /* cppdom nodes
   * When constructed, these are both null.
   */
   cppdom::NodePtr         mRootNode;        /**< Base node of the tree */
   std::vector<NodeState>  mCurNodeStack;    /**< Stack of current nodes we are examining recursively */
   std::stringstream       mAttribSource;    /**< Source of attribute data */
   CurSource               mCurSource;       /**< The current source of reading data */
};

/** Initialize the members based on a serialized version of something in the data buffer */
inline void XMLObjectReader::initCppDomTree(std::vector<vpr::Uint8> data)
{
   std::string xml_data(data.begin(), data.end());
   std::stringstream xml_stream(xml_data);
   cppdom::ContextPtr cxt(new cppdom::Context);
   cppdom::NodePtr new_node = cppdom::NodePtr(new cppdom::Node(cxt));
   new_node->load(xml_stream, cxt);

   mRootNode = new_node;
   mCurNodeStack.clear();
}

/** Starting a new tag
* Get the local cdata into the current data source
*/
inline  vpr::ReturnStatus XMLObjectReader::beginTag(std::string tagName)
{
   // Find the correct child
   if(mCurNodeStack.empty())  // We should enter the root
   {
      mCurNodeStack.push_back(NodeState(mRootNode.get()));
   }
   else                      // Find the next child and push it on
   {
      vprASSERT(mCurNodeStack.back().nextChild != mCurNodeStack.back().node->getChildren().end() && "Past last child");

      // Make sure that we get to child of type node.
      // While next child is not of type node skip over cdata...
      while( (*(mCurNodeStack.back().nextChild))->getType() != cppdom::xml_nt_node)
      {
         mCurNodeStack.back().nextChild++;
         vprASSERT(mCurNodeStack.back().nextChild != mCurNodeStack.back().node->getChildren().end() && "Past last child");
      }

      cppdom::NodePtr next_child = *(mCurNodeStack.back().nextChild);      // Get the next child
      mCurNodeStack.back().nextChild++;                                    // Step to the next child for next time
      mCurNodeStack.push_back( NodeState(next_child.get()));               // Add the child to the stack
   }

   std::string cur_node_name = mCurNodeStack.back().node->getName();
   vprASSERT(tagName == cur_node_name && "Reading back in incorrect order");
   return vpr::ReturnStatus::Succeed;
}

/** Ends the most recently named tag.
* Just pop of the cur node state information
*/
inline vpr::ReturnStatus XMLObjectReader::endTag()
{
   vprASSERT(!mCurNodeStack.empty());
   mCurNodeStack.pop_back();
   return vpr::ReturnStatus::Succeed;
}

/** Starts an attribute of the name attributeName
* Get the attribute content and set the attribute source with that content
*/
inline vpr::ReturnStatus XMLObjectReader::beginAttribute(std::string attributeName)
{
   std::string attrib_content = mCurNodeStack.back().node->getAttribute(attributeName).getString();
   mAttribSource.str(attrib_content);
   mCurSource = AttribSource;             // Get content from attribute now
   return vpr::ReturnStatus::Succeed;
}

/** Ends the most recently named attribute */
inline vpr::ReturnStatus XMLObjectReader::endAttribute()
{
   mAttribSource.str("");     // Clear the attrib content (not required, but helps point out bugs)
   mCurSource = CdataSource;     // Set back to getting data from cdata
   return vpr::ReturnStatus::Succeed;
}
//@}


inline vpr::Uint8 XMLObjectReader::readUint8()
{
   // Read a uint16 so that it does not treat it as a single char
   vpr::Uint16 temp_data;
   readValueStringRep(temp_data);
   return vpr::Uint8(temp_data);
}

inline vpr::Uint16 XMLObjectReader::readUint16()
{
   vpr::Uint16 val;
   readValueStringRep(val);
   return val;
}

inline vpr::Uint32 XMLObjectReader::readUint32()
{
   vpr::Uint32 val;
   readValueStringRep(val);
   return val;
}

inline vpr::Uint64 XMLObjectReader::readUint64()
{
   vpr::Uint64 val;
   readValueStringRep(val);
   return val;
}

inline float XMLObjectReader::readFloat()
{
   float val;
   readValueStringRep(val);
   return val;
}

inline double XMLObjectReader::readDouble()
{
   double val;
   readValueStringRep(val);
   return val;
}


inline std::string XMLObjectReader::readString(unsigned len)
{
   std::string ret_val("");

   if(AttribSource == mCurSource)   // Just return a copy of the attribute source
   {
      ret_val = mAttribSource.str();
   }
   else  // Cdata, extract the "'s around the string
   {
      char char_buffer[1024];    // Buffer for writing the final value
      char temp_char('0');

      // Get the first "
      while('"' != temp_char)
      {
         mCurNodeStack.back().cdataSource.get(temp_char);
         vprASSERT(('>' != temp_char) && ('<' != temp_char) && "String delimiter not found");
      }

      // Get the contents of the string
      bool done_reading(false);
      while(!done_reading)
      {
         mCurNodeStack.back().cdataSource.get(char_buffer,1024, '"');      // Copy over to target until we have second "
         ret_val += char_buffer;                                           // append onto end
         done_reading = (mCurNodeStack.back().cdataSource.peek() == '"');     // Done reading if got to a "
      }
      mCurNodeStack.back().cdataSource.ignore(1);                    // Ignore that " as well
   }

   return ret_val;
}

inline bool XMLObjectReader::readBool()
{
   bool val;
   readValueStringRep(val);
   return val;
}

} // namespace vpr

#endif

