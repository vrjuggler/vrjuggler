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

#ifndef _VRJUGGLER_SOUND_MANAGER_
#define _VRJUGGLER_SOUND_MANAGER_

#include <vrj/vrjConfig.h>
#include <jccl/Config/ConfigElementPtr.h>
#include <jccl/RTRC/ConfigElementHandler.h>


namespace vrj
{
   /** \class SoundManager SoundManager.h vrj/Sound/SoundManager.h
    *
    * VR Juggler sound manager.
    */
   class VJ_CLASS_API SoundManager : public jccl::ConfigElementHandler
   {
   protected:
      SoundManager();

   public:
      virtual ~SoundManager();

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
       * @return true if we can handle the element; false if not.
       */
      virtual bool configCanHandle(jccl::ConfigElementPtr element);

      /** Enables a frame to be drawn. */
      virtual void update();

      /**
       * Blocks until the end of the frame.
       * @post The frame has been rendered.
       */
      virtual void sync();

      /**
       * Shuts down the sound API currently in use.
       *
       * @since 2.3.15
       */
      void closeAPI();
   };
} // end namespace

#endif   /* _VRJUGGLER_SOUND_MANAGER_ */
