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

#include <vpr/vprConfig.h>

#include <vpr/IO/BufferObjectWriter.h>


namespace vpr
{
/** Number of bytes used to store the size of the string. */
const unsigned int BufferObjectWriter::STRING_LENGTH_SIZE = 4;

BufferObjectWriter::BufferObjectWriter()
   : mOwnDataBuffer(true)
   , mData(new std::vector<vpr::Uint8>)
   , mCurHeadPos(0)
{
   mIsBinary = true;
}

BufferObjectWriter::BufferObjectWriter(std::vector<vpr::Uint8>* data,
                                       const unsigned int curPos)
   : mOwnDataBuffer(false)
   , mData(data)
   , mCurHeadPos(curPos)
{
   mIsBinary = true;
}

BufferObjectWriter::~BufferObjectWriter() throw ()
{
   if(mOwnDataBuffer)
   {
      delete mData;
      mData = NULL;
   }
}

}
