#ifndef _VJ_DISPLAY_H_
#define _VJ_DISPLAY_H_

#include <vjConfig.h>

#include <Math/vjVec3.h>
#include <Input/vjPosition/vjPosition.h>
#include <Input/InputManager/vjPosInterface.h>
#include <Kernel/vjUser.h>

    // Config stuff
#include <Config/vjConfigChunkDB.h>

//---------------------------------------------------------------------
//: Base class for Display windows.
//
// Base class for all window system independant data about a display.
//
// Stores location of window and projection data used.
//
// @author Allen Bierbaum
//  Date: 9-8-97
//-----------------------------------------------------------------------
class vjDisplay
{
public:
   vjDisplay() : mUser(NULL)
   {
      _xo = _yo = _xs = _ys = -1;
   }

   enum DisplayType { SURFACE, SIM};      // What type of display is it

public:
      //: Takes a display chunk and configures the display based one it.
      //! PRE: chunk is a valid chunk
      //! POST: display is configured
      //+       If there is an error is the specified config, we output error
      //+       and "fix" the error.
      //! NOTE: All derived display classes MUST call this function
      //+       after doing local configuration.
   virtual void config(vjConfigChunk* chunk);

   //: Updates the projection data for this display
   // Uses the data for the head position for this window
   virtual void updateProjections() = 0;

public:
   DisplayType getType()
   { return mType;}

   bool isSimulator()
   { return (mType == SIM); }

   bool isSurface()
   { return (mType == SURFACE); }

   bool isActive()
   { return mActive; }

   void setName(std::string name)
   { mName = name; }

   //: Get the name of the display
   std::string getName()
   { return mName;}

   bool  shouldDrawBorder()
   { return mBorder;}

   //!NOTE: If we are in simulator, we can not be in stereo
   bool inStereo()
   { return mStereo; }

   void setOriginAndSize(int xo, int yo, int xs, int ys)
   { _xo = xo; _yo = yo; _xs = xs; _ys = ys;}
   void getOriginAndSize(int& xo, int& yo, int& xs, int& ys)
   {
      vjASSERT(xo != -1);     // Make sure we have been configured
      xo = _xo; yo = _yo; xs = _xs; ys = _ys;
   }

   void setPipe(int pipe)
   { mPipe = pipe; }
   int getPipe()
   { return mPipe; }

   vjConfigChunk* getConfigChunk()
   { return displayChunk; }

   vjUser*  getUser()
   { return mUser;}

   friend ostream& operator<<(ostream& out, vjDisplay& disp);

// <<<<<<< vjDisplay.h
// public:
//    /// Defines the projection for this window. Ex. RIGHT, LEFT, FRONT
//    vjProjection*   leftProj;              // Left eye
//    vjProjection*   rightProj;             // Right eye
//    vjProjection*   cameraProj;            // Camera projection. (For sim, etc.)

//    vjSimulator*      mSim;                // Simulator. if we are one.
//    //vjPosInterface    mHeadInterface;      //: The proxy interface for the head
//     vjTimeStamp*       mHeadUpdateTime;        //: When head pos was updated.
//    vjUser*           mUser;               //: The user being rendered by this window

// =======
protected:
   vjUser*           mUser;         //: The user being rendered by this window

protected:
   DisplayType mType;                  //: The type of display
   std::string mName;                  //: Name of the display
   int         _xo, _yo, _xs, _ys;     //: X and Y origin and size of the view
   bool        mBorder;                //: Should we have a border
   int         mPipe;                  //: Hardware pipe. Index of the rendering hardware
   bool        mStereo;                //: Do we want stereo
   bool        mActive;                //: Is the display active or not

   vjConfigChunk* displayChunk;        //: The chunk data for this display
};

ostream& operator<<(ostream& out,  vjDisplay& disp);

#endif
