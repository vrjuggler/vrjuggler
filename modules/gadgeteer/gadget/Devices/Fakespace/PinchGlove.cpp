/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
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
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#include <vrj/vrjConfig.h>

#include <string>
#include <sys/time.h>
#include <stdio.h>                      /* need stdio for sprintf */
#include <vpr/System.h>

#include <vrj/Input/Devices/Fakespace/PinchGloveStandalone.h> /* fakespace pinch driver */
#include <vrj/Input/Devices/Fakespace/PinchGlove.h> /* vrjuggler pinch driver */
//#include <vrj/Kernel/Kernel.h>
#include <vrj/Config/ConfigChunk.h>


namespace vrj
{

bool PinchGlove::config(ConfigChunk *c)
{
   if(! (Input::config(c) && Glove::config(c) ))
      return false;

    vprASSERT(myThread == NULL);      // This should have been set by Input(c)

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
       vjDEBUG(vjDBG_INPUT_MGR,0)
          << "[Pinch] ERROR: PinchGloveStandalone has no posProxy." << std::endl
          << vjDEBUG_FLUSH;
       return false;
    }

    // init glove proxy interface
    /*
    int proxy_index = Kernel::instance()->getInputManager()->getProxyIndex(glove_pos_proxy);
    if(proxy_index != -1)
       mGlovePos[0] = Kernel::instance()->getInputManager()->getPosProxy(proxy_index);
    else
       vjDEBUG(vjDBG_INPUT_MGR,0)
          << "[Pinch] ERROR: PinchGloveStandalone::PinchGloveStandalone: Can't find posProxy."
          << std::endl << std::endl << vjDEBUG_FLUSH;
          */

    mGlove = new PinchGloveStandalone();

    return true;
}

PinchGlove::~PinchGlove ()
{
   stopSampling();      // Stop the glove
   delete mGlove;       // Delete the glove
}

int PinchGlove::startSampling()
{
   vjDEBUG(vjDBG_INPUT_MGR, 0) << "[Pinch] Begin sampling\n"
                               << vjDEBUG_FLUSH;

   if (myThread == NULL)
   {
      resetIndexes();

      // Create a new thread to handle the control
      vjDEBUG(vjDBG_INPUT_MGR, 0) << "[Pinch] Spawning control thread\n"
                                  << vjDEBUG_FLUSH;
      vpr::ThreadMemberFunctor<PinchGlove>* memberFunctor =
         new vpr::ThreadMemberFunctor<PinchGlove>(this, &PinchGlove::controlLoop, NULL);

      myThread = new vpr::Thread(memberFunctor);

      if (!myThread->valid())
      {
         return 0;
      }
      else
      {
         vjDEBUG(vjDBG_INPUT_MGR,1) << "[Pinch] PinchGlove is active "
                                    << std::endl << vjDEBUG_FLUSH;
         active = 1;
         return 1;
      }
  }
  else
     return 0; // already sampling
}

