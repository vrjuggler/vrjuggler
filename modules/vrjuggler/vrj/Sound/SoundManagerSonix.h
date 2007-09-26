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

#ifndef _VRJUGGLER_SONIX_SOUND_MANAGER_
#define _VRJUGGLER_SONIX_SOUND_MANAGER_

#include <vrj/vrjConfig.h>
#include <vpr/Util/Interval.h>
#include <vrj/Sound/SoundManager.h>

namespace vrj
{
   /** \class SoundManagerSonix SoundManagerSonix.h vrj/Sound/SoundManagerSonix.h
    *
    * Enables VR Juggler to reconfigure the sonix sound interface.
    */
   class SoundManagerSonix : public SoundManager
   {
   public:
      SoundManagerSonix();

      /**
       * Adds the element to the configuration.
       * @pre configCanHandle(element) == true
       */
      virtual bool configAdd(jccl::ConfigElementPtr element);

      /**
       * Removes the element from the current configuration.
       * @pre configCanHandle(element) == true
       */
      virtual bool configRemove(jccl::ConfigElementPtr element);

      /**
       * Can the handler handle the given element?
       * @return true if we can handle it; false if not
       */
      virtual bool configCanHandle(jccl::ConfigElementPtr element);

   public:
      /** Enables a frame to be drawn. */
      virtual void update();

      /**
       * Blocks until the end of the frame.
       * @post The frame has been rendered.
       */
      virtual void sync();

      /**
       * Tells Sonix to shut down the current sound API. Depending on what
       * sound API is in use, this will cause sounds to stop playing and
       * loaded resources to be released.
       *
       * @since 2.3.15
       */
      virtual void closeAPI();

   private:
      vpr::Interval mLastFrameTime;
   };
} // end namespace

#endif   /* _VRJUGGLER_SOUND_MANAGER_SONIX_ */
