#ifndef _VJ_DISPLAY_MANAGER_
#define _VJ_DISPLAY_MANAGER_
#pragma once

#include <vjConfig.h>

class vjDrawManager;
class vjDisplay;

#include <Input/vjPosition/vjPosition.h>
#include <Kernel/vjDebug.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Kernel/vjConfigChunkHandler.h>

//-----------------------------------------------------------------------------
//: Singleton Container class for all vjDisplays.
//
// PURPOSE:
//	This class is responsible for holding the data about display aspects
//  of the application.  The display object are window/system independant
//  this class is in charge of holding all the display data and keeping it
//  current.  This includes updating projections, adding/deleting new displays, etc.
//
// @author Allen Bierbaum
//  Date: 9-7-97
//-----------------------------------------------------------------------------
class vjDisplayManager : public vjConfigChunkHandler
{
public:     // --- Config stuff -- //
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! POST: (display of same name already loaded) ==> old display closed, new one opened
   //+       (display is new) ==> (new display is added)
   //+       draw manager is notified of the display change
   virtual bool configAdd(vjConfigChunk* chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   virtual bool configRemove(vjConfigChunk* chunk);

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(vjConfigChunk* chunk);

public:
   //: This function updates the projections for all contained displays.
   // It also tells the draw manager that the projections have changed
   virtual void updateProjections();

   //: Set the draw manager that the system is running
   // We need to know this in order to notify the draw
   // manager of any display changes
   //! POST: draw manager is notified of any displays currently configured
   void setDrawManager(vjDrawManager* drawMgr);

   //: Return a list of the current displays
   //! NOTE: DO NOT EDIT THE DISPLAYS
   std::vector<vjDisplay*> getActiveDisplays()
   { return mActiveDisplays;}

   //: Return list of inactive displays
   //! NOTE: DO NOT EDIT THE DISPLAYS
   std::vector<vjDisplay*> getInActiveDisplays()
   { return mInactiveDisplays;}

   //: Return list of all displays (inactive and active)
   //! NOTE: DO NOT EDIT THE DISPLAYS
   std::vector<vjDisplay*> getAllDisplays();

private:
   //: Add a display to the current system
   //! PRE: disp is a valid display
   //! POST: disp has been added to the list of displays
   //+  (notifyDrawMgr == true) && (drawMgr != NULL) && (disp is active)
   //+  ==> Draw manager now has been given new window to display
   int addDisplay(vjDisplay* disp, bool notifyDrawMgr = true);

   //: Close the given display
   //! PRE: disp is a display we know about
   //! POST: disp has been removed from the list of displays
   //+   (notifyDrawMgr == true) && (drawMgr != NULL) && (disp is active)
   //+   ==> Draw manager has been told to clode the window for the display
   int closeDisplay(vjDisplay* disp, bool notifyDrawMgr = true);

   // Is the display a member of the display manager
   bool isMemberDisplay(vjDisplay* disp);

   //: Find a display given the display name
   //! RETURNS: NULL - not found
   vjDisplay* findDisplayNamed(std::string name);

public:
   std::vector<vjDisplay*> mActiveDisplays;           //: List of currently active displays
   std::vector<vjDisplay*> mInactiveDisplays;   //: List of currently inactive displays

protected:
   vjDrawManager*  mDrawManager;     //: The current drawManager to communicate with


   // ---- Singleton stuff ---- //
public:
   static vjDisplayManager* instance()
   {
      if (_instance == NULL)
         _instance = new vjDisplayManager();
      return _instance;
   }

protected:
   vjDisplayManager() : mDrawManager(NULL)
   {;}

private:
   static vjDisplayManager* _instance;
};

#endif
