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

//===============================================================
// vjIsense (a Wrapper for isIntersense)
//
// Purpose:
//      VR Juggler Intersense tracking class
//
// Author:
//	Ben Thompson
//
// Date: 4-22-99
//===============================================================
#include <vjConfig.h>

#include <strstream>
#include <fstream>

#include <Input/vjPosition/vjIsense.h>
#include <Math/vjCoord.h>
#include <Math/vjQuat.h>
#include <Kernel/vjDebug.h>

// Helper to return the index for theData array
// given the stationNum we are dealing with and the bufferIndex
// to get
//! ARGS:stationNum - The number of the cube we care about
//! ARGS:bufferIndex - the value of current, progress, or valid (it is an offset in the array)
// XXX: We are going to say the cubes are 0 based 


int vjIsense::getStationIndex(int stationNum, int bufferIndex)
{
    int ret_val = (stationNum*3)+bufferIndex;
    vjASSERT((ret_val >= 0) && (ret_val < ((mTracker.NumStations()+1)*3)));
    return ret_val;
}


vjIsense::vjIsense()
{
   myThread = NULL;
}

bool vjIsense::config(vjConfigChunk *c)
{
    vjDEBUG(vjDBG_INPUT_MGR,1) << "	 vjIsense::vjIsense(vjConfigChunk*)"
			       << std::endl << vjDEBUG_FLUSH;

// read in vjPosition's, vjDigital's, and vjAnalog's config stuff,
// --> this will be the port and baud fields
    if (!vjPosition::config(c) || !vjDigital::config(c) || !vjAnalog::config(c))
	return false;
   
// keep isIntersense's port and baud members in sync with vjInput's port and baud members.
    vjDEBUG(vjDBG_INPUT_MGR,1) << "   vjIsense::vjIsense(vjConfigChunk*) -> vjInput::getPort() = " << vjInput::getPort() << std::endl << vjDEBUG_FLUSH;
    mTracker.setPortName( vjInput::getPort() );
    mTracker.rBaudRate() = vjInput::getBaudRate();
    mTracker.rNumStations() = (int)  static_cast<int>(c->getProperty("num"));

// load an init script for the tracker and then pass it to mTracker
    char* filename = c->getProperty("script").cstring();
    std::strstream	script;
    std::ifstream	scriptFile;
    scriptFile.open(filename);
    script<<scriptFile.rdbuf();
    mTracker.setScript(script.str());
    scriptFile.close();
    
    return true;
}

vjIsense::~vjIsense()
{
    this->stopSampling();
    if (theData != NULL)
	delete [] theData;
    if (mDataTimes != NULL)
	delete mDataTimes;
}

// Main thread of control for this active object
void vjIsense::controlLoop(void* nullParam)
{

    if (theData != NULL)
	delete [] theData;
    if (mDataTimes != NULL)
	delete mDataTimes;

// Allocate buffer space for inertia cubes
// mugsy -> TODO: hardcode number of stations at 2(?) for a test
// what flock does is first query number of birds, with 0 being a valid number
//mugsy -> WARNING: added stations I've moved the problem to the wrapper class
     
    int numbuffs = (mTracker.NumStations())*3;
    theData = (vjMatrix*) new vjMatrix[numbuffs];
    mDataTimes = new vjTimeStamp[numbuffs];

// Reset current, progress, and valid indices
    resetIndexes();
    
// Loop through and keep sampling
    for (;;)
    {
	this->sample();
    }
}

