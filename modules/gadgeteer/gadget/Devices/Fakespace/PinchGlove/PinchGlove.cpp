/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */

#include <string>
#include <sys/time.h>
#include <stdio.h>                      // need stdio for sprintf

#include <vjConfig.h>
#include <Input/vjGlove/fsPinchGlove.h> //fakespace pinch driver
#include <Input/vjGlove/vjPinchGlove.h> //vrjuggler pinch driver
#include <Kernel/vjKernel.h>


bool vjPinchGlove::mLookupInitialized = false;
float vjPinchGlove::mOnLookupTable[4][11];
float vjPinchGlove::mOffLookupTable[4][11];

bool vjPinchGlove::config(vjConfigChunk *c)           
{
   if(!vjGlove::config(c))
      return false;

   // init the lookup table if it hasn't been
    if (vjPinchGlove::mLookupInitialized == false)
      vjPinchGlove::mInitLookupTable();

    vjASSERT(myThread == NULL);      // This should have been set by vjInput(c)

    /*
    char* home_dir = c->getProperty("calDir").cstring();
    if (home_dir != NULL)
    {
        mCalDir = new char [strlen(home_dir) + 1];
        strcpy(mCalDir,home_dir);
    }
    */
      
    // already set in base interface.
    //std::string sPort = c->getProperty("port");
    std::string glove_pos_proxy = c->getProperty("glovePos");    // Get the name of the pos_proxy
    if(glove_pos_proxy == std::string(""))
    {
       vjDEBUG(vjDBG_INPUT_MGR,0) << "[vjPinch] ERROR: fsPinchGlove has no posProxy." << endl << vjDEBUG_FLUSH;
       return false;
    }

    // init glove proxy interface
    int proxy_index = vjKernel::instance()->getInputManager()->getProxyIndex(glove_pos_proxy);
    if(proxy_index != -1)
       mGlovePos[0] = vjKernel::instance()->getInputManager()->getPosProxy(proxy_index);
    else
       vjDEBUG(vjDBG_INPUT_MGR,0) << "[vjPinch] ERROR: fsPinchGlove::fsPinchGlove: Can't find posProxy." << endl << vjDEBUG_FLUSH << endl;

    mGlove = new fsPinchGlove();

    return true;
}

vjPinchGlove::~vjPinchGlove ()
{
   stopSampling();      // Stop the glove
   delete mGlove;       // Delete the glove
}

int vjPinchGlove::startSampling()
{
   vjDEBUG(vjDBG_INPUT_MGR, 0)<<"[vjPinch] Begin sampling\n"<<flush;
   
   if (myThread == NULL)
   {
      resetIndexes();

      // Create a new thread to handle the control
      vjDEBUG(vjDBG_INPUT_MGR, 0)<<"[vjPinch] Spawning control thread\n"<<flush;
      vjThreadMemberFunctor<vjPinchGlove>* memberFunctor =
         new vjThreadMemberFunctor<vjPinchGlove>(this, &vjPinchGlove::controlLoop, NULL);

      myThread = new vjThread(memberFunctor, 0);

      if (!myThread->valid())
      {
         return 0;
      }
      else
      {
         vjDEBUG(vjDBG_INPUT_MGR,1) << "[vjPinch] vjPinchGlove is active " << endl;
         active = 1;
         return 1;
      }
  }
  else
     return 0; // already sampling
}

void vjPinchGlove::controlLoop(void* nullParam)
{
   vjDEBUG(vjDBG_INPUT_MGR, 0)<<"[vjPinch] Entered control thread\n"<<flush;
   
   bool result = false;
   while (result == false)
	{
		vjDEBUG(vjDBG_INPUT_MGR, 0)<<"[vjPinch] Connecting to "<<sPort<<"...\n"<<flush;
      result = mGlove->connectToHardware( sPort );
      if (result == false)
      {
         vjDEBUG(vjDBG_INPUT_MGR,0) << "[vjPinch] ERROR: Can't open or it is already opened." << vjDEBUG_FLUSH;
	      usleep(14500);
      }
   }

   vjDEBUG(vjDBG_INPUT_MGR,0) << "[vjPinch] Successfully connected, Now sampling pinch data." << vjDEBUG_FLUSH;
	      
	while(1)
	{
		sample();
	}
}

