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

#ifndef _VPR_XML_OBJECT_READER_H
#define _VPR_XML_OBJECT_READER_H

#include <vpr/vprConfig.h>

#include <vector>

#include <boost/static_assert.hpp>
#include <vpr/IO/ObjectReader.h>

#include <cppdom/cppdom.h>
#include <string>
#include <sstream>

#include <vpr/Util/Assert.h>


namespace vpr
{

/** Object reader that reads out of a data buffer
*
* @todo: Add smart buffering for type sizes
*/
class VPR_CLASS_API XMLObjectReader : public ObjectReader
{
public:
   XMLObjectReader(std::vector<vpr::Uint8> data);

   XMLObjectReader(cppdom::NodePtr rootNode);

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
   virtual std::string readString();
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
   virtual void readString(std::string& str)
   { str = this->readString(); }
   virtual void readBool(bool& val)
   { val = this->readBool(); }


protected:
   enum CurSource
   { AttribSource, /**< We are currently targetting an attribute */
     CdataSource /**< We are currently targetting cdata */
    };

    /** Helper to read the data from the current string */
    template<class T>
    vpr::ReturnStatus readValueStringRep(T& val)
    {
       std::stringstream* in_stream(NULL);
       //std::string stream_content;

       if(AttribSource == mCurSource)
       { 
          in_stream = &mAttribSource; 
          //stream_content = mAttribSource.str();
       }
       else
       { 
          in_stream = &(mCurNodeStack.back().cdataSource);
          //stream_content = mCurNodeStack.back().cdataSource.str();
       }

       // Just to be safe do this again for the heck of it. :)
       //in_stream->exceptions( std::ios::eofbit | std::ios::failbit | std::ios::badbit );
       vprASSERT(!in_stream->bad() && "Bad stream, BAAADDD stream.");
       vprASSERT(!in_stream->fail() && "Stream failed.");
       vprASSERT(!in_stream->eof() && "Stream EOF'd.");
       vprASSERT(in_stream->good());

       //stream_content = (*in_stream).str();

       (*in_stream) >> val;

       return vpr::ReturnStatus::Succeed;
    }

    /** Initialize the members based on a serialized version of something in the data buffer */
    void initCppDomTree(std::vector<vpr::Uint8> data);

    void debugDumpStack(int debug_level);

protected:
   /** State to keep at each level of recursion */
   struct NodeState
   {
      NodeState(cppdom::Node* cur_node=NULL);

      NodeState(const NodeState& rhs);

      NodeState& operator=(const NodeState& rhs);

      void debugDump(int debug_level);

      cppdom::Node*              node;          /**< The node that this state is for */
      cppdom::NodeList::iterator nextChild_i;      /**< Index of the current child we are examining */
      cppdom::NodeList::iterator endChild_i;      /**< The end of the child list */
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


} // namespace vpr

#endif