int vjIsense::startSampling()
{
// make sure inertia cubes aren't already started
    if (this->isActive() == true)
    {
	vjDEBUG(vjDBG_INPUT_MGR,2) << "vjIsense was already started."
                                   << std::endl << vjDEBUG_FLUSH;
	return 0;
    }

// Has the thread actually started already
    if(myThread != NULL)
    {
	vjDEBUG(vjDBG_ERROR,vjDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
						<< "vjIsense: startSampling called, when already sampling.\n"
						<< vjDEBUG_FLUSH;
	vjASSERT(false);
	
    } else {

// open the tracker connection
	mTracker.open();
// sanity check.. make sure birds actually started
    	if (this->isActive() == false) {
		vjDEBUG(vjDBG_INPUT_MGR,2)  << "vjIsense: mTracker.open failed to start tracker" << std::endl << vjDEBUG_FLUSH;
		return 0;
    	}

// Create a new thread to handle the control
	vjThreadMemberFunctor<vjIsense>* memberFunctor =
	    new vjThreadMemberFunctor<vjIsense>(this, &vjIsense::controlLoop, NULL);
	vjThread* new_thread;
	new_thread = new vjThread(memberFunctor, 0);
	myThread = new_thread;


	if ( myThread == NULL )
	{
	    return 0;  // Fail
	} else {
	    return 1;   // success
	}
    }
}

int vjIsense::sample()
{
    if (this->isActive() == false)
	return 0;

    int i,  j;
    vjTimeStamp sampletime;


    sampletime.set();
    mTracker.updateData();   


    int cnt_digital, cnt_analog;

    for (i=0, cnt_digital = 0, cnt_analog = 0; i < (mTracker.NumStations()); i++)
    {
	int index = getStationIndex(i,progress);

	if( mTracker.rAngleFormat(i) == ISD_EULER ) {
	    theData[index].makeZYXEuler(mTracker.zRot( i ),
					mTracker.yRot( i ),
					mTracker.xRot( i ));
 
	    theData[index].setTrans(mTracker.xPos( i ),
  	                            mTracker.yPos( i ),
  	                            mTracker.zPos( i ));
	} else {

	    vjQuat quatValue(mTracker.xQuat( i ),
			     mTracker.yQuat( i ), 
			     mTracker.zQuat( i ), 
			     mTracker.wQuat( i ));
	    theData[index].makeQuaternion(quatValue);
	}

	for( j = 0; j < MAX_NUM_BUTTONS; j++) 
	    mInput[current].digital[cnt_digital++] = mTracker.buttonState(i, j);
	for( j = 0; j < MAX_ANALOG_CHANNELS; j++)
	    mInput[current].analog[cnt_analog++] = mTracker.analogData(i, j);

	mDataTimes[index] = sampletime;

// Transforms between the cord frames
// See transform documentation and VR System pg 146
// Since we want the reciver in the world system, Rw
// wTr = wTt*tTr

	vjMatrix world_T_transmitter, transmitter_T_reciever, world_T_reciever;

	world_T_transmitter = xformMat;                    // Set transmitter offset from local info
	transmitter_T_reciever = theData[index];           // Get reciever data from sampled data
	world_T_reciever.mult(world_T_transmitter, transmitter_T_reciever);   // compute total transform
	theData[index] = world_T_reciever;                                     // Store corrected xform back into data

   }
   
// Locks and then swaps the indices
    swapValidIndexes();

    return 1;
}

int vjIsense::stopSampling()
{
    if (this->isActive() == false)
	return 0;

    if (myThread != NULL)
    {
	vjDEBUG(vjDBG_INPUT_MGR,1) << "Stopping the intersense thread..." << vjDEBUG_FLUSH;

	myThread->kill();
	delete myThread;
	myThread = NULL;

	mTracker.close();

// sanity check: did the flock actually stop?
	if (this->isActive() == true)
	{
	    vjDEBUG(vjDBG_INPUT_MGR,0) << "Intersense tracker didn't stop." << std::endl << vjDEBUG_FLUSH;
	    return 0;
	}

	vjDEBUG(vjDBG_INPUT_MGR,1) << "stopped." << std::endl << vjDEBUG_FLUSH;
    }

    return 1;
}


//d == station#
vjMatrix* vjIsense::getPosData( int d )
{
    if (this->isActive() == false)
	return NULL;

    return (&theData[getStationIndex(d,current)]);
}


int vjIsense::getDigitalData( int d )
{
    if(this->isActive() == false)
	return NULL;
  
    return mInput[current].digital[d];
}

float vjIsense::getAnalogData( int d )
{
 
    if(this->isActive() == false)
	return NULL;
  
    return mInput[current].analog[d];
 
}  

vjTimeStamp* vjIsense::getPosUpdateTime (int d) 
{
    if (this->isActive() == false)
	return NULL;

    return (&mDataTimes[getStationIndex(d,current)]);
}

void vjIsense::updateData()
{
    if (this->isActive() == false)
	return;

// this unlocks when this object is destructed (upon return of the function)
    vjGuard<vjMutex> updateGuard(lock);

// TODO: modify the datagrabber to get correct data
// Copy the valid data to the current data so that both are valid
    for(int i=0;i<mTracker.NumStations();i++)
	theData[getStationIndex(i,current)] = theData[getStationIndex(i,valid)];   // first hand

// Locks and then swap the indicies
    swapCurrentIndexes();

    return;
}