//: Get the digital data for the given devNum
//  Returns digital 0 or 1, if devNum makes sense.<BR>
//  Returns -1 if function fails or if devNum is out of range.<BR>
//  NOTE: If devNum is out of range, function will fail, possibly issueing 
//  an error to a log or console - but will not ASSERT.<BR>   
int vjPinchGlove::getDigitalData(int devNum)
{
   // get the fakespace "gesture", it's a string like this "00000.00000"
   std::string gesture;
   mGlove->getSampledString( gesture );
      
   // make sure the passed value is within range.
   if (devNum >= 0 && devNum <= gesture.size())
   {
      // get the requested digital data from the "gesture" string
      char character[2];
      character[0] = gesture[devNum];
      character[1] = '\0';
      
      // convert the character to a number.
      // TODO: what to do if the fsPinchGlove ever gives us something 
      //       other than 0,1?
      int number = atoi( character ); //probably a better way to do this...
      return number;
   } 
   
   else
   {
      // DONT assert!  just notify system that there was a user error.
      // asserting could bring the system down, and that's bad.
      vjDEBUG(vjDBG_INPUT_MGR,0) << "[vjPinch] Assertion: application requested a digital ID out of range.  Valid range is [0.."<<gesture.size() <<"]\n"<< vjDEBUG_FLUSH;
   }
   
   // function failed
   return -1;
}
   
int vjPinchGlove::sample()
{
   // Tell the glove to resample
    mGlove->updateStringFromHardware();
	
    
    
    
    
    //*** old, from vjGesture stuff
    // Copy the data from the fsPinchGlove to myself.
    //copyDataFromGlove();//dies here...
	
    // Update the xform data
    //mTheData[0][progress].calcXforms();
    //mTheData[1][progress].calcXforms();
	
    //swapValidIndexes();
    //*** old 
    
	 return 1;
}

void vjPinchGlove::updateData()
{
vjGuard<vjMutex> updateGuard(lock);

   // Copy the valid data to the current data so that both are valid
   mTheData[0][current] = mTheData[0][valid];   // first hand
   mTheData[1][current] = mTheData[1][valid];   // other hand


   // swap the indicies for the pointers
   swapCurrentIndexes();

    return;
}

int vjPinchGlove::stopSampling()
{
   if (myThread != NULL)
   {
      myThread->kill();
      delete myThread;
      myThread = NULL;
      usleep(100);

      // XXX: there is no "close"
      //mGlove->Close();
      vjDEBUG(vjDBG_INPUT_MGR,1) << "[vjPinch] stopping vjPinchGlove.." << endl;
   }
   return 1;
}

