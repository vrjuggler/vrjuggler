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

#include <gadget/Devices/DriverConfig.h>

#include <dtk.h>

#include <jccl/Config/ConfigChunk.h>
#include <drivers/Open/DTK/DTKMemorySegment.h>

namespace gadget
{

DTKMemorySegment::DTKMemorySegment()
{
   floatData = NULL;
   intData = NULL;
   charData = NULL;
   _segmentName = _remotehost = NULL;

   _numItems = _segmentSize = InputIndex = 0;
}

DTKMemorySegment::~DTKMemorySegment()
{
   if ( floatData != NULL ) delete [] floatData;
   if ( intData != NULL ) delete [] intData;
   if ( charData != NULL ) delete [] charData;

   if ( _segmentName != NULL ) delete [] _segmentName;
   if ( _remotehost != NULL ) delete [] _remotehost;
}


bool DTKMemorySegment::config(jccl::ConfigChunkPtr c)
{
   _type = (DTK_dataType) c->getProperty<int>("data_type");

   _numItems = c->getProperty<int>("item_count");
   _segmentType = (DTK_memoryType) c->getProperty<int>("input_type");

   _segmentName = c->getProperty<std::string>("segment_name").c_str();
   _remotehost = c->getProperty<std::string>("host_name").c_str();
   if(_remotehost[0] == '\0')
   {
      delete [] _remotehost;
      _remotehost = NULL;
   }

   switch(_segmentType)
   {
   case dtk_pos_ZYXEuler:
   case dtk_pos_XYZEuler:
      _numItems = 6; // Positional Euler data has to be 6 floats;
      _type = DTK_FLOAT;
      break;
   case dtk_pos_QUAT:
      _numItems = 4; // Quaternians have to be 4 floats;
      _type = DTK_FLOAT;
      break;
   case dtk_digital:
      _numItems = 1; // Assuming digital data is passes as single integers... STill must be tested and confirmed.
      _type = DTK_INT;
      break;
   case dtk_analog:
      _numItems = 1; // Same as digital, only we are assuming floats.  Both digital and analog need to be refined
      _type = DTK_FLOAT;
      break;
   case dtk_custom:
      break; // The dataType and itemCount must be specified earlier for this option
   }

   switch(_type)
   {
   case DTK_INVALID:
      break; // Don't allocate any data... just continue on.
   case DTK_INT:
      _segmentSize = sizeof(int)*_numItems;
      intData = (int*)malloc(_segmentSize);
      break;
   case DTK_FLOAT:
      _segmentSize = sizeof(float)*_numItems;
      floatData = (float*)malloc(_segmentSize);
      break;
   case DTK_BYTES:
   case DTK_CSTRING:
      _segmentSize = sizeof(char)*_numItems;
      charData = (char*)malloc(_segmentSize);
      break;
   default:
      break;
   }

   return true;
}

bool DTKMemorySegment::connectSegment(dtkClient* in_parent)
{
   parent_client = in_parent;
   m = parent_client->getSharedMem(_segmentSize, _segmentName);
   if ( _remotehost != NULL ) m->getRemote(_remotehost);
   return true;
}

DTKMemorySegment::operator float*() const
{
   if ( floatData == NULL ) return 0;

   m->read(floatData);
   if ( m->differentByteOrder() ) dtk_swapBytes(floatData, _segmentSize);

   switch ( _type )
   {
      case DTK_INVALID:
         return 0;
      case DTK_FLOAT:
         return floatData;
      default:
         return 0;
   }
}

DTKMemorySegment::operator int*() const
{
   if ( intData == NULL ) return 0;

   m->read(intData);
   if ( m->differentByteOrder() ) dtk_swapBytes(intData, _segmentSize);

   switch ( _type )
   {
      case DTK_INVALID:
         return 0;
      case DTK_INT:
         return intData;
      default:
         return 0;
   }
}

DTKMemorySegment::operator char*() const
{
   if ( charData == NULL ) return 0;

   m->read(charData);
   if ( m->differentByteOrder() ) dtk_swapBytes(charData, _segmentSize);

   switch ( _type )
   {
      case DTK_INVALID:
         return 0;
      case DTK_BYTES:
      case DTK_CSTRING:
         return charData;
      default:
         return 0;
   }
}

} // End of gadget namespace
