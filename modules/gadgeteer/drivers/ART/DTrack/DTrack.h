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
 * Last modified: 2005/05/28
 *
 * DTrack.h,v 1.2 2005/06/16 14:43:31 kurt Exp
 */

#ifndef _GADGET_DTRACK_H_
#define _GADGET_DTRACK_H_

#include <gadget/Devices/DriverConfig.h>
#include <gadget/Type/InputBaseTypes.h>
#include <gadget/Type/Input.h>
#include <gadget/Type/Digital.h>
#include <gadget/Type/Analog.h>
#include <gadget/Type/Position.h>

#include "DTrackStandalone.h"


namespace gadget
{

class DTrack
   : public input_digital_analog_position_t
{
public:
	DTrack();
	~DTrack();

	/** Returns a string that matches this device's configuration element type. */
	static std::string getElementType();

	/** Passes configuration data to the driver. */
	virtual bool config(jccl::ConfigElementPtr e);


	/** Begins sampling. */
	virtual bool startSampling();

	/** Samples a value. */
	virtual bool sample();

	/** Stops sampling. */
	virtual bool stopSampling();

	/** Swap the data indices. */
	virtual void updateData();

private:
	vpr::Thread*  thrThread;
	void thrFunction();
	bool thrRunning;

	DTrackStandalone* standalone;
	unsigned short receive_port;
	bool use_commands;
	std::string server_name;
	unsigned short command_port;

	std::vector<PositionData> curPosition;
	std::vector<DigitalData> curDigital;
	std::vector<AnalogData> curAnalog;
	
	void resize_curPosition(unsigned long n);

	gmtl::Matrix44f getpos(dtracklib_body_type& bod);
	gmtl::Matrix44f getpos(dtracklib_flystick_type& bod);
	gmtl::Matrix44f getpos_default(void);
};

} // End of gadget namespace


#endif  // _GADGET_DTRACK_H_


