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


#include <vjConfig.h>
#include <Kernel/vjAssert.h>
#include <Input/vjSim/vjSimDigitalGlove.h>
#include <Config/vjConfigChunk.h>

//: Default Constructor
vjSimDigitalGlove::vjSimDigitalGlove() : vjDigital(), vjSimInput(), vjGlove()
{
   //vjDEBUG(vjDBG_ALL,3)<<"*** vjSimDigitalGlove::vjSimDigitalGlove()\n"<< vjDEBUG_FLUSH;
}

//: Destructor
vjSimDigitalGlove::~vjSimDigitalGlove()
{
  // vjDEBUG(vjDBG_ALL,3)<<"*** vjSimDigitalGlove::~vjSimDigitalGlove()\n"<< vjDEBUG_FLUSH;
}

bool vjSimDigitalGlove::config( vjConfigChunk* chunk )
{
   //vjDEBUG(vjDBG_ALL,3)<<"*** vjSimDigitalGlove::config\n"<< vjDEBUG_FLUSH;
   if(! (vjInput::config(chunk) && vjDigital::config(chunk) && vjSimInput::config(chunk) && vjGlove::config(chunk)))
      return false;

   //mCurGesture = 0;     // We are in no gesture yet

   //if ((!vjDigital::config( chunk )) || (!vjSimInput::config( chunk )))
   //   return false;

   std::vector< vjVarValue* > key_list = chunk->getAllProperties( "keyPairs" );
   mSimKeys = readKeyList( key_list );

   int num_pairs = mSimKeys.size();
   mDigitalData = std::vector< int >( num_pairs, 0 );      // Initialize to all zeros

   ////////////////  left posProxy
   vjASSERT( VJ_MAX_GLOVE_DEVS >= 2 );
   const int LEFT_INDEX = 0, RIGHT_INDEX = 1;
   if (LEFT_INDEX < VJ_MAX_GLOVE_DEVS)
   {
      std::string glove_pos_proxy = chunk->getProperty( "glovePos" );    // Get the name of the pos_proxy
      if (glove_pos_proxy == std::string(""))
      {
         vjDEBUG( vjDBG_INPUT_MGR, 0 )
            << "[ERROR]: vjSimPinchglove has no posProxy, config fails."
            << std::endl << vjDEBUG_FLUSH;
         return false;
      }

      // init glove pos proxy interface
      /*
      int proxy_index = vjKernel::instance()->getInputManager()->getProxyIndex( glove_pos_proxy );
      if (proxy_index != -1)
         mGlovePos[LEFT_INDEX] = vjKernel::instance()->getInputManager()->getPosProxy( proxy_index );
      else
         vjDEBUG( vjDBG_INPUT_MGR, 0 )
            << "[ERROR]: vjSimPinchglove::config(): Can't find posProxy, config fails."
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
            << "[ERROR]: vjSimPinchglove has no rightPosProxy, config fails."
            << std::endl << vjDEBUG_FLUSH;
         return false;
      }

      // init right glove pos proxy interface
      /*
      int proxy_index = vjKernel::instance()->getInputManager()->getProxyIndex( glove_pos_proxy );
      if (proxy_index != -1)
         mGlovePos[RIGHT_INDEX] = vjKernel::instance()->getInputManager()->getPosProxy( proxy_index );
      else
         vjDEBUG( vjDBG_INPUT_MGR, 0 )
            << "[ERROR]: vjSimPinchglove::config(): Can't find posProxy, config fails."
            << std::endl << std::endl << vjDEBUG_FLUSH;
            */
   }


   return true;
}

// Updates the state of the digital data vector <br>
//
// NOTE: Digital is on when key is held down
//+     When key is release, digital goes to off state
void vjSimDigitalGlove::updateData()
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