void vjPinchGlove::mInitLookupTable()
{
    //  DIJ = Distal Interphalangeal Joint  --- finger tip
    //  PIJ = Proximal "              "     --- Middle joint
    //  MPJ = Metacarpo "             "     --- closest to hand
    //  ABDUCT = spread of fingers
    //
    // YAW and PITCH apply only to WRIST

    // LPINKY = 0, LRING = 1, LMIDDLE = 2, LINDEX = 3, LTHUMB = 4, : RTHUMB = 6, RINDEX = 7, RMIDDLE = 8, RRING = 9, RPINKY = 10;

    // Initialize the lookup tables
    mOnLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::LPINKY ] = -15;
    mOnLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::LPINKY ] = -45;
    mOnLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::LPINKY ] = -45;
    mOnLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::LPINKY ] = 0;

    mOnLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::LRING ] = -15;
    mOnLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::LRING ] = -45;
    mOnLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::LRING ] = -45;
    mOnLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::LRING ] = 0;

    mOnLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::LMIDDLE ] = -15;
    mOnLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::LMIDDLE ] = -45;
    mOnLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::LMIDDLE ] = -45;
    mOnLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::LMIDDLE ] = 0;

    mOnLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::LINDEX ] = -15;
    mOnLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::LINDEX ] = -45;
    mOnLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::LINDEX ] = -45;
    mOnLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::LINDEX ] = 0;

    mOnLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::LTHUMB ] = -15;
    mOnLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::LTHUMB ] = -45;
    mOnLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::LTHUMB ] = -45;
    mOnLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::LTHUMB ] = 0;

    // Initialize the lookup tables
    mOffLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::RPINKY ] = -15;
    mOffLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::RPINKY ] = -45;
    mOffLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::RPINKY ] = -45;
    mOffLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::RPINKY ] = 0;

    mOffLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::RRING ] = -15;
    mOffLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::RRING ] = -45;
    mOffLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::RRING ] = -45;
    mOffLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::RRING ] = 0;

    mOffLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::RMIDDLE ] = -15;
    mOffLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::RMIDDLE ] = -45;
    mOffLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::RMIDDLE ] = -45;
    mOffLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::RMIDDLE ] = 0;

    mOffLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::RINDEX ] = -15;
    mOffLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::RINDEX ] = -45;
    mOffLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::RINDEX ] = -45;
    mOffLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::RINDEX ] = 0;

    mOffLookupTable[ vjGloveData::DIJ ][ fsPinchGlove::RTHUMB ] = -15;
    mOffLookupTable[ vjGloveData::PIJ ][ fsPinchGlove::RTHUMB ] = -45;
    mOffLookupTable[ vjGloveData::MPJ ][ fsPinchGlove::RTHUMB ] = -45;
    mOffLookupTable[ vjGloveData::ABDUCT ][ fsPinchGlove::RTHUMB ] = 0;
	
	vjPinchGlove::mLookupInitialized = true;
}

