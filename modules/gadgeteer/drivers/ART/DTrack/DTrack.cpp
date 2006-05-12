/* Gadgeteer Driver for 'A.R.T. DTrack' Tracker
 * Copyright (C) 2005, Advanced Realtime Tracking GmbH
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 * Purpose: Gadgeteer driver class
 *
 * Authors: Sylvain Brandel, LSIIT-IGG (http://igg.u-strasbg.fr)
 *          Kurt Achatz, Advanced Realtime Tracking GmbH (http://www.ar-tracking.de)
 *
 * Last modified: 2005/06/16
 *
 * DTrack.cpp,v 1.2 2005/06/16 14:43:31 kurt Exp
 */

#include <gadget/Devices/DriverConfig.h>

#include <boost/bind.hpp>

#include <gadget/Type/DeviceConstructor.h>
#include <gadget/gadgetParam.h>

#include <jccl/Config/ConfigElement.h>

#include <vpr/Util/Debug.h>

#include "DTrack.h"


// some constants:

#define VERSION_STRING   "DTrack Driver v0.1.0; (C) 2005, Advanced Realtime Tracking GmbH"

#define BUTTONS_PER_FLYSTICK    8  // number of buttons per 'Flystick' (fixed)
#define VALUATORS_PER_FLYSTICK  2  // number of valuators per 'Flystick' (fixed)


extern "C" GADGET_DRIVER_EXPORT(vpr::Uint32) getGadgeteerVersion()
{
   return __GADGET_version;
}

/** Entry point function for the device driver plug-in. */
extern "C" GADGET_DRIVER_EXPORT(void) initDevice(gadget::InputManager* inputMgr)
{
	new gadget::DeviceConstructor<gadget::DTrack>(inputMgr);
}



