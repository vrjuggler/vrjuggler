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
//      Ben Thompson
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
    vjDEBUG(vjDBG_INPUT_MGR,1) << "*** vjIsense::vjIsense() ***\n" << vjDEBUG_FLUSH;
    vjDEBUG(vjDBG_INPUT_MGR,1) << "*** vjIsense::deviceAbilities = " << deviceAbilities << " ***\n" << vjDEBUG_FLUSH;
}

bool vjIsense::config(vjConfigChunk *c)
{
    vjDEBUG(vjDBG_INPUT_MGR,1) << "         vjIsense::vjIsense(vjConfigChunk*)"
                               << std::endl << vjDEBUG_FLUSH;

// read in vjPosition's, vjDigital's, and vjAnalog's config stuff,
// --> this will be the port and baud fields
// XXX: The problem with calling each of these is that vjInput::config(c) is called by each
// one of them.  Therefore, we have multiple cases where vjInput::Config is being called for
// a single object.  A possible solution is to move the vjInput::config(c) out of these base
// classes into the highest level class (the device driver).
// If we choose to do this, all device drivers need to be updated accordingly

    if (!vjPosition::config(c) || !vjDigital::config(c) || !vjAnalog::config(c))
        return false;
   
// keep isIntersense's port and baud members in sync with vjInput's port and baud members.
    vjDEBUG(vjDBG_INPUT_MGR,1) << "   vjIsense::vjIsense(vjConfigChunk*) -> vjInput::getPort() = " << vjInput::getPort() << std::endl << vjDEBUG_FLUSH;
    mTracker.setPortName( vjInput::getPort() );
    mTracker.rBaudRate() = vjInput::getBaudRate();
    mTracker.rNumStations() = c->getNum("stations");
    
    if(stations != NULL) delete [] stations;
    stations = new vjISStationConfig[mTracker.rNumStations()];
    vjConfigChunk* stationConfig = NULL;
    for( int i = 0; i < mTracker.rNumStations(); i++)
    {
	 stationConfig = static_cast<vjConfigChunk*>(c->getProperty("stations", i));
	 stations[i].enabled = static_cast<bool>(stationConfig->getProperty("enabled"));
	 stations[i].stationIndex = static_cast<int>(stationConfig->getProperty("stationIndex"));
	 stations[i].useDigital = static_cast<bool>(stationConfig->getProperty("useDigital"));
	 stations[i].useAnalog = static_cast<bool>(stationConfig->getProperty("useAnalog"));
    
	 stations[i].dig_min = static_cast<int>(stationConfig->getProperty("digitalFirst"));
	 stations[i].dig_num = static_cast<int>(stationConfig->getProperty("digitalNum"));
	 stations[i].ana_min = static_cast<int>(stationConfig->getProperty("analogFirst"));
	 stations[i].ana_num = static_cast<int>(stationConfig->getProperty("analogNum"));
    }

    int jugglerUnits = static_cast<int>(c->getProperty("vjUnits"));
    int intersenseUnits = static_cast<int>(c->getProperty("isUnits"));
    if(jugglerUnits >= vjIsense_Units_COUNT || jugglerUnits < 0 || intersenseUnits >= vjIsense_Units_COUNT || intersenseUnits < 0)
    {
	vjDEBUG(vjDBG_INPUT_MGR,1) << "   vjIsense::vjIsense(vjConfigChunk*)  Invalid units specified... no conversion applied!" << std::endl << vjDEBUG_FLUSH;
	curConvFactor = 1.0;
    }
    else
    {
        // Conversion table to convert from (units) to meters
        float vjIsense_Conversion_Factors[vjIsense_Units_COUNT] = { 
            0.0254, // Inches to meters
            0.3048, // Feet to meters
            0.01, // Centimeters to meters
            1.0, // Meters to meters
        };

	curConvFactor = vjIsense_Conversion_Factors[intersenseUnits]/vjIsense_Conversion_Factors[jugglerUnits];
    }

// load an init script for the tracker and then pass it to mTracker
    char* filename = c->getProperty("script").cstring();
    std::strstream        script;
    std::ifstream        scriptFile;
    scriptFile.open(filename);
    script<<scriptFile.rdbuf();
    mTracker.setScript(script.str());
    scriptFile.close();
    
    mTracker.rVerbose() = static_cast<bool>(c->getProperty("verbose"));

    return true;
}