//TODO: move this function up the hierarchy, since vjPinchGlove also has this one.
//NOTE: this function *is* slightly different...
void vjSimDigitalGlove::updateFingerAngles()
{
    const int LEFT_HAND = 0;
    const int RIGHT_HAND = 1;

    vjASSERT( progress < 3 && progress >= 0 );
    vjASSERT( LEFT_HAND < VJ_MAX_GLOVE_DEVS );
    vjASSERT( RIGHT_HAND < VJ_MAX_GLOVE_DEVS );

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
       mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::MPJ] = mLeftHand.pinky().mpj();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::PIJ] = mLeftHand.pinky().pij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::DIJ] = mLeftHand.pinky().dij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT] = mLeftHand.pinky().abduct();

       // Left Ring
       mTheData[LEFT_HAND][progress].angles[vjGloveData::RING][vjGloveData::MPJ] = mLeftHand.ring().mpj();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::RING][vjGloveData::PIJ] = mLeftHand.ring().pij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::RING][vjGloveData::DIJ] = mLeftHand.ring().dij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::RING][vjGloveData::ABDUCT] = mLeftHand.ring().abduct();

       // Left Middle
       mTheData[LEFT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::MPJ] = mLeftHand.middle().mpj();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::PIJ] = mLeftHand.middle().pij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::DIJ] = mLeftHand.middle().dij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::ABDUCT] = mLeftHand.middle().abduct();

       // Left Index
       mTheData[LEFT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::MPJ] = mLeftHand.index().mpj();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::PIJ] = mLeftHand.index().pij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::DIJ] = mLeftHand.index().dij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::ABDUCT] = mLeftHand.index().abduct();

       // Left Thumb
       mTheData[LEFT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::MPJ] = mLeftHand.thumb().mpj();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::PIJ] = mLeftHand.thumb().pij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::DIJ] = mLeftHand.thumb().dij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::ABDUCT] = mLeftHand.thumb().abduct();

       // Left Wrist
       mTheData[LEFT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::YAW] = mLeftHand.yaw();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::PITCH] = mLeftHand.pitch();
    }

    // if that assert failed, then at least the code will still run...
    if ( RIGHT_HAND < VJ_MAX_GLOVE_DEVS )
    {
       //vjDEBUG(vjDBG_ALL,0)<<"Rpinky:"<<mRightHand.pinky().mpj()<<","<<mRightHand.pinky().pij()<<","<<mRightHand.pinky().dij()<<","<<mRightHand.pinky().abduct()<<"   "<<vjDEBUG_FLUSH;
       // Right Pinky
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::MPJ] = mRightHand.pinky().mpj();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::PIJ] = mRightHand.pinky().pij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::DIJ] = mRightHand.pinky().dij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT] = mRightHand.pinky().abduct();

       // Right Ring
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::RING][vjGloveData::MPJ] = mRightHand.ring().mpj();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::RING][vjGloveData::PIJ] = mRightHand.ring().pij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::RING][vjGloveData::DIJ] = mRightHand.ring().dij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::RING][vjGloveData::ABDUCT] = mRightHand.ring().abduct();

       // Right Middle
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::MPJ] = mRightHand.middle().mpj();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::PIJ] = mRightHand.middle().pij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::DIJ] = mRightHand.middle().dij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::ABDUCT] = mRightHand.middle().abduct();

       // Right Index
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::MPJ] = mRightHand.index().mpj();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::PIJ] = mRightHand.index().pij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::DIJ] = mRightHand.index().dij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::ABDUCT] = mRightHand.index().abduct();

       // Right Thumb
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::MPJ] = mRightHand.thumb().mpj();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::PIJ] = mRightHand.thumb().pij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::DIJ] = mRightHand.thumb().dij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::ABDUCT] = mRightHand.thumb().abduct();

       // Right Wrist
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::YAW] = mRightHand.yaw();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::PITCH] = mRightHand.pitch();
    }

    //vjDEBUG(vjDBG_ALL,0)<<"out\n"<<std::flush<<vjDEBUG_FLUSH;
}

/*
//: Get the current gesture.
//! RETURNS: id of current gesture
int vjSimDigitalGlove::getGesture()
{ return mCurGesture; }

//: Load trained data for the gesture object
// Loads the file for trained data
void vjSimDigitalGlove::loadTrainedFile(std::string fileName)
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
