/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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

#ifndef _VRJ_DISPLAY_MANAGER_H_
#define _VRJ_DISPLAY_MANAGER_H_

#include <vrj/vjConfig.h>

#include <vpr/Util/Singleton.h>

#include <vrj/Input/Type/Position.h>
#include <vrj/Util/Debug.h>
#include <vrj/Kernel/ConfigChunkHandler.h>

namespace vrj
{
   
class DrawManager;
class Display;

//-----------------------------------------------------------------------------
//: Singleton Container class for all Displays.
//
// PURPOSE:
// This class is responsible for holding the data about display aspects
//  of the application.  The display object are window/system independant
//  this class is in charge of holding all the display data and keeping it
//  current.  This includes updating projections, adding/deleting new displays, etc.
//
// @author Allen Bierbaum
//  Date: 9-7-97
//-----------------------------------------------------------------------------
class VJ_CLASS_API DisplayManager : public ConfigChunkHandler
{
public:     // --- Config stuff -- //
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   virtual bool configAdd(ConfigChunk* chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   virtual bool configRemove(ConfigChunk* chunk);

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(ConfigChunk* chunk);

public:
   //: This function updates the projections for all contained displays.
   // It also tells the draw manager that the projections have changed
   virtual void updateProjections();

   //: Set the draw manager that the system is running
   // We need to know this in order to notify the draw
   // manager of any display changes
   //! POST: draw manager is notified of any displays currently configured
   void setDrawManager(DrawManager* drawMgr);

   //: Return a list of the current displays
   //! NOTE: DO NOT EDIT THE DISPLAYS
   std::vector<vrj::Display*> getActiveDisplays()
   { return mActiveDisplays;}

   //: Return list of inactive displays
   //! NOTE: DO NOT EDIT THE DISPLAYS
   std::vector<vrj::Display*> getInActiveDisplays()
   { return mInactiveDisplays;}

   //: Return list of all displays (inactive and active)
   //! NOTE: DO NOT EDIT THE DISPLAYS
   std::vector<vrj::Display*> getAllDisplays();

   ConfigChunk* getDisplaySystemChunk()
   {return mDisplaySystemChunk;}

private:
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! POST: (display of same name already loaded) ==> old display closed, new one opened
   //+       (display is new) ==> (new display is added)
   //+       draw manager is notified of the display change
   bool configAddDisplay(ConfigChunk* chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   bool configRemoveDisplay(ConfigChunk* chunk);


   //: Add a display to the current system
   //! PRE: disp is a valid display
   //! POST: disp has been added to the list of displays
   //+  (notifyDrawMgr == true) && (drawMgr != NULL) && (disp is active)
   //+  ==> Draw manager now has been given new window to display
   int addDisplay(vrj::Display* disp, bool notifyDrawMgr = true);

   //: Close the given display
   //! PRE: disp is a display we know about
   //! POST: disp has been removed from the list of displays
   //+   (notifyDrawMgr == true) && (drawMgr != NULL) && (disp is active)
   //+   ==> Draw manager has been told to clode the window for the display
   int closeDisplay(vrj::Display* disp, bool notifyDrawMgr = true);

   // Is the display a member of the display manager
   bool isMemberDisplay(vrj::Display* disp);

   //: Find a display given the display name
   //! RETURNS: NULL - not found
   vrj::Display* findDisplayNamed(std::string name);

public:
   std::vector<vrj::Display*> mActiveDisplays; //: List of currently active displays
   std::vector<vrj::Display*> mInactiveDisplays; //: List of currently inactive displays

protected:
   DrawManager*    mDrawManager;           //: The current drawManager to communicate with
   ConfigChunk*    mDisplaySystemChunk;    //: Config chunk for the displaySystem



protected:
   DisplayManager() : mDrawManager(NULL)
   {
       mDisplaySystemChunk = NULL;
   }

   virtual ~DisplayManager()
   {;}

   DisplayManager (const DisplayManager& o) {;}
   void operator= (const DisplayManager& o) {;}

   vprSingletonHeader( DisplayManager );
};

} // end namespace vrj
#endif
