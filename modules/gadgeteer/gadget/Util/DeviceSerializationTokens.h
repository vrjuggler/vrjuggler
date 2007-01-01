/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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

#ifndef _GADGET_DEV_SERIALIZATION_TOKENS_H_
#define _GADGET_DEV_SERIALIZATION_TOKENS_H_

namespace gadget
{

namespace tokens
{
   /** Id for the attribute containing a unique data type id */
   const std::string DataTypeAttrib("data_type_id");

   /** Id for the attrib containing the length of the entire sample buffer */
   const std::string SampleBufferLenAttrib("sample_buffer_len");

   /** Id for the Tag of a buffer sample */
   const std::string BufferSampleTag("BufferSample");

   /** Id for the attrib containing the length of the sample */
   const std::string BufferSampleLenAttrib("sample_len");

   const std::string PosValue("PosValue");
   const std::string DigitalValue("DigValue");
   const std::string StringValue("StrValue");
   const std::string AnalogValue("AnaValue");
   const std::string GloveValue("GloValue");

   const std::string TimeStamp("timestamp");
}

}

#endif
