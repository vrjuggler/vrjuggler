/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


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
   vjDisplay() : mUser(NULL), mDisplayChunk(NULL)
   {
      _xo = _yo = _xs = _ys = -1;
      mType = vjDisplay::UNDEFINED;
      mDisplayChunk = NULL;
      mPipe = vjDisplay::NONE;
   }

   virtual ~vjDisplay()
   {;}

   enum DisplayType { UNDEFINED, SURFACE, SIM};             // What type of display is it
   enum DisplayView { NONE=0, LEFT_EYE=1, RIGHT_EYE=2, STEREO=3 };      // For referring to which eye(s) to draw

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
   { return (mView == STEREO); }

   // Which view are we supposed to render
   DisplayView getView()
   { return mView; }

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

   //: Get the config chunk that configured this display
   vjConfigChunk* getConfigChunk()
   { return mDisplayChunk; }

   //: Get the user associated with this display
   vjUser*  getUser()
   { return mUser;}

   virtual ostream& outStream(ostream& out);
   friend ostream& operator<<(ostream& out, vjDisplay& disp);

protected:
   vjUser*           mUser;         //: The user being rendered by this window

protected:
   DisplayType mType;                  //: The type of display
   std::string mName;                  //: Name of the display
   int         _xo, _yo, _xs, _ys;     //: X and Y origin and size of the view
   bool        mBorder;                //: Should we have a border
   int         mPipe;                  //: Hardware pipe. Index of the rendering hardware
   bool        mActive;                //: Is the display active or not
   DisplayView  mView;                 //: Which buffer(s) to display (left, right, stereo)

   vjConfigChunk* mDisplayChunk;        //: The chunk data for this display
};

//ostream& operator<<(ostream& out, vjDisplay* disp);

#endif
