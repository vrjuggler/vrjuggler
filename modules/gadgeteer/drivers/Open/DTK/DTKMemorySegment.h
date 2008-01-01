/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _GADGET_DTK_MEMORY_SEGMENT_H_
#define _GADGET_DTK_MEMORY_SEGMENT_H_

#include <gadget/Devices/DriverConfig.h>
#include <drivers/Open/DTK/DTK.h>

namespace gadget
{

typedef enum
{
   dtk_pos_ZYXEuler = 0,
   dtk_pos_XYZEuler,
   dtk_pos_QUAT,
   dtk_digital,
   dtk_analog,
   dtk_custom
} DTK_memoryType;

typedef enum
{
   DTK_INVALID = 0,
   DTK_INT,
   DTK_FLOAT,
   DTK_BYTES,
   DTK_CSTRING
} DTK_dataType;

class dtkSharedMem;
class dtkClient;

class GADGET_CLASS_API DTKMemorySegment
{
public:
   DTKMemorySegment();
   ~DTKMemorySegment();

   /** Configure the device with a config chunk. */
   bool config(jccl::ConfigChunkPtr c);

   /** Returns what chunk type is associated with this class. */
   static std::string getChunkType()
   {
      return std::string("DTKMemorySegment");
   }

   const char* SegmentName()
   {
      return _segmentName;
   }

   const char* RemoteHost()
   {
      return _remotehost;
   }

   DTK_memoryType SegmentType()
   {
      return _segmentType;
   }

   /**
    * Returns the number of items, size of each item, and the total memory
    * size of the specified memory segment.  This allows users to make sure
    * they are receiving the correct data.
    */
   int ItemCount()
   {
      return _numItems;
   }

   int ItemSize()
   {
      if ( _segmentSize % _numItems != 0 ); //ERROR!!!  Do something here!!!
      return _segmentSize/_numItems;
   }

   int SegmentSize()
   {
      return _segmentSize;
   }

   /**
    * The index is used by the Multi type device DTK.  The index is used for
    * the proxy interfaces.  The Item index is specified at configuration
    * time, depending on the order each chunk was entered.
    */
   int ItemIndex()
   {
      return InputIndex;
   }
   void setItemIndex(int i)
   {
      InputIndex = i;
   }

   /** Makes the connection to the DTK Client and gets the memory segment. */
   bool connectSegment(dtkClient* in_parent);

public:

   /** @name Typecasting Operators. */
   //@{
   operator float*() const;
   operator int*() const;
   operator char*() const;
   //@}

private:
   dtkSharedMem* m;
   dtkClient* parent_client;

   char* _segmentName;
   char* _remotehost;

   DTK_memoryType _segmentType;
   DTK_dataType _type;

   int InputIndex;


   int _numItems;
   int _segmentSize;

   float *floatData;
   int   *intData;
   char  *charData;
};

} // End of gadget namespace

#endif
