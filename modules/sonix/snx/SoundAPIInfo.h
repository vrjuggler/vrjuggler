
/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 ****************** <SNX heading END do not edit this line> ******************/
/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000, 2001 by Iowa State University
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





#ifndef SNX_SOUND_API_INFO
#define SNX_SOUND_API_INFO

namespace snx
{

class SoundAPIInfo
{
public:
   SoundAPIInfo() : voices( 65535 ), speakerConfig( STEREO ), sampleRate( STEREO_22050_KHZ ) {}

   int voices; // 0 - n (hardware limited)
   
   enum SpeakerConfig
   {
      MONO, STEREO, QUAD, DOLBY5_1, OCT
   };   

   SpeakerConfig speakerConfig;

   enum SampleRate
   {
      STEREO_22050_KHZ, MONO_22050_KHZ, STEREO_44100_KHZ, MONO_44100_KHZ
   };

   SampleRate sampleRate;
};

}; // end namespace

#endif //SNX_SOUND_API_INFO