void PinchGlove::controlLoop(void* nullParam)
{
   vjDEBUG(vjDBG_INPUT_MGR, 0) << "[Pinch] Entered control thread\n"
                               << vjDEBUG_FLUSH;

   bool result = false;
   while (result == false)
   {
      vjDEBUG(vjDBG_INPUT_MGR, 0) << "[Pinch] Connecting to "
                                            << sPort << "...\n"
                                            << vjDEBUG_FLUSH;
      result = mGlove->connectToHardware( sPort );
      if (result == false)
      {
         vjDEBUG(vjDBG_INPUT_MGR,0) << "[Pinch] ERROR: Can't open or it is already opened." << vjDEBUG_FLUSH;
         vpr::System::usleep(14500);
      }
   }

   vjDEBUG(vjDBG_INPUT_MGR,0) << "[Pinch] Successfully connected, Now sampling pinch data." << vjDEBUG_FLUSH;

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
int PinchGlove::getDigitalData(int devNum)
{
   // get the fakespace "gesture", it's a string like this "00000.00000"
   std::string gesture;
   mGlove->getSampledString( gesture );

   // make sure the passed value is within range.
   if ((devNum >= 0) && ((unsigned)devNum <= gesture.size()))
   {
      // get the requested digital data from the "gesture" string
      char character[2];
      character[0] = gesture[devNum];
      character[1] = '\0';

      // convert the character to a number.
      // TODO: what to do if the PinchGloveStandalone ever gives us something
      //       other than 0,1?
      int number = atoi( character ); //probably a better way to do this...
      return number;
   }

   else
   {
      // DONT assert!  just notify system that there was a user error.
      // asserting could bring the system down, and that's bad.
      vjDEBUG(vjDBG_INPUT_MGR,0) << "[Pinch] Assertion: application requested a digital ID out of range.  Valid range is [0.."<<gesture.size() <<"]\n"<< vjDEBUG_FLUSH;
   }

   // function failed
   return -1;
}

int PinchGlove::sample()
{
    // Tell the glove to resample
    mGlove->updateStringFromHardware();

    updateFingerAngles();

    // Update the xform data
    mTheData[0][progress].calcXforms();
    mTheData[1][progress].calcXforms();

    swapValidIndexes();

    return 1;
}

void PinchGlove::updateData()
{
vpr::Guard<vpr::Mutex> updateGuard(lock);

   // Copy the valid data to the current data so that both are valid
   mTheData[0][current] = mTheData[0][valid];   // first hand
   mTheData[1][current] = mTheData[1][valid];   // other hand


   // swap the indicies for the pointers
   swapCurrentIndexes();

    return;
}

int PinchGlove::stopSampling()
{
   if (myThread != NULL)
   {
      myThread->kill();
      delete myThread;
      myThread = NULL;
      vpr::System::usleep(100);

      // XXX: there is no "close"
      //mGlove->Close();
      vjDEBUG(vjDBG_INPUT_MGR,1) << "[Pinch] stopping PinchGlove.."
                                 << std::endl << vjDEBUG_FLUSH;
   }
   return 1;
}


//TODO: move this function up the hierarchy, since PinchGlove also has this one.
void PinchGlove::updateFingerAngles()
{
    std::string gesture;
    // the the fakespace "gesture", it's a string like this "00000.00000"
    mGlove->getSampledString( gesture );

    const int RIGHT_HAND = 0;
    const int LEFT_HAND = 1;

    assert( progress < 3 && progress >= 0 );
    assert( LEFT_HAND < VJ_MAX_GLOVE_DEVS );
    assert( RIGHT_HAND < VJ_MAX_GLOVE_DEVS );

    // use the digital data set the angles for each joint.
    right.setFingers( gesture[PinchGloveStandalone::RPINKY] == '1',
                     gesture[PinchGloveStandalone::RRING] == '1',
                     gesture[PinchGloveStandalone::RMIDDLE] == '1',
                     gesture[PinchGloveStandalone::RINDEX] == '1',
                     gesture[PinchGloveStandalone::RTHUMB] == '1' );
    left.setFingers( gesture[PinchGloveStandalone::LPINKY] == '1',
                     gesture[PinchGloveStandalone::LRING] == '1',
                     gesture[PinchGloveStandalone::LMIDDLE] == '1',
                     gesture[PinchGloveStandalone::LINDEX] == '1',
                     gesture[PinchGloveStandalone::LTHUMB] == '1' );

    //Now, set the ugly ambiguously named array, mTheData:

    // if that assert failed, then at least the code will still run...
    if ( RIGHT_HAND >= VJ_MAX_GLOVE_DEVS )
    {
       // Right Pinky
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::MPJ] = right.pinky().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::PIJ] = right.pinky().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::DIJ] = right.pinky().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::ABDUCT] = right.pinky().abduct();

       // Right Ring
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::MPJ] = right.ring().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::PIJ] = right.ring().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::DIJ] = right.ring().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::ABDUCT] = right.ring().abduct();

       // Right Middle
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::MPJ] = right.middle().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::PIJ] = right.middle().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::DIJ] = right.middle().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::ABDUCT] = right.middle().abduct();

       // Right Index
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::MPJ] = right.index().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::PIJ] = right.index().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::DIJ] = right.index().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::ABDUCT] = right.index().abduct();

       // Right Thumb
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::MPJ] = right.thumb().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::PIJ] = right.thumb().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::DIJ] = right.thumb().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::ABDUCT] = right.thumb().abduct();

       // Right Wrist
       mTheData[RIGHT_HAND][progress].angles[GloveData::WRIST][GloveData::YAW] = right.yaw();
       mTheData[RIGHT_HAND][progress].angles[GloveData::WRIST][GloveData::PITCH] = right.pitch();
    }

    // if that assert failed, then at least the code will still run...
    if ( LEFT_HAND >= VJ_MAX_GLOVE_DEVS )
    {
       // Left Pinky
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::MPJ] = left.pinky().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::PIJ] = left.pinky().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::DIJ] = left.pinky().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::ABDUCT] = left.pinky().abduct();

       // Left Ring
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::MPJ] = left.ring().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::PIJ] = left.ring().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::DIJ] = left.ring().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::ABDUCT] = left.ring().abduct();

       // Left Middle
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::MPJ] = left.middle().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::PIJ] = left.middle().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::DIJ] = left.middle().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::ABDUCT] = left.middle().abduct();

       // Left Index
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::MPJ] = left.index().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::PIJ] = left.index().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::DIJ] = left.index().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::ABDUCT] = left.index().abduct();

       // Left Thumb
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::MPJ] = left.thumb().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::PIJ] = left.thumb().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::DIJ] = left.thumb().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::ABDUCT] = left.thumb().abduct();

       // Left Wrist
       mTheData[LEFT_HAND][progress].angles[GloveData::WRIST][GloveData::YAW] = left.yaw();
       mTheData[LEFT_HAND][progress].angles[GloveData::WRIST][GloveData::PITCH] = left.pitch();
    }
}

};
