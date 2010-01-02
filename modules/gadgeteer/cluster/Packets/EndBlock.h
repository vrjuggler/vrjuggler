/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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

#ifndef _GADGET_RIM_END_BLOCK_H
#define _GADGET_RIM_END_BLOCK_H

#include <gadget/gadgetConfig.h>

#include <vpr/vprTypes.h>
#include <vpr/IO/BufferObjectReader.h>
#include <vpr/IO/BufferObjectWriter.h>

#include <cluster/Packets/Header.h>
#include <cluster/Packets/Packet.h>
#include <cluster/Packets/EndBlockPtr.h>

#include <boost/concept_check.hpp>

namespace cluster
{

class GADGET_CLASS_API EndBlock
   : public Packet
{
protected:
   EndBlock();

   /**
    * Create a EndBlock packet to signal that the local node has reached the
    * end of the frame.
    *
    * @param frame_number The current number of frames that have been drawn.
    */
   EndBlock(const vpr::Uint32 frameNum);

public:
   /**
    * Creates a EndBlock instance and returns it wrapped in a
    * EndBlockPtr object.
    *
    * @since 1.3.7
    */
   static EndBlockPtr create();

   /**
    * Creates a EndBlock instance and returns it wrapped in a
    * EndBlockPtr object.
    *
    * @since 1.3.7
    */
   static EndBlockPtr create(const vpr::Uint32 frameNum);

   virtual ~EndBlock();

   /**
    * Serializes member variables into a data stream.
    */
   void serialize();

   /**
    * Parses the data stream into the local member variables.
    */
   virtual void parse();

   /**
    * Print the data to the screen in a readable form.
    */
   virtual void printData(int debugLevel) const;

   /**
    * Return the type of this packet.
    */
   static vpr::Uint16 getPacketFactoryType()
   {
       return(Header::RIM_END_BLOCK);
   }

   vpr::Uint16 getTempVar()
   {
      return mTempVar;
   }

private:
   vpr::Uint16    mTempVar;  /**< Temporary variable that is no longer used. */
};

}


#endif