vjIsense::~vjIsense()
{
    this->stopSampling();
    if (stations != NULL)
	delete [] stations;
    if (theData != NULL)
        delete [] theData;
    if (mDataTimes != NULL)
        delete [] mDataTimes;
}

// Main thread of control for this active object
void vjIsense::controlLoop(void* nullParam)
{

    if (theData != NULL)
        delete [] theData;
    if (mDataTimes != NULL)
        delete mDataTimes;
     
    int numbuffs = (mTracker.NumStations())*3;
    theData = (vjMatrix*) new vjMatrix[numbuffs];
    mDataTimes = new vjTimeStamp[numbuffs];

// Configure the stations used by the configuration
    int j = 0;
    for( int i = 0; i < mTracker.NumStations(); i++ )
    {
	j = stations[i].stationIndex;
    
	mTracker.getConfigState(j);
	mTracker.rState(j) = stations[i].enabled;
	mTracker.rAngleFormat(j) = ISD_EULER;
	mTracker.rButtons(j) = stations[i].useDigital;
	mTracker.rAnalogData(j) = stations[i].useAnalog;
	mTracker.setConfigState(j);
    }
    
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
    	if (this->isActive() == false) {
		vjDEBUG(vjDBG_ERROR,vjDBG_CRITICAL_LVL) << clrOutNORM(clrRED,"ERROR:")
						<< "vjIsense: mTracker.open() failed to connect to tracker.\n"
						<< vjDEBUG_FLUSH;
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

    return 0;
}

int vjIsense::sample()
{
    if (this->isActive() == false)
        return 0;

    int i,  j;
    vjTimeStamp sampletime;


    sampletime.set();
    mTracker.updateData();   


    int k;
    int stationIndex;
    int min, num;

    for (i = 0 ; i < (mTracker.NumStations()); i++)
    {
	int index = getStationIndex(i,progress);
	
	stationIndex = stations[i].stationIndex;
	
	if( mTracker.rAngleFormat(stationIndex) == ISD_EULER ) {
	    theData[index].makeZYXEuler(mTracker.zRot( stationIndex ),
					mTracker.yRot( stationIndex ),
					mTracker.xRot( stationIndex ));
	    theData[index].setTrans(curConvFactor*mTracker.xPos( stationIndex ),
  	                            curConvFactor*mTracker.yPos( stationIndex ),
  	                            curConvFactor*mTracker.zPos( stationIndex ));
	} else {

	    vjQuat quatValue(mTracker.xQuat( stationIndex ),
			     mTracker.yQuat( stationIndex ), 
			     mTracker.zQuat( stationIndex ), 
			     mTracker.wQuat( stationIndex ));
	    theData[index].makeQuaternion(quatValue);
	}

// We start at the index of the first digital item (set in the config files)
// and we copy the digital data from this station to the vjIsense device for range (min -> min+count-1)
	min = stations[i].dig_min;
	num = min + stations[i].dig_num;
	if(stations[i].useDigital) {
	    for( j = 0, k = min; (j < MAX_NUM_BUTTONS) && (k < IS_BUTTON_NUM) && (k < num); j++, k++) 
		mInput[current].digital[k] = mTracker.buttonState(stationIndex, j);
	}
// Analog works the same as the digital
	min = stations[i].ana_min;
	num = min + stations[i].ana_num;
	if(stations[i].useAnalog) {
	    for( j = 0, k = min; (j < MAX_ANALOG_CHANNELS) && (k < IS_ANALOG_NUM) && (k < num); j++, k++)
		mInput[current].analog[k] = mTracker.analogData(stationIndex, j);
	}
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
	vjDEBUG(vjDBG_INPUT_MGR,1) << "vjIsense::stopSampling(): Stopping the intersense thread... " << vjDEBUG_FLUSH;

	myThread->kill();
	delete myThread;
	myThread = NULL;

	mTracker.close();

	if (this->isActive() == true)
	{
	    vjDEBUG(vjDBG_INPUT_MGR,0)  << clrOutNORM(clrRED,"\nERROR:")
					<< "vjIsense::stopSampling(): Intersense tracker failed to stop.\n" 
					<< vjDEBUG_FLUSH;
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
        return 0;

    return mInput[current].digital[d];
}

float vjIsense::getAnalogData( int d )
{
    float newValue;
    if(this->isActive() == false)
	return 0.0;
    vjAnalog::normalizeMinToMax(mInput[current].analog[d], newValue);
    return newValue; 
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
}


