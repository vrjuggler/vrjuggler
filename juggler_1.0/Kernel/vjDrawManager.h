#ifndef _VJ_DRAW_MANAGER_
#define _VJ_DRAW_MANAGER_

#include <vjConfig.h>

class vjDisplayManager;
class vjApp;
class vjConfigChunkDB;

//--------------------------------------------------------
//: Abstract base class for API specific Draw Manager.
//
//	Concrete classes are resonsible for all rendering. 
//
// @author Allen Bierbaum
//  Date: 9-7-97
//--------------------------------------------------------
class vjDrawManager
{
public:
   
   //: Function to config API specific stuff.
   // Takes a chunkDB and extracts API specific stuff
   virtual void config(vjConfigChunkDB*  chunkDB) = 0;

   //: Enable a frame to be drawn
   virtual void draw() = 0;
   
   //: Blocks until the end of the frame
   //! POST: The frame has been drawn
   virtual void sync() = 0;

   //: Set the app the draw whould interact with.
   //! NOTE: The member variable is not in the base class
   //+    because it's "real" type is only known in the derived classes
   virtual void setApp(vjApp* _app) = 0;

   void setDisplayManager(vjDisplayManager* _dispMgr);

   //: Initialize the drawing API (if not already running)
   virtual void initAPI() = 0;

   //: Initialize the drawing state for the API 
   // based on the data in the display manager.
   //! PRE: API is running (initAPI has been called)
   //! POST: API is ready do draw
   virtual void initDrawing() = 0;

   //: Shutdown the drawing API
   virtual void closeAPI() = 0;

   //: Updates the projections for the displays.
   // It calls the displayManager to update the display.projection
   // stuff.  Then it does any API specific stuff.
   virtual void updateProjections();

protected:   
   vjDisplayManager* displayManager;  //: The display manager dealing with       
};
#endif
