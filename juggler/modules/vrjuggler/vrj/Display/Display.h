#ifndef _VJ_DISPLAY_
#define _VJ_DISPLAY_

class vjProjection;
#include <vjConfig.h>
#include <iostream.h>
class vjDisplayManager;		    // Because prev include includes us

#include <Math/vjVec3.h>
#include <Input/vjPosition/vjPosition.h>
#include <Input/InputManager/vjPosInterface.h>
class vjSimulator;

    // Config stuff
#include <Config/vjConfigChunkDB.h>

//---------------------------------------------------------------------
//: Base class for Display windows.
//
//	Responsible for encapsulating all display window information.
//  Since this captures all machine/OS/Window System specific stuff,
//  no API specific details should be added to this area.  API specific
//  window information should stay in the DrawManager which is API
//  specific.
//
// Display also knows what type of display it is: Normal, Simulator, ...
//
// @author Allen Bierbaum
//  Date: 9-8-97
//-----------------------------------------------------------------------
class vjDisplay
{
public:
   vjDisplay();

   enum DisplayType { PROJ, SIM};      // What type of display is it

public:
      //: Takes a display chunk and configures the display based one it.
      //! PRE: chunk is a valid chunk
      //! POST: display is configured
      //+       If there is an error is the specified config, we output error
      //+       and "fix" the error.
      //! NOTE: All derived display classes MUST call this function
      //+       after doing local configuration.
   void config(vjConfigChunk* chunk);

public:
   DisplayType getType()
   { return mType;}

   bool isSimulator()
   { return (mType == SIM); }

   void setName(const char* name)
   {
      if (name != NULL)
         strncpy(mName, name, 99);
   }

   char* getName()
   { return mName;}

   bool  drawBorder()
   { return mBorder;}

   //!NOTE: If we are in simulator, we can not be in stereo
   bool inStereo()
   { return (mStereo && (mType != SIM)); }

   void setOriginAndSize(int xo, int yo, int xs, int ys)
   { _xo = xo; _yo = yo; _xs = xs; _ys = ys;}
   void originAndSize(int& xo, int& yo, int& xs, int& ys)
   { xo = _xo; yo = _yo; xs = _xs; ys = _ys;}

   void setPipe(int pipe)
   { mPipe = pipe; }
   int pipe()
   { return mPipe; }

   vjConfigChunk* configChunk()
   { return displayChunk; }

      //: Updates the projection data for this display
      //    Normally just calls the projection class
      // Uses the data for the head position for this window
   void updateProjections();


   friend ostream& operator<<(ostream& out, vjDisplay& disp);

public:
   /// Defines the projection for this window. Ex. RIGHT, LEFT, FRONT
   vjProjection*   leftProj;              // Left eye
   vjProjection*   rightProj;             // Right eye
   vjProjection*   cameraProj;            // Camera projection. (For sim, etc.)

   vjSimulator*      mSim;                // Simulator. if we are one.
   vjPosInterface    mHeadInterface;      //: The proxy interface for the head


protected:
   DisplayType mType;                  //: The type of display
   char        mName[100];             //: Name of the display
   int         _xo, _yo, _xs, _ys;     //: X and Y origin and size of the view
   bool        mBorder;                //: Should we have a border
   int         mPipe;                  //: Hardware pipe. Index of the rendering hardware
   bool        mStereo;                //: Do we want stereo

   vjConfigChunk* displayChunk;        //: The chunk data for this display

   //vjDisplayManager* dispMgr;       //: My parent DisplayManager.  Needed to get headPostion on update.
};

ostream& operator<<(ostream& out,  vjDisplay& disp);

#endif
