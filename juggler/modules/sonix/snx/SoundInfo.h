
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




/* Generated by Together */

#ifndef SOUND_INFO_DATA
#define SOUND_INFO_DATA
#include <string>
#include <vector>
#include <vrj/Math/Vec3.h>

namespace snx
{

/**
 * info struct that describes one sound entry.
 */
struct SoundInfo
{
   SoundInfo() : alias(), 
                 datasource( FILESYSTEM ), 
                 filename(),
                 data(),
                 ambient( true ),
                 retriggerable( false ),
                 repeat( 1 ),
                 triggerOnNextBind( false ), 
                 repeatCountdown( 0 )
   {
      //position.makeIdent();
      position[0] = 0.0f;
      position[1] = 0.0f;
      position[2] = 0.0f;
   }
   
   
   
   void allocBuffers( int number, int size, unsigned int sampSize )
   {
      // only suport 8 or 16 bit sound right now.
      assert( sampSize == 1 || sampSize == 2 );
      data.resize( number );
      for (int x = 0; x < number; ++x)
      {
         data[x].resize( size * sampSize );
      }      
   }   
   
   std::string alias;

   enum DataSource
   {
      FILESYSTEM, DATA_16_MONO, DATA_8_MONO
   };
   DataSource datasource; // which of the following resources to use...

   // source of the sound...
   std::string filename;
   
   // this field is used when datasource != FILESYSTEM
   std::vector< std::vector<char> > data;

   vrj::Vec3 position;
   
   bool ambient;  // is the sound ambient (true) or positional (false)?
   bool retriggerable;  // can the sound be retriggered while playing?
   int repeat;           // number of times to repeat (static), -1 is infinite

   // -----------------------------------------------
   bool triggerOnNextBind;
   int repeatCountdown; // number of times left to repeat (changes during play)
};

}; // end namespace

#endif //SOUND_INFO_DATA
