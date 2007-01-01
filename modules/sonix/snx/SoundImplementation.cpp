/****************** <SNX heading BEGIN do not edit this line> *****************
 *
 * sonix
 *
 * Original Authors:
 *   Kevin Meinert, Carolina Cruz-Neira
 *
 ****************** <SNX heading END do not edit this line> ******************/

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

#include <snx/snxConfig.h>

#include <gmtl/Math.h>
#include <gmtl/Vec.h>
#include <gmtl/MatrixOps.h>
#include <gmtl/VecOps.h>
#include <gmtl/Xforms.h>
#include <vpr/Util/Assert.h>

#include <snx/SoundImplementation.h>


namespace snx
{

void SoundImplementation::copy( const SoundImplementation& si )
{
   // copy over the current state
   mName = si.mName;
   mSounds = si.mSounds;
   mListenerPos = si.mListenerPos;
   mSoundAPIInfo = si.mSoundAPIInfo;
}

void SoundImplementation::configure( const std::string& alias, const snx::SoundInfo& description )
{
   this->unbind( alias );
   snx::SoundInfo temp = mSounds[alias];
   mSounds[alias] = description; // TODO: put is_playing within the SoundInfo.

   // restore fields that should be preserved on a reconfigure...
   mSounds[alias].triggerOnNextBind = temp.triggerOnNextBind;
   //std::cout<<"DEBUG: triggerOnNextBind = "<<mSounds[alias].triggerOnNextBind<<"\n"<<std::flush;

   if (this->isStarted())
   {
      this->bind( alias );
   }
}

void SoundImplementation::remove(const std::string& alias)
{
   if (this->isStarted())
   {
      this->unbind( alias );
   }
   mSounds.erase( alias );
}

void SoundImplementation::bindAll()
{
   std::map< std::string, snx::SoundInfo >::iterator it;
   for( it = mSounds.begin(); it != mSounds.end(); ++it)
   {
      //std::cout<<"DEBUG: loading alias: "<<(*it).first<<"\n"<<std::flush;
      this->bind( (*it).first );
   }
}   

void SoundImplementation::unbindAll()
{
   std::map< std::string, snx::SoundInfo >::iterator it;
   for( it = mSounds.begin(); it != mSounds.end(); ++it)
   {
      //std::cout<<"DEBUG: loading alias: "<<(*it).first<<"\n"<<std::flush;
      this->unbind( (*it).first );
   }
   /*
   std::map< std::string, AlSoundInfo >::iterator it;
   for( it = mBindLookup.begin(); it != mBindLookup.end(); ++it)
   {
      this->unbind( (*it).first );
   }

   vprASSERT(mBindLookup.size() == 0 && "unbindAll failed");
   */
}

} // End of snx namespace
