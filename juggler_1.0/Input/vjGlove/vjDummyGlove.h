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


#ifndef _VJ_DUMMY_GLOVE_H_
#define _VJ_DUMMY_GLOVE_H_

#include <vjConfig.h>
#include <Input/vjGlove/vjGlove.h>

//: A dummy glove for the system
// This class is used only when there is a proxy and it must be
// stupified.
//
// It returns the data in it's default init.
// i.e. 0 angles, and ident matrices
//!PUBLIC_API:
class vjDummyGlove : public vjGlove
{
public:
   //: Construct using chunk
   vjDummyGlove() : vjGlove()
   {
      active = 1;
      resetIndexes();
      mTheData[0][current].calcXforms();
   }

   //: Destroy the glove
   ~vjDummyGlove ()
   {;}

   virtual char* getDeviceName() { return "vjDummyGlove";}

   virtual int startSampling() { return 1; }
   virtual int stopSampling() { return 1; }
   virtual int sample() { return 1; }
   virtual void updateData () {}

   void setGlovePosProxy(vjPosProxy* posProxy)
   { mGlovePos[0] = posProxy; }

};

#endif	/* _VJ_CYBER_GLOVE_H_ */
