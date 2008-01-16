/* Gadgeteer Driver for 'A.R.T. DTrack' Tracker
 * Copyright (C) 2005-2008, Advanced Realtime Tracking GmbH
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
 * Last modified: 2007/06/20
 *
 * DTrack.cpp,v 1.5 2007/06/20 15:12:58 kurt Exp
 */

#include <gadget/Devices/DriverConfig.h>

#include <boost/bind.hpp>

#include <gadget/Type/DeviceConstructor.h>
#include <gadget/gadgetParam.h>

#include <jccl/Config/ConfigElement.h>

#include <vpr/Util/Debug.h>

#include "DTrack.h"


// some constants:

#define VERSION_STRING   "DTrack Driver v0.2.1; (C) 2005-2008, Advanced Realtime Tracking GmbH"

#define BUTTONS_PER_FLYSTICK    8  // number of buttons per 'Flystick' (fixed)
#define VALUATORS_PER_FLYSTICK  2  // number of valuators per 'Flystick' (fixed)
#define BUTTONS_PER_MEATOOL     2  // number of buttons per 'Measurement Tool' (fixed)


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

	return std::string("dtrack");
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
		standalone->cmd_cameras(true);
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
		standalone->cmd_cameras(false);
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
	int i, j, id;
	int nbt, nvt;
	int num_body, num_flystick, num_meatool;

	if(!thrRunning){
		return false;
	}
	
	stat = standalone->receive();  // receive data from DTrack (blocking with timeout)

	if(!stat){
		return false;
	}

	num_body = standalone->get_num_body();
	num_flystick = standalone->get_num_flystick();
	num_meatool = standalone->get_num_meatool();
	
	resize_curPosition(num_flystick + num_meatool + num_body);
	resize_curDigital(num_flystick * BUTTONS_PER_FLYSTICK + num_meatool * BUTTONS_PER_MEATOOL);
	resize_curAnalog(num_flystick * VALUATORS_PER_FLYSTICK);

	// get 'Flystick' data:

	for(i=0; i<num_flystick; i++){
		dtrack_flystick_type dat = standalone->get_flystick(i);
		
		if(dat.quality >= 0){  // check if Flystick position is tracked
			curPosition[i].mPosData = getpos(dat);
			curPosition[i].setTime();
		}                      // otherwise keep last valid position

		// Flystick buttons:

		nbt = dat.num_button;
		
		if(nbt > BUTTONS_PER_FLYSTICK){
			nbt = BUTTONS_PER_FLYSTICK;
		}
		
		for(j=0; j<nbt; j++){
			id = j + i * BUTTONS_PER_FLYSTICK;  // VRJuggler id number
			
			curDigital[id] = dat.button[j];
			curDigital[id].setTime();
		}
		
		// Flystick valuators ('HAT switch' or 'joystick'):
		
		nvt = dat.num_joystick;
		
		if(nvt > VALUATORS_PER_FLYSTICK){
			nvt = VALUATORS_PER_FLYSTICK;
		}
		
		for(j=0; j<nvt; j++){
			id = j + i * VALUATORS_PER_FLYSTICK;  // VRJuggler id number
			
			curAnalog[id] = dat.joystick[j];
			curAnalog[id].setTime();
		}
	}

	// get 'measurement tool' data:

	for(i=0; i<num_meatool; i++){
		dtrack_meatool_type dat = standalone->get_meatool(i);
		
		if(dat.quality >= 0){  // check if position is tracked
			id = i + num_flystick;  // VRJuggler id number
			
			curPosition[id].mPosData = getpos(dat);
			curPosition[id].setTime();
		}                      // otherwise keep last valid position

		// measurement tool buttons:

		nbt = dat.num_button;
		
		if(nbt > BUTTONS_PER_MEATOOL){
			nbt = BUTTONS_PER_MEATOOL;
		}
		
		for(j=0; j<nbt; j++){
			id = j + i * BUTTONS_PER_MEATOOL + num_flystick * BUTTONS_PER_FLYSTICK;  // VRJuggler id number
			
			curDigital[id] = dat.button[j];
			curDigital[id].setTime();
		}
	}

	// get 'standard body' data:

	for(i=0; i<num_body; i++){
		dtrack_body_type dat = standalone->get_body(i);

		if(dat.quality >= 0){  // check if position is tracked
			id = i + num_flystick + num_meatool;  // VRJuggler id number

			curPosition[id].mPosData = getpos(dat);
			curPosition[id].setTime();
		}                      // otherwise keep last valid position
	}

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

void DTrack::resize_curPosition(int n)
{
	int nsize = (int )curPosition.size();

	if(n > nsize){
		curPosition.resize(n);

		for(int i=nsize; i<n; i++){  // set default for new elements
			curPosition[i].mPosData = getpos_default();
			curPosition[i].setTime();
		}
	}
}

// Resize curDigital vector:
// n (i): new size

void DTrack::resize_curDigital(int n)
{
	int nsize = (int )curDigital.size();

	if(n > nsize){
		curDigital.resize(n);

		for(int i=nsize; i<n; i++){  // set default for new elements
			curDigital[i] = 0;
			curDigital[i].setTime();
		}
	}
}

// Resize curAnalog vector:
// n (i): new size

void DTrack::resize_curAnalog(int n)
{
	int nsize = (int )curAnalog.size();

	if(n > nsize){
		curAnalog.resize(n);

		for(int i=nsize; i<n; i++){  // set default for new elements
			curAnalog[i] = 0.0f;
			curAnalog[i].setTime();
		}
	}
}


// Transfer DTracklib pose into gmtl pose:
//
// bod (i): DTracklib pose of a body or Flystick
// return value (o): gmtl pose

gmtl::Matrix44f DTrack::getpos(dtrack_body_type& bod)
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

gmtl::Matrix44f DTrack::getpos(dtrack_flystick_type& bod)
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

gmtl::Matrix44f DTrack::getpos(dtrack_meatool_type& bod)
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