void vjPinchGlove::copyDataFromGlove()
{
    std::string gesture;
    // the the fakespace "gesture", it's a string like this "00000.00000"
    mGlove->getSampledString( gesture );

    const int RIGHT_HAND = 0;
    const int LEFT_HAND = 1;

    // use the digital data to lookup the expected angles for each joint.

    // Right Pinky
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::MPJ]
   = (gesture[fsPinchGlove::RPINKY] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::RPINKY] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::RPINKY];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::PIJ]
   = (gesture[fsPinchGlove::RPINKY] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::RPINKY] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::RPINKY];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::DIJ]
   = (gesture[fsPinchGlove::RPINKY] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::RPINKY] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::RPINKY];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
   = (gesture[fsPinchGlove::RPINKY] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RPINKY] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RPINKY];


    // Right Ring
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::RING][vjGloveData::MPJ]
   = (gesture[fsPinchGlove::RRING] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::RRING] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::RRING];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::RING][vjGloveData::PIJ]
   = (gesture[fsPinchGlove::RRING] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::RRING] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::RRING];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::RING][vjGloveData::DIJ]
   = (gesture[fsPinchGlove::RRING] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::RRING] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::RRING];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
   = (gesture[fsPinchGlove::RRING] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RRING] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RRING];


    // Right Middle
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::MPJ]
   = (gesture[fsPinchGlove::RMIDDLE] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::RMIDDLE] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::RMIDDLE];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::PIJ]
   = (gesture[fsPinchGlove::RMIDDLE] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::RMIDDLE] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::RMIDDLE];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::DIJ]
   = (gesture[fsPinchGlove::RMIDDLE] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::RMIDDLE] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::RMIDDLE];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
   = (gesture[fsPinchGlove::RMIDDLE] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RMIDDLE] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RMIDDLE];

    // Right Index
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::MPJ]
   = (gesture[fsPinchGlove::RINDEX] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::RINDEX] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::RINDEX];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::PIJ]
   = (gesture[fsPinchGlove::RINDEX] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::RINDEX] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::RINDEX];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::DIJ]
   = (gesture[fsPinchGlove::RINDEX] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::RINDEX] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::RINDEX];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
   = (gesture[fsPinchGlove::RINDEX] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RINDEX] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RINDEX];

    // Right Thumb
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::MPJ]
   = (gesture[fsPinchGlove::RTHUMB] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::RTHUMB] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::RTHUMB];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::PIJ]
   = (gesture[fsPinchGlove::RTHUMB] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::RTHUMB] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::RTHUMB];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::DIJ]
   = (gesture[fsPinchGlove::RTHUMB] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::RTHUMB] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::RTHUMB];
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
   = (gesture[fsPinchGlove::RTHUMB] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RTHUMB] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::RTHUMB];


    // Left Pinky
    mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::MPJ]
   = (gesture[fsPinchGlove::LPINKY] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::LPINKY] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::LPINKY];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::PIJ]
   = (gesture[fsPinchGlove::LPINKY] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::LPINKY] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::LPINKY];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::DIJ]
   = (gesture[fsPinchGlove::LPINKY] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::LPINKY] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::LPINKY];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
   = (gesture[fsPinchGlove::LPINKY] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LPINKY] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LPINKY];

    // Left Ring
    mTheData[LEFT_HAND][progress].angles[vjGloveData::RING][vjGloveData::MPJ]
   = (gesture[fsPinchGlove::LRING] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::LRING] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::LRING];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::RING][vjGloveData::PIJ]
   = (gesture[fsPinchGlove::LRING] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::LRING] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::LRING];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::RING][vjGloveData::DIJ]
   = (gesture[fsPinchGlove::LRING] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::LRING] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::LRING];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
   = (gesture[fsPinchGlove::LRING] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LRING] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LRING];

    // Left Middle
    mTheData[LEFT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::MPJ]
   = (gesture[fsPinchGlove::LMIDDLE] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::LMIDDLE] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::LMIDDLE];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::PIJ]
   = (gesture[fsPinchGlove::LMIDDLE] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::LMIDDLE] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::LMIDDLE];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::DIJ]
   = (gesture[fsPinchGlove::LMIDDLE] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::LMIDDLE] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::LMIDDLE];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
   = (gesture[fsPinchGlove::LMIDDLE] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LMIDDLE] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LMIDDLE];

    // Left Index
    mTheData[LEFT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::MPJ]
   = (gesture[fsPinchGlove::LINDEX] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::LINDEX] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::LINDEX];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::PIJ]
   = (gesture[fsPinchGlove::LINDEX] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::LINDEX] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::LINDEX];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::DIJ]
   = (gesture[fsPinchGlove::LINDEX] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::LINDEX] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::LINDEX];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
   = (gesture[fsPinchGlove::LINDEX] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LINDEX] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LINDEX];

    // Left Thumb
    mTheData[LEFT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::MPJ]
   = (gesture[fsPinchGlove::LTHUMB] == '1') ? mOnLookupTable[vjGloveData::MPJ][fsPinchGlove::LTHUMB] : mOffLookupTable[vjGloveData::MPJ][fsPinchGlove::LTHUMB];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::PIJ]
   = (gesture[fsPinchGlove::LTHUMB] == '1') ? mOnLookupTable[vjGloveData::PIJ][fsPinchGlove::LTHUMB] : mOffLookupTable[vjGloveData::PIJ][fsPinchGlove::LTHUMB];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::DIJ]
   = (gesture[fsPinchGlove::LTHUMB] == '1') ? mOnLookupTable[vjGloveData::DIJ][fsPinchGlove::LTHUMB] : mOffLookupTable[vjGloveData::DIJ][fsPinchGlove::LTHUMB];
    mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT]
   = (gesture[fsPinchGlove::LTHUMB] == '1') ? mOnLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LTHUMB] : mOffLookupTable[vjGloveData::ABDUCT][fsPinchGlove::LTHUMB];

    // Left and Right Wrist
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::YAW] = 0;
    mTheData[RIGHT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::PITCH] = 0;
    mTheData[LEFT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::YAW] = 0;
    mTheData[LEFT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::PITCH] = 0;
}
