/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _VRJ_DRAW_MANAGER_
#define _VRJ_DRAW_MANAGER_

#include <vrj/vrjConfig.h>
#include <jccl/Config/ConfigChunkDB.h>
#include <jccl/RTRC/ConfigChunkHandler.h>


namespace vrj
{

class DisplayManager;
class App;
class Display;

/**
 * Abstract base class for API specific Draw Manager.
 *
 * Concrete classes are resonsible for all rendering.
 *
 * @date 9-7-97
 */
class VJ_CLASS_API DrawManager : public jccl::ConfigChunkHandler
{
public:
   DrawManager (void)
   {
      mDisplayManager = NULL;
   }

   /**
    * Function to initialy config API specific stuff.
    * Takes a chunkDB and extracts API specific stuff
    */
   //**//virtual void configInitial(jccl::ConfigChunkDB*  chunkDB) = 0;

   /// Enable a frame to be drawn
   virtual void draw() = 0;

   /**
    * Blocks until the end of the frame.
    * @post The frame has been drawn.
    */
   virtual void sync() = 0;

   /**
    * Sets the app the draw whould interact with.
    *
    * @note The member variable is not in the base class because its "real"
    * type is only known in the derived classes.
    */
   virtual void setApp(App* _app) = 0;

   /**
    * Initializes the drawing API (if not already running).
    *
    * @note If the draw manager should be an active object, start the process
    *       here.
    */
   virtual void initAPI() = 0;

   /// Callback when display is added to display manager
   virtual void addDisplay(Display* disp) = 0;

   /// Callback when display is removed to display manager
   virtual void removeDisplay(Display* disp) = 0;

   /**
    * Shutdown the drawing API.
    *
    * @note If it was an active object, kill process here.
    */
   virtual void closeAPI() = 0;

   /// Setter for display manager variable
   void setDisplayManager(DisplayManager* _dispMgr);
   DisplayManager* getDisplayManager();

   friend VJ_API(std::ostream&) operator<<(std::ostream& out,
                                           DrawManager& drawMgr);
   virtual void outStream(std::ostream& out)
   { out << "vjDrawManager: outstream\n"; }  // Set a default

protected:
   DisplayManager* mDisplayManager;  /**< The display manager dealing with */
};

};
#endif
