/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
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


#ifndef _VJ_PINCH_GLOVE_H_
#define _VJ_PINCH_GLOVE_H_

#include <vjConfig.h>
#include <Input/vjGlove/vjGlove.h>
#include <Input/vjGlove/fsPinchGlove.h>

//!PUBLIC_API:
class vjPinchGlove : public vjGlove
{
public:
   //: Construct
   vjPinchGlove() : mGlove(NULL), mControlThread(NULL), mCalDir(NULL)
   {;}

   //: Destroy the glove
   ~vjPinchGlove ();

   virtual bool config(vjConfigChunk* c);

   virtual char* getDeviceName() { return "vjPinchGlove";}
   static string getChunkType() { return string("PinchGlove");}

   virtual int startSampling();
   virtual int stopSampling();
   virtual int sample();
   virtual void updateData ();

protected:
	//: The main control loop for the object
   void controlLoop(void* nullParam);

   void copyDataFromGlove();

protected:
   vjThread*         mControlThread;      // The thread of control for the object
   fsPinchGlove*     mGlove;              // The actual glove

   char*             mCalDir;             // Calibration file directory

private:
    //lookup tables for the finger angles.
   static float	    mOnLookupTable[4][11];
   static float	    mOffLookupTable[4][11];
   static bool	    mLookupInitialized; // false by default, set true when lookup table has been initialized.
   static void	    mInitLookupTable();
};

#endif	/* _VJ_PINCH_GLOVE_H_ */
