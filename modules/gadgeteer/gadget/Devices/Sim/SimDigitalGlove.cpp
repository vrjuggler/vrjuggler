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

#include <vrj/vjConfig.h>
#include <vpr/Util/Assert.h>
#include <vrj/Input/Devices/Sim/SimDigitalGlove.h>
#include <vrj/Config/ConfigChunk.h>

namespace vrj
{
   
//: Default Constructor
SimDigitalGlove::SimDigitalGlove() : Digital(), SimInput(), Glove()
{
   //vjDEBUG(vjDBG_ALL,3)<<"*** SimDigitalGlove::SimDigitalGlove()\n"<< vjDEBUG_FLUSH;
}

//: Destructor
SimDigitalGlove::~SimDigitalGlove()
{
  // vjDEBUG(vjDBG_ALL,3)<<"*** SimDigitalGlove::~SimDigitalGlove()\n"<< vjDEBUG_FLUSH;
}

bool SimDigitalGlove::config( ConfigChunk* chunk )
{
   //vjDEBUG(vjDBG_ALL,3)<<"*** SimDigitalGlove::config\n"<< vjDEBUG_FLUSH;
   if(! (Input::config(chunk) && Digital::config(chunk) && SimInput::config(chunk) && Glove::config(chunk)))
      return false;

   //mCurGesture = 0;     // We are in no gesture yet

   //if ((!Digital::config( chunk )) || (!SimInput::config( chunk )))
   //   return false;

   std::vector< VarValue* > key_list = chunk->getAllProperties( "keyPairs" );
   mSimKeys = readKeyList( key_list );

   int num_pairs = mSimKeys.size();
   mDigitalData = std::vector< int >( num_pairs, 0 );      // Initialize to all zeros

   ////////////////  left posProxy
   vprASSERT( VJ_MAX_GLOVE_DEVS >= 2 );
   const int LEFT_INDEX = 0, RIGHT_INDEX = 1;
   if (LEFT_INDEX < VJ_MAX_GLOVE_DEVS)
   {
      std::string glove_pos_proxy = chunk->getProperty( "glovePos" );    // Get the name of the pos_proxy
      if (glove_pos_proxy == std::string(""))
      {
         vjDEBUG( vjDBG_INPUT_MGR, 0 )
            << "[ERROR]: SimPinchglove has no posProxy, config fails."
            << std::endl << vjDEBUG_FLUSH;
         return false;
      }

      // init glove pos proxy interface
      /*
      int proxy_index = Kernel::instance()->getInputManager()->getProxyIndex( glove_pos_proxy );
      if (proxy_index != -1)
         mGlovePos[LEFT_INDEX] = Kernel::instance()->getInputManager()->getPosProxy( proxy_index );
      else
         vjDEBUG( vjDBG_INPUT_MGR, 0 )
            << "[ERROR]: SimPinchglove::config(): Can't find posProxy, config fails."
            << std::endl << std::endl << vjDEBUG_FLUSH;
      */
   }


   ////////////////  right posProxy

   if (RIGHT_INDEX < VJ_MAX_GLOVE_DEVS)
   {
      std::string glove_pos_proxy = chunk->getProperty( "rightGlovePos" );    // Get the name of the pos_proxy
      if (glove_pos_proxy == std::string(""))
      {
         vjDEBUG( vjDBG_INPUT_MGR, 0 )
            << "[ERROR]: SimPinchglove has no rightPosProxy, config fails."
            << std::endl << vjDEBUG_FLUSH;
         return false;
      }

      // init right glove pos proxy interface
      /*
      int proxy_index = Kernel::instance()->getInputManager()->getProxyIndex( glove_pos_proxy );
      if (proxy_index != -1)
         mGlovePos[RIGHT_INDEX] = Kernel::instance()->getInputManager()->getPosProxy( proxy_index );
      else
         vjDEBUG( vjDBG_INPUT_MGR, 0 )
            << "[ERROR]: SimPinchglove::config(): Can't find posProxy, config fails."
            << std::endl << std::endl << vjDEBUG_FLUSH;
            */
   }


   return true;
}

// Updates the state of the digital data vector <br>
//
// NOTE: Digital is on when key is held down
//+     When key is release, digital goes to off state
void SimDigitalGlove::updateData()
{
   // this unlocks when this object is destructed (upon return of the function)
   vpr::Guard<vpr::Mutex> updateGuard(lock);

   // Copy the valid data to the current data so that both are valid
   mTheData[0][current] = mTheData[0][valid];   // first hand
   mTheData[1][current] = mTheData[1][valid];   // other hand

   // -- Update digital data --- //
   for (unsigned int i = 0; i < mSimKeys.size(); i++)
   {
      if (checkKeyPair( mSimKeys[i] ))             // If keys pressed
         mDigitalData[i] = 1;
      else
         mDigitalData[i] = 0;
   }

   // The data is triple buffered in mTheData
   //  The data slots are as follows:
   //
   // [writable], [temp/valid], [readable]    <-- what they mean
   //  progress,   valid,        current      <-- what you type
   //
   // You write to progress, whenever you swap, the data moves to valid,
   //  then current.

   // this function looks at mDigitalData, and sets angles in mTheData.
   // This updates the writable buffer of mTheData (called progress)
   this->updateFingerAngles();

   // Update the xform data for the writable buffer (called progress)
   mTheData[0][progress].calcXforms();
   mTheData[1][progress].calcXforms();

   // swap the indicies for the pointers.
   // This swaps the temp and readable buffers (called 'valid' and 'current')
   swapCurrentIndexes();

   //vjDEBUG(vjDBG_ALL,0)<<mTheData[0][current].outputAngles(cout)<<vjDEBUG_FLUSH;
   //vjDEBUG(vjDBG_ALL,0)<<mTheData[1][current].outputAngles(cout)<<vjDEBUG_FLUSH;


   //TODO:  how does the angles get turned into a gesture ID????
   return;
}

//TODO: move this function up the hierarchy, since PinchGlove also has this one.
//NOTE: this function *is* slightly different...
void SimDigitalGlove::updateFingerAngles()
{
    const int LEFT_HAND = 0;
    const int RIGHT_HAND = 1;

    vprASSERT( progress < 3 && progress >= 0 );
    vprASSERT( LEFT_HAND < VJ_MAX_GLOVE_DEVS );
    vprASSERT( RIGHT_HAND < VJ_MAX_GLOVE_DEVS );

    // use the digital data set the angles for each joint.
    mLeftHand.setFingers( mDigitalData[LPINKY] == 1,
                     mDigitalData[LRING] == 1,
                     mDigitalData[LMIDDLE] == 1,
                     mDigitalData[LINDEX] == 1,
                     mDigitalData[LTHUMB] == 1 );
    mRightHand.setFingers( mDigitalData[RPINKY] == 1,
                     mDigitalData[RRING] == 1,
                     mDigitalData[RMIDDLE] == 1,
                     mDigitalData[RINDEX] == 1,
                     mDigitalData[RTHUMB] == 1 );

    //Now, set the ugly ambiguously named array, mTheData:

    // if that assert failed, then at least the code will still run...
    if ( LEFT_HAND < VJ_MAX_GLOVE_DEVS )
    {
       //vjDEBUG(vjDBG_ALL,0)<<"Lpinky:"<<mLeftHand.pinky().mpj()<<","<<mLeftHand.pinky().pij()<<","<<mLeftHand.pinky().dij()<<","<<mLeftHand.pinky().abduct()<<"\n"<<vjDEBUG_FLUSH;
       // Left Pinky
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::MPJ] = mLeftHand.pinky().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::PIJ] = mLeftHand.pinky().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::DIJ] = mLeftHand.pinky().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::PINKY][GloveData::ABDUCT] = mLeftHand.pinky().abduct();

       // Left Ring
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::MPJ] = mLeftHand.ring().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::PIJ] = mLeftHand.ring().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::DIJ] = mLeftHand.ring().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::RING][GloveData::ABDUCT] = mLeftHand.ring().abduct();

       // Left Middle
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::MPJ] = mLeftHand.middle().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::PIJ] = mLeftHand.middle().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::DIJ] = mLeftHand.middle().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::MIDDLE][GloveData::ABDUCT] = mLeftHand.middle().abduct();

       // Left Index
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::MPJ] = mLeftHand.index().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::PIJ] = mLeftHand.index().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::DIJ] = mLeftHand.index().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::INDEX][GloveData::ABDUCT] = mLeftHand.index().abduct();

       // Left Thumb
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::MPJ] = mLeftHand.thumb().mpj();
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::PIJ] = mLeftHand.thumb().pij();
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::DIJ] = mLeftHand.thumb().dij();
       mTheData[LEFT_HAND][progress].angles[GloveData::THUMB][GloveData::ABDUCT] = mLeftHand.thumb().abduct();

       // Left Wrist
       mTheData[LEFT_HAND][progress].angles[GloveData::WRIST][GloveData::YAW] = mLeftHand.yaw();
       mTheData[LEFT_HAND][progress].angles[GloveData::WRIST][GloveData::PITCH] = mLeftHand.pitch();
    }

    // if that assert failed, then at least the code will still run...
    if ( RIGHT_HAND < VJ_MAX_GLOVE_DEVS )
    {
       //vjDEBUG(vjDBG_ALL,0)<<"Rpinky:"<<mRightHand.pinky().mpj()<<","<<mRightHand.pinky().pij()<<","<<mRightHand.pinky().dij()<<","<<mRightHand.pinky().abduct()<<"   "<<vjDEBUG_FLUSH;
       // Right Pinky
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::MPJ] = mRightHand.pinky().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::PIJ] = mRightHand.pinky().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::DIJ] = mRightHand.pinky().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::PINKY][GloveData::ABDUCT] = mRightHand.pinky().abduct();

       // Right Ring
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::MPJ] = mRightHand.ring().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::PIJ] = mRightHand.ring().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::DIJ] = mRightHand.ring().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::RING][GloveData::ABDUCT] = mRightHand.ring().abduct();

       // Right Middle
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::MPJ] = mRightHand.middle().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::PIJ] = mRightHand.middle().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::DIJ] = mRightHand.middle().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::MIDDLE][GloveData::ABDUCT] = mRightHand.middle().abduct();

       // Right Index
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::MPJ] = mRightHand.index().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::PIJ] = mRightHand.index().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::DIJ] = mRightHand.index().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::INDEX][GloveData::ABDUCT] = mRightHand.index().abduct();

       // Right Thumb
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::MPJ] = mRightHand.thumb().mpj();
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::PIJ] = mRightHand.thumb().pij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::DIJ] = mRightHand.thumb().dij();
       mTheData[RIGHT_HAND][progress].angles[GloveData::THUMB][GloveData::ABDUCT] = mRightHand.thumb().abduct();

       // Right Wrist
       mTheData[RIGHT_HAND][progress].angles[GloveData::WRIST][GloveData::YAW] = mRightHand.yaw();
       mTheData[RIGHT_HAND][progress].angles[GloveData::WRIST][GloveData::PITCH] = mRightHand.pitch();
    }

    //vjDEBUG(vjDBG_ALL,0)<<"out\n"<<std::flush<<vjDEBUG_FLUSH;
}

/*
//: Get the current gesture.
//! RETURNS: id of current gesture
int SimDigitalGlove::getGesture()
{ return mCurGesture; }

//: Load trained data for the gesture object
// Loads the file for trained data
void SimDigitalGlove::loadTrainedFile(std::string fileName)
{
   ifstream inFile(fileName.c_str());

   if (inFile)
   {
      this->loadFileHeader(inFile);
      inFile.close();                     // Close the file
   }
   else
   {
      vjDEBUG(vjDBG_INPUT_MGR,0)
         << "vjSimGloveGesture:: Can't load trained file: " << fileName.c_str()
         << std::endl << vjDEBUG_FLUSH;
   }
}
*/

};
