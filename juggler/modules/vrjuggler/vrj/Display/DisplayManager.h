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
//  of the application.  The displays themselves may be machine/OS specific,
//  but this class should be able to run for all configurations.  This
//  includes all machine/OS specific aspects.  For now that basically means
//  simply tracking window data.
//
// @author Allen Bierbaum
//  Date: 9-7-97
//-----------------------------------------------------------------------------
class vjDisplayManager : public vjConfigChunkHandler
{
public:
   //: This function updates the projections for all contained displays.
   virtual void updateProjections();

   //: Set the draw manager that the system is running
   // We need to know this in order to notify the draw
   // manager of any display changes
   void setDrawManager(vjDrawManager* drawMgr);

   //: Return a list of the current displays
   //! NOTE: DO NOT EDIT THE DISPLAYS
   std::vector<vjDisplay*> getDisplays()
   { return mDisplays;}

public:     // --- Config stuff -- //
   //: Add the chunk to the configuration
   //! PRE: configCanHandle(chunk) == true
   //! RETURNS: success
   virtual bool configAdd(vjConfigChunk* chunk);

   //: Remove the chunk from the current configuration
   //! PRE: configCanHandle(chunk) == true
   //!RETURNS: success
   virtual bool configRemove(vjConfigChunk* chunk);

   //: Can the handler handle the given chunk?
   //! RETURNS: true - Can handle it
   //+          false - Can't handle it
   virtual bool configCanHandle(vjConfigChunk* chunk);

private:
   //: Add a display to the current system
   //! PRE: Draw manager is known
   //! POST: disp has been added to the list of displays
   //+  (notifyDrawMgr == true) ==> Draw manager now has been given new window to display
   virtual int addDisplay(vjDisplay* disp, bool notifyDrawMgr = true);

   //: Close the given display
   virtual int closeDisplay(int dispId);

   //: Get the display information for the display
   virtual vjDisplay* getDisplay(int dispId);

public:
   std::vector<vjDisplay*>  mDisplays;    //: List of displays currently operating

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
   vjDisplayManager()
   {;}

private:
   static vjDisplayManager* _instance;
};

#endif
