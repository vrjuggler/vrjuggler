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


#include <vjConfig.h>
#include <Input/vjSim/vjSimDigitalGlove.h>

//: Default Constructor
vjSimDigitalGlove::vjSimDigitalGlove() : vjDigital(), vjGlove(), vjSimInput()
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
   
   vjDigital::config( chunk );
   vjSimInput::config( chunk );
   vjGlove::config( chunk );
   
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
         vjDEBUG( vjDBG_INPUT_MGR, 0 ) << "[ERROR]: vjSimPinchglove has no posProxy, config fails." << endl << vjDEBUG_FLUSH;
         return false;
      }

      // init glove proxy interface
      int proxy_index = vjKernel::instance()->getInputManager()->getProxyIndex( glove_pos_proxy );
      if (proxy_index != -1)
         mGlovePos[LEFT_INDEX] = vjKernel::instance()->getInputManager()->getPosProxy( proxy_index );
      else
         vjDEBUG( vjDBG_INPUT_MGR, 0 ) << "[ERROR]: vjSimPinchglove::config(): Can't find posProxy, config fails." << endl << vjDEBUG_FLUSH << endl;
   }
      
    
   ////////////////  right posProxy
   if (RIGHT_INDEX < VJ_MAX_GLOVE_DEVS)
   {
      std::string glove_pos_proxy = chunk->getProperty( "rightGlovePos" );    // Get the name of the pos_proxy
      if (glove_pos_proxy == std::string(""))
      {
         vjDEBUG( vjDBG_INPUT_MGR, 0 ) << "[ERROR]: vjSimPinchglove has no rightPosProxy, config fails." << endl << vjDEBUG_FLUSH;
         return false;
      }

      // init right glove proxy interface
      int proxy_index = vjKernel::instance()->getInputManager()->getProxyIndex( glove_pos_proxy );
      if (proxy_index != -1)
         mGlovePos[RIGHT_INDEX] = vjKernel::instance()->getInputManager()->getPosProxy( proxy_index );
      else
         vjDEBUG( vjDBG_INPUT_MGR, 0 ) << "[ERROR]: vjSimPinchglove::config(): Can't find posProxy, config fails." << endl << vjDEBUG_FLUSH << endl;
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
   vjGuard<vjMutex> updateGuard(lock);

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
  
   //vjDEBUG(vjDBG_ALL,0)<<mTheData[0][current].outputAngles(cout);
   //vjDEBUG(vjDBG_ALL,0)<<mTheData[1][current].outputAngles(cout);
	
   return;
}

//TODO: move this function up the hierarchy, since vjPinchGlove also has this one.
//NOTE: this function *is* slightly different...
void vjSimDigitalGlove::updateFingerAngles()
{
    const int LEFT_HAND = 0;
    const int RIGHT_HAND = 1;

    assert( progress < 3 && progress >= 0 );
    assert( LEFT_HAND < VJ_MAX_GLOVE_DEVS );
    assert( RIGHT_HAND < VJ_MAX_GLOVE_DEVS );
    
    // use the digital data set the angles for each joint.
    left.setFingers( mDigitalData[LPINKY] == 1,
                     mDigitalData[LRING] == 1,
                     mDigitalData[LMIDDLE] == 1,
                     mDigitalData[LINDEX] == 1,
                     mDigitalData[LTHUMB] == 1 );
    right.setFingers( mDigitalData[RPINKY] == 1,
                     mDigitalData[RRING] == 1,
                     mDigitalData[RMIDDLE] == 1,
                     mDigitalData[RINDEX] == 1,
                     mDigitalData[RTHUMB] == 1 );
   
    //Now, set the ugly ambiguously named array, mTheData:
        
    // if that assert failed, then at least the code will still run...
    if ( LEFT_HAND < VJ_MAX_GLOVE_DEVS )
    {
       //vjDEBUG(vjDBG_ALL,0)<<"Lpinky:"<<left.pinky().mpj()<<","<<left.pinky().pij()<<","<<left.pinky().dij()<<","<<left.pinky().abduct()<<"\n"<<vjDEBUG_FLUSH;
       // Left Pinky
       mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::MPJ] = left.pinky().mpj();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::PIJ] = left.pinky().pij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::DIJ] = left.pinky().dij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT] = left.pinky().abduct();

       // Left Ring
       mTheData[LEFT_HAND][progress].angles[vjGloveData::RING][vjGloveData::MPJ] = left.ring().mpj();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::RING][vjGloveData::PIJ] = left.ring().pij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::RING][vjGloveData::DIJ] = left.ring().dij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::RING][vjGloveData::ABDUCT] = left.ring().abduct();

       // Left Middle
       mTheData[LEFT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::MPJ] = left.middle().mpj();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::PIJ] = left.middle().pij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::DIJ] = left.middle().dij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::ABDUCT] = left.middle().abduct();

       // Left Index
       mTheData[LEFT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::MPJ] = left.index().mpj();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::PIJ] = left.index().pij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::DIJ] = left.index().dij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::ABDUCT] = left.index().abduct();

       // Left Thumb
       mTheData[LEFT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::MPJ] = left.thumb().mpj();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::PIJ] = left.thumb().pij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::DIJ] = left.thumb().dij();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::ABDUCT] = left.thumb().abduct();

       // Left Wrist
       mTheData[LEFT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::YAW] = left.yaw();
       mTheData[LEFT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::PITCH] = left.pitch();
    }
    
    // if that assert failed, then at least the code will still run...
    if ( RIGHT_HAND < VJ_MAX_GLOVE_DEVS )
    {
       //vjDEBUG(vjDBG_ALL,0)<<"Rpinky:"<<right.pinky().mpj()<<","<<right.pinky().pij()<<","<<right.pinky().dij()<<","<<right.pinky().abduct()<<"   "<<vjDEBUG_FLUSH;
       // Right Pinky
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::MPJ] = right.pinky().mpj();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::PIJ] = right.pinky().pij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::DIJ] = right.pinky().dij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::PINKY][vjGloveData::ABDUCT] = right.pinky().abduct();

       // Right Ring
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::RING][vjGloveData::MPJ] = right.ring().mpj();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::RING][vjGloveData::PIJ] = right.ring().pij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::RING][vjGloveData::DIJ] = right.ring().dij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::RING][vjGloveData::ABDUCT] = right.ring().abduct();

       // Right Middle
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::MPJ] = right.middle().mpj();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::PIJ] = right.middle().pij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::DIJ] = right.middle().dij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::MIDDLE][vjGloveData::ABDUCT] = right.middle().abduct();

       // Right Index
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::MPJ] = right.index().mpj();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::PIJ] = right.index().pij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::DIJ] = right.index().dij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::INDEX][vjGloveData::ABDUCT] = right.index().abduct();

       // Right Thumb
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::MPJ] = right.thumb().mpj();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::PIJ] = right.thumb().pij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::DIJ] = right.thumb().dij();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::THUMB][vjGloveData::ABDUCT] = right.thumb().abduct();

       // Right Wrist
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::YAW] = right.yaw();
       mTheData[RIGHT_HAND][progress].angles[vjGloveData::WRIST][vjGloveData::PITCH] = right.pitch();
    }
    
    //vjDEBUG(vjDBG_ALL,0)<<"out\n"<<flush; 
}