namespace gadget
{

// Constructor:

DTrack::DTrack()
{

	thrRunning = false;
	thrThread = NULL;
	
	standalone = NULL;
	receive_port = 5000;
	use_commands = false;
	command_port = 5001;
}


// Destructor:

DTrack::~DTrack()
{

	if(thrRunning){
		this->stopSampling();
	}
}


// Returns a string that matches this device's configuration element type:
//
// return value (o): name string

std::string DTrack::getElementType()
{

	return "dtrack";
}


// Passes configuration data to the driver:
//
// e (i): configuration
// return value (o): configuration was successfull

bool DTrack::config(jccl::ConfigElementPtr e)
{

	if(! (Input::config(e) && Position::config(e) && Digital::config(e) && Analog::config(e))){
		return false;
	}

	// DTrack configuration:

	receive_port = e->getProperty<int>("receive_port");
	server_name= e->getProperty<std::string>("server_name");
	command_port = e->getProperty<int>("command_port");

	vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
		<< "[DTrack] " << VERSION_STRING << std::endl << vprDEBUG_FLUSH;

	vprDEBUG(vprDBG_ALL, vprDBG_CONFIG_STATUS_LVL)
		<< "[DTrack] configuration: receive port = " << receive_port
		<< " command server = " << server_name << ":" << command_port
		<< std::endl << vprDEBUG_FLUSH;
					
	use_commands = true;
	if(server_name.length() == 0 || command_port == 0){
		use_commands = false;
	}

	return true;
}


// Begins sampling:
//
// return value (o): start was successfull

bool DTrack::startSampling()
{

	if(thrRunning){
		return false;
	}

	// initialize standalone driver:

	if(use_commands){
		standalone = new DTrackStandalone(receive_port, server_name.c_str(), command_port);
	}else{
		standalone = new DTrackStandalone(receive_port);
	}

	if(!standalone->valid()){
		delete standalone;
		standalone = NULL;
		
		return false;  // standalone initialization failed
	}

	// start DTrack (if necessary):

	if(use_commands){
		standalone->send(DTRACKLIB_CMD_CAMERAS_AND_CALC_ON);
		standalone->send(DTRACKLIB_CMD_SEND_DATA);
	}

	// start sample thread:

	thrRunning = true;

	thrThread = new vpr::Thread(boost::bind(&DTrack::thrFunction, this));

	if(!thrThread->valid()){
		thrRunning = false;

		return false;  // thread creation failed
	}

	return true;
}


// Stops sampling:
//
// return value (o): stop was successfull

bool DTrack::stopSampling()
{

	if(!thrRunning){
		return false;
	}

	// stop sample thread:
	
	thrRunning = false;

	if(thrThread != NULL){
		thrThread->kill();  // Not guaranteed to work on all platforms
		thrThread->join();

		delete thrThread;
		thrThread = NULL;
	}

	// stop DTrack (if necessary):

	if(use_commands){
		standalone->send(DTRACKLIB_CMD_STOP_DATA);
		standalone->send(DTRACKLIB_CMD_CAMERAS_OFF);
	}

	// release standalone object:

	delete standalone;
	standalone = NULL;

	return true;
}


// Sample thread:
//
// classPointer (i): pointer to calling class

void DTrack::thrFunction()
{
	// sampling loop:

	while(thrRunning){
		sample();
	}
}


// Samples a value (called by the sample thread):
//
// return value (o): sampling was successfull

bool DTrack::sample()
{
	bool stat;
	unsigned long i;
	unsigned long nbody, nflystick;

	if(!thrRunning){
		return false;
	}
	
	stat = standalone->receive();  // receive data from DTrack (blocking with timeout)

	if(!stat){
		return false;
	}

	nbody = (unsigned long )standalone->get_nbody();
	nflystick = (unsigned long )standalone->get_nflystick();

	if(standalone->get_nbodycal() > -1){        // number of calibrated bodies available
		unsigned long nbodycal = (unsigned long )standalone->get_nbodycal();
		nbodycal -= standalone->get_nmeatool();  // ignore measurement tools
	
		resize_curPosition(nbodycal);
	}

	// get 'Flystick' data:

	resize_curPosition(nflystick);

	if(nflystick * BUTTONS_PER_FLYSTICK > curDigital.size()){
		curDigital.resize(nflystick * BUTTONS_PER_FLYSTICK);   // all elements will be set later
	}

	if(nflystick * VALUATORS_PER_FLYSTICK > curAnalog.size()){
		curAnalog.resize(nflystick * VALUATORS_PER_FLYSTICK);  // all elements will be set later
	}

	for(i=0; i<nflystick; i++){
		dtracklib_flystick_type dat = standalone->get_flystick(i);
		
		if(dat.quality >= 0){  // check if Flystick position is tracked
			curPosition[i].mPosData = getpos(dat);
			curPosition[i].setTime();
		}                      // otherwise keep last valid position

		// Flystick buttons:

		unsigned long bt = dat.bt;
		for(int j=0; j<BUTTONS_PER_FLYSTICK; j++){
			curDigital[i*BUTTONS_PER_FLYSTICK + j] = bt & 0x0001;
			curDigital[i*BUTTONS_PER_FLYSTICK + j].setTime();
			
			bt >>= 1;
		}
		
		// Flystick 'HAT switch' (valuators simulated using four buttons):
		
		for(int j=0; j<VALUATORS_PER_FLYSTICK; j++){
			curAnalog[i*VALUATORS_PER_FLYSTICK + j] = 0;
			curAnalog[i*VALUATORS_PER_FLYSTICK + j].setTime();
		}
		
		float f, fn;

		f = 0;
		if(dat.bt & 0x0020){  
			f = -1;
		}else if(dat.bt & 0x0080){
			f = 1;
		}

		if(f != 0){
			this->normalizeMinToMax(f, fn);
			curAnalog[i*VALUATORS_PER_FLYSTICK + 0] = fn;
		}

		f = 0;
		if(dat.bt & 0x0010){  
			f = -1;
		}else if(dat.bt & 0x0040){
			f = 1;
		}

		if(f != 0){
			this->normalizeMinToMax(f, fn);
			curAnalog[i*VALUATORS_PER_FLYSTICK + 1] = fn;
		}
	}

	// get 'standard body' data:

	for(i=0; i<nbody; i++){
		dtracklib_body_type dat = standalone->get_body(i);

		unsigned long id = dat.id + nflystick;  // VRJuggler id number of this 'Standard Body'
		resize_curPosition(id+1);

		curPosition[id].mPosData = getpos(dat);
		curPosition[id].setTime();
	}  // bodies not in this list are not tracked: keep their last valid position

	// update buffers:

	addPositionSample(curPosition);
	addDigitalSample(curDigital);
	addAnalogSample(curAnalog);
	
	return true;
}


// Swap the data indices:

void DTrack::updateData()
{

	// swap buffered data:

	if(thrRunning){
		swapPositionBuffers();
		swapDigitalBuffers();
		swapAnalogBuffers();
	}
}


// Resize curPosition vector:
// n (i): new size

void DTrack::resize_curPosition(unsigned long n)
{
	unsigned long nsize = curPosition.size();

	if(n > nsize){
		curPosition.resize(n);

		for(unsigned long i=nsize; i<n; i++){  // set default for new elements
			curPosition[i].mPosData = getpos_default();
			curPosition[i].setTime();
		}
	}
}


// Transfer DTracklib pose into gmtl pose:
//
// bod (i): DTracklib pose of a body or Flystick
// return value (o): gmtl pose

gmtl::Matrix44f DTrack::getpos(dtracklib_body_type& bod)
{
	gmtl::Matrix44f ret_val;

	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			ret_val[i][j] = bod.rot[i + 3*j];
		}

		ret_val[3][i] = 0;
		ret_val[i][3] = bod.loc[i] / 1000;  // convert to meters
	}

	ret_val[3][3] = 1;

	return ret_val;
}

gmtl::Matrix44f DTrack::getpos(dtracklib_flystick_type& bod)
{
	gmtl::Matrix44f ret_val;

	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			ret_val[i][j] = bod.rot[i + 3*j];
		}

		ret_val[3][i] = 0;
		ret_val[i][3] = bod.loc[i] / 1000;  // convert to meters
	}

	ret_val[3][3] = 1;

	return ret_val;
}

gmtl::Matrix44f DTrack::getpos_default(void)
{
	gmtl::Matrix44f ret_val;

	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			ret_val[i][j] = 0;
		}

		ret_val[i][i] = 1;
	}

	return ret_val;
}


} // End of gadget namespace

