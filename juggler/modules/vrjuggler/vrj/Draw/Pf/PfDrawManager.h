#ifndef _VJ_PF_DRAW_MANAGER_
#define _VJ_PF_DRAW_MANAGER_

#include <vjConfig.h>
#include <function.h>
#include <algorithm>

#include <Performer/pf/pfChannel.h>

#include <Kernel/vjDrawManager.h>
#include <Kernel/vjDisplay.h>
//#include <Kernel/Pf/vjPfApp.h>
#include <Kernel/Pf/vjPfUtil.h>


class vjPfApp;
class vjProjection;
class vjConfigChunkDB;
class vjSimDisplay;

    // Performer Config function called in draw proc after window is set up
void vjPFconfigPWin(pfPipeWindow* pWin);
void vjPfDrawFunc(pfChannel *chan, void* chandata);

//------------------------------------------------------------
//: Concrete singleton class for API specific Draw Manager.
//
//	Responsible for all Performer rendering and windowing
//
// @author Allen Bierbaum
//  Date: 9-7-97
//------------------------------------------------------------
class vjPfDrawManager : public vjDrawManager
{
protected:

   struct pfDisp
   {
      pfDisp()
      {
         chans[0] = chans[1] = NULL;
         disp = NULL;
         pWin = NULL;
      }

      enum {LEFT = 0, RIGHT = 1};

      vjDisplay*     disp;
      pfPipeWindow*  pWin;
      pfChannel*     chans[2];
   };

   struct findPfDispChan : unary_function<pfDisp, bool>
   {
      findPfDispChan(pfChannel* chan) : mChan(chan) {;}
      bool operator()(pfDisp disp) {
         return ((disp.chans[0] == mChan) || (disp.chans[1] == mChan));
      }
      pfChannel* mChan;                // Channel to find
   };

public:
    //: Function to config API specific stuff.
    // Takes a chunkDB and extracts API specific stuff
   virtual void configInitial(vjConfigChunkDB*  chunkDB);

    //: Blocks until the end of the frame
    //! PRE: none
    //! POST: The frame has been drawn
   virtual void sync();

   //: Enable a frame to be drawn
   //! PRE: none
   //! POST: Frame has been triggered to begin drawing
   virtual void draw();


   //: Set the app the draw whould interact with.
   //! PRE: none
   //! POST: self'.app = _app
   virtual void setApp(vjApp* _app);


   //: Initialize the drawing API (if not already running)
   //  should call pfInit()
   virtual void initAPI();

   //: Initialize the drawing state for the API based on
   //: the data in the display manager.
   //
   //! PRE: API is running (initAPI has been called)
   //! POST: API is ready do draw
   //
   // Configure process model
   // Configure multi-pipe model
   // Then call pfConfig to start the MP stuff
   // Sets up channels and pWins.
   //
   //!NOTE: Fork happens here
   virtual void initDrawing();

      //: Shutdown the drawing API
   virtual void closeAPI();

      //: Update all the projections for the displays
      //!POST: All windows have the projections correctly set.
   virtual void updateProjections();

   //: dumps the object's internal state
   void debugDump();

   friend void vjPFconfigPWin(pfPipeWindow* pWin);
   friend void vjPfDrawFunc(pfChannel *chan, void* chandata);

protected:
   //: Helper to set channel view params from a vjProjection
   void updatePfProjection(pfChannel* chan, vjProjection* proj, bool simulator=false);

   //: Helper function to bind Performer to the pfApp
   void initPerformerApp();

   //: Helper to initialize the Performer simulator
   void initSimulator();
   void initLoaders();
   void updateSimulator(vjSimDisplay* sim);

   //: Helper to get the pfDisp given a channel
   //! RETURNS: NULL - Not found
   pfDisp* getPfDisp(pfChannel* chan)
   {
      std::vector<pfDisp>::iterator theDisp = std::find_if(disps.begin(), disps.end(), findPfDispChan(chan));
      if(theDisp == disps.end())
         return NULL;
      else
         return (&(*theDisp));
   }


protected:
   // --- Config Data --- //
   int numPipes;    // The number of Performer pipes

   // --- Performer State --- //
   vjPfApp*             app;        // There User applications
   std::vector<pfDisp>  disps;      // List of displays with Performer data
   std::vector<pfPipe*> pipes;      // Performer pipes we have opened
   std::vector<char*> pipeStrs;     // The X-Strs of the pipes
   pfScene*          sceneRoot;     // Root of Performer tree to render

   // ---- Simulator stuff --- //
   pfGroup*          mSimTree;      // The simulator scene graph
   pfScene*          mRootWithSim;  // The root with the simulator added on to it
   pfDCS*            mHeadDCS;      // The DCS above the head
   pfDCS*            mWandDCS;      // The DCS above the wand
   std::string       mHeadModel;    // The head model file path
   std::string       mWandModel;    // The wand model file path

   // --- Singleton Stuff --- //
public:
   static vjPfDrawManager* instance()
   {
      if (_instance == NULL)
         _instance = new vjPfDrawManager();
      return _instance;
   }
protected:
   vjPfDrawManager()
   {;}
private:
   static vjPfDrawManager* _instance;
};


#endif
