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

#ifndef _VPR_XML_OBJECT_READER_H
#define _VPR_XML_OBJECT_READER_H

#include <vpr/vprConfig.h>

#include <vector>
#include <string>
#include <sstream>
#include <boost/static_assert.hpp>
#include <cppdom/cppdom.h>

#include <vpr/Util/Assert.h>
#include <vpr/IO/ObjectReader.h>


namespace vpr
{

/** \class XMLObjectReader XMLObjectReader.h vpr/IO/XMLObjectReader.h
 *
 * Object reader that reads out of a data buffer.
 *
 * @todo Add smart buffering for type sizes.
 */
class VPR_CLASS_API XMLObjectReader : public ObjectReader
{
public:
   XMLObjectReader(const std::vector<vpr::Uint8>& data);

   XMLObjectReader(cppdom::NodePtr rootNode);

   virtual ~XMLObjectReader() throw ()
   {
      /* Do nothing. */ ;
   }

   /** @name Tag and attribute handling. */
   //@{
   /** Starts a new section/element of name \p tagName. */
   virtual void beginTag(const std::string& tagName) throw (IOException);

   /** Ends the most recently named tag. */
   virtual void endTag() throw (IOException);

   /** Starts an attribute of the name \p attributeName. */
   virtual void beginAttribute(const std::string& attributeName)
      throw (IOException);

   /** Ends the most recently named attribute. */
   virtual void endAttribute() throw (IOException);
   //@}

   virtual void resetReading();

   virtual void pushState();
   virtual void popState();

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual vpr::Uint8 readUint8() throw (IOException);

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual vpr::Uint16 readUint16() throw (IOException);

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual vpr::Uint32 readUint32() throw (IOException);

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual vpr::Uint64 readUint64() throw (IOException);

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual float readFloat() throw (IOException);

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual double readDouble() throw (IOException);

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual std::string readString() throw (IOException);

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual bool readBool() throw (IOException);

   /** @name Helper methods */
   //@{

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint8(vpr::Uint8& val) throw (IOException)
   {
      val = this->readUint8();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint16(vpr::Uint16& val) throw (IOException)
   {
      val = this->readUint16();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint32(vpr::Uint32& val) throw (IOException)
   {
      val = this->readUint32();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readUint64(vpr::Uint64& val) throw (IOException)
   {
      val = this->readUint64();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readFloat(float& val) throw (IOException)
   {
      val = this->readFloat();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readDouble(double& val) throw (IOException)
   {
      val = this->readDouble();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readString(std::string& str) throw (IOException)
   {
      str = this->readString();
   }

   /**
    * @throw EOFException If end of file is reached while reading.
    * @throw IOException  If some other I/O error occurs while reading from
    *                     the underlying data source.
    */
   virtual void readBool(bool& val) throw (IOException)
   {
      val = this->readBool();
   }
   //@}

protected:
   enum CurSource
   {
      AttribSource, /**< We are currently targetting an attribute. */
      CdataSource   /**< We are currently targetting cdata. */
   };

   /** Gets the current string source we are reading from. */
   std::stringstream* getCurSource();

   /**
    * Initializes the members based on a serialized version of something in
    * the data buffer.
    */
   void initCppDomTree(const std::vector<vpr::Uint8>& data);

   void debugDumpStack(int debug_level);

protected:
   /** State to keep at each level of recursion. */
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
   /**
    * @name CppDOM nodes.
    * When constructed, these are both null.
    */
   //@{
   cppdom::NodePtr         mRootNode;        /**< Base node of the tree */
   std::vector<NodeState>  mCurNodeStack;    /**< Stack of current nodes we are examining recursively */
   std::stringstream       mAttribSource;    /**< Source of attribute data */
   CurSource               mCurSource;       /**< The current source of reading data */
   //@}

   /** Stack to store state information. */
   struct ReadState
   {
      std::vector<NodeState> mNodeStack;      
   };
   std::vector<ReadState>  mReadStateStack;  /**< Stack of previous read states */
};

} // namespace vpr


#endif
