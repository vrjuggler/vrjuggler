/* Gadgeteer Driver for 'A.R.T. DTrack' Tracker
 * Copyright (C) 2005-2007, Advanced Realtime Tracking GmbH
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
 * Purpose: standalone driver class; derived from the A.R.T. sample source
 *          code 'DTrackSDK' (version v1.3.1), just modified for use with VPR
 *
 * Authors: Kurt Achatz, Advanced Realtime Tracking GmbH (http://www.ar-tracking.de)
 *
 * Last modified: 2007/03/27
 *
 * DTrackStandalone.cpp,v 1.4 2007/06/20 15:15:12 kurt Exp
 */

#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vpr/vpr.h>
#include <vpr/vprTypes.h>
#include <vpr/System.h>
#include <vpr/IO/TimeoutException.h>
#include <vpr/IO/Socket/SocketDatagram.h>
#include <vpr/Util/Interval.h>
#include <vpr/Util/Debug.h>

#include "DTrackStandalone.h"


// Local error codes:

#define DTRACK_ERR_NONE       0  // no error
#define DTRACK_ERR_TIMEOUT    1  // timeout while receiving data
#define DTRACK_ERR_UDP        2  // UDP receive error
#define DTRACK_ERR_PARSE      3  // error in UDP packet

// DTrack remote commands:

#define DTRACK_CMD_CAMERAS_OFF           1
#define DTRACK_CMD_CAMERAS_ON            2
#define DTRACK_CMD_CAMERAS_AND_CALC_ON   3

#define DTRACK_CMD_SEND_DATA            11
#define DTRACK_CMD_STOP_DATA            12
#define DTRACK_CMD_SEND_N_DATA          13


// Local prototypes:

static char* string_nextline(char* str, char* start, int len);
static char* string_get_i(char* str, int* i);
static char* string_get_ui(char* str, unsigned int* ui);
static char* string_get_d(char* str, double* d);
static char* string_get_f(char* str, float* f);
static char* string_get_block(char* str, char* fmt, int* idat, float* fdat);


// --------------------------------------------------------------------------
// Library class

// Constructor:
//
// udpport (i): UDP port number to receive data from DTrack
//
// remote_host (i): DTrack remote control: hostname or IP address of DTrack PC (NULL if not used)
// remote_port (i): port number of DTrack remote control (0 if not used)
//
// udpbufsize (i): size of buffer for UDP packets (in bytes)
// udptimeout_us (i): UDP timeout (receiving and sending) in us (micro second)

DTrackStandalone::DTrackStandalone(
	int udpport, const char* remote_host, int remote_port,
	int udpbufsize, int udptimeout_us
)
{
	vpr::InetAddr addr;

	d_udpsock = NULL;
	d_udpbuf = NULL;
	set_noerror();

	// creat UDP socket:

	if(udpport <= 0 || udpport > 65535){
		return;
	}

	addr = vpr::InetAddr::AnyAddr;
	addr.setPort((unsigned short )udpport);

	d_udpsock = new vpr::SocketDatagram(addr, vpr::InetAddr::AnyAddr);

	try
	{
		d_udpsock->open();
		d_udpsock->bind();
		d_udptimeout.set(udptimeout_us, vpr::Interval::Usec);

		// create UDP buffer:

		d_udpbufsize = udpbufsize;

		d_udpbuf = (char *)malloc(udpbufsize);

		if(!d_udpbuf){
			d_udpsock->close();
			delete d_udpsock;
			d_udpsock = NULL;
			return;
		}

		// DTrack remote control parameters:
	
		d_use_remote = false;
	
		if(remote_host != NULL && remote_port > 0 && remote_port <= 65535){
			try
			{
				d_remote.setAddress(remote_host, remote_port);
				d_use_remote = true;
			}
			catch (vpr::UnknownHostException& ex)
			{
				vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
					<< "[DTrackStandalone] invalid remote address "
					<< remote_host << ":" << remote_port
					<< std::endl << vprDEBUG_FLUSH;
				vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
					<< ex.what() << std::endl;
				free(d_udpbuf);
				d_udpsock->close();
				delete d_udpsock;
				d_udpsock = NULL;
				return;
			}
		}

		d_remote_cameras = false;
		d_remote_tracking = true;
		d_remote_sending = true;

		// reset actual DTrack data:

		act_framecounter = 0;
		act_timestamp = -1;

		act_num_body = act_num_flystick = act_num_meatool = act_num_hand = 0;
		act_num_marker = 0;
	}
	catch (vpr::Exception& ex)
	{
		vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
			<< "[DTrackStandalone] cannot open UDP socket, port "
			<< udpport << std::endl << vprDEBUG_FLUSH;
		vprDEBUG_NEXT(vprDBG_ALL, vprDBG_CRITICAL_LVL)
			<< ex.what() << std::endl;
		delete d_udpsock;
		d_udpsock = NULL;
	}
}


// Destructor:

DTrackStandalone::~DTrackStandalone(void)
{

	// release buffer:

	if(d_udpbuf){
		free(d_udpbuf);
	}

	// release UDP socket:

	if(d_udpsock){
		d_udpsock->close();
		delete d_udpsock;
		d_udpsock = NULL;
	}
}


// Check if initialization was successfull:
//
// return value (o): boolean

bool DTrackStandalone::valid(void)
{
	return (d_udpsock != NULL);
}


// Check last receive/send error:
//
// return value (o): boolean

bool DTrackStandalone::timeout(void)         // 'timeout'
{
	return (d_lasterror == DTRACK_ERR_TIMEOUT);
}

bool DTrackStandalone::udperror(void)        // 'udp error'
{
	return (d_lasterror == DTRACK_ERR_UDP);
}

bool DTrackStandalone::parseerror(void)      // 'parse error'
{
	return (d_lasterror == DTRACK_ERR_PARSE);
}

// Set last receive/send error:

void DTrackStandalone::set_noerror(void)     // 'no error'
{
	d_lasterror = DTRACK_ERR_NONE;
}

void DTrackStandalone::set_timeout(void)     // 'timeout'
{
	d_lasterror = DTRACK_ERR_TIMEOUT;
}

void DTrackStandalone::set_udperror(void)    // 'udp error'
{
	d_lasterror = DTRACK_ERR_UDP;
}

void DTrackStandalone::set_parseerror(void)  // 'parse error'
{
	d_lasterror = DTRACK_ERR_PARSE;
}


// --------------------------------------------------------------------------
// Receive and process one DTrack data packet (UDP; ASCII protocol):
//
// return value (o): receiving was successfull

bool DTrackStandalone::receive(void)
{
	vpr::Uint32 len;
	vpr::InetAddr addr;
	
	char* s;
	int i, j, k, l, n, id;
	char sfmt[20];
	int iarr[3];
	float f, farr[6];
	int loc_num_bodycal, loc_num_handcal, loc_num_flystick1, loc_num_meatool;

	if(!valid()){
		return false;
	}

	// defaults:
	
	act_framecounter = 0;
	act_timestamp = -1;   // i.e. not available
	
	loc_num_bodycal = loc_num_handcal = -1;  // i.e. not available
	loc_num_flystick1 = loc_num_meatool = 0;
	
	// receive UDP packet:

	do{
		try
		{
			len = d_udpsock->recvfrom((void *)d_udpbuf, d_udpbufsize - 1,
						  addr, d_udptimeout);
		}
		catch (vpr::TimeoutException&)
		{
			set_timeout();
			return false;
		}
		catch (vpr::IOException&)
		{
			set_udperror();
			return false;
		}
	}while(d_udpsock->availableBytes() > 0);

	s = d_udpbuf;
	s[len] = '\0';

	// process lines:

	set_parseerror();

	do{
		// line for frame counter:

		if(!strncmp(s, "fr ", 3)){
			s += 3;
			
			if(!(s = string_get_ui(s, &act_framecounter))){  // get frame counter
				act_framecounter = 0;
				return false;
			}

			continue;
		}

		// line for timestamp:

		if(!strncmp(s, "ts ", 3)){
			s += 3;
			
			if(!(s = string_get_d(s, &act_timestamp))){   // get timestamp
				act_timestamp = -1;
				return false;
			}

			continue;
		}
		
		// line for additional information about number of calibrated bodies:

		if(!strncmp(s, "6dcal ", 6)){
			s += 6;
			
			if(!(s = string_get_i(s, &loc_num_bodycal))){  // get number of calibrated bodies
				return false;
			}

			continue;
		}

		// line for standard body data:

		if(!strncmp(s, "6d ", 3)){
			s += 3;
			
			for(i=0; i<act_num_body; i++){  // disable all existing data
				memset(&act_body[i], 0, sizeof(dtrack_body_type));
				act_body[i].id = i;
				act_body[i].quality = -1;
			}

			if(!(s = string_get_i(s, &n))){               // get number of standard bodies (in line)
				return false;
			}

			for(i=0; i<n; i++){                           // get data of standard bodies
				if(!(s = string_get_block(s, "if", &id, &f))){
					return false;
				}

				if(id >= act_num_body){  // adjust length of vector
					act_body.resize(id + 1);

					for(j=act_num_body; j<=id; j++){
						memset(&act_body[j], 0, sizeof(dtrack_body_type));
						act_body[j].id = j;
						act_body[j].quality = -1;
					}

					act_num_body = id + 1;
				}
				
				act_body[id].id = id;
				act_body[id].quality = f;
				
				if(!(s = string_get_block(s, "fff", NULL, act_body[id].loc))){
					return false;
				}
			
				if(!(s = string_get_block(s, "fffffffff", NULL, act_body[id].rot))){
					return false;
				}
			}
			
			continue;
		}
		
		// line for Flystick data (older format):

		if(!strncmp(s, "6df ", 4)){
			s += 4;
			
			if(!(s = string_get_i(s, &n))){               // get number of calibrated Flysticks
				return false;
			}

			loc_num_flystick1 = n;
			
			if(n != act_num_flystick){  // adjust length of vector
				act_flystick.resize(n);
				
				act_num_flystick = n;
			}
			
			for(i=0; i<n; i++){                           // get data of Flysticks
				if(!(s = string_get_block(s, "ifi", iarr, &f))){
					return false;
				}
					
				if(iarr[0] != i){  // not expected
					return false;
				}

				act_flystick[i].id = iarr[0];
				act_flystick[i].quality = f;

				act_flystick[i].num_button = 8;
				k = iarr[1];
				for(j=0; j<8; j++){
					act_flystick[i].button[j] = k & 0x01;
					k >>= 1;
				}
				
				act_flystick[i].num_joystick = 2;  // additionally to buttons 5-8
				if(iarr[1] & 0x20){
					act_flystick[i].joystick[0] = -1;
				}else if(iarr[1] & 0x80){
					act_flystick[i].joystick[0] = 1;
				}else{
					act_flystick[i].joystick[0] = 0;
				}
				if(iarr[1] & 0x10){
					act_flystick[i].joystick[1] = -1;
				}else if(iarr[1] & 0x40){
					act_flystick[i].joystick[1] = 1;
				}else{
					act_flystick[i].joystick[1] = 0;
				}
				
				if(!(s = string_get_block(s, "fff", NULL, act_flystick[i].loc))){
					return false;
				}
				
				if(!(s = string_get_block(s, "fffffffff", NULL, act_flystick[i].rot))){
					return false;
				}
			}
			
			continue;
		}
		
		// line for Flystick data (newer format):

		if(!strncmp(s, "6df2 ", 5)){
			s += 5;
			
			if(!(s = string_get_i(s, &n))){               // get number of calibrated Flysticks
				return false;
			}

			if(n != act_num_flystick){  // adjust length of vector
				act_flystick.resize(n);
				
				act_num_flystick = n;
			}
			
			if(!(s = string_get_i(s, &n))){               // get number of Flysticks
				return false;
			}

			for(i=0; i<n; i++){                           // get data of Flysticks
				if(!(s = string_get_block(s, "ifii", iarr, &f))){
					return false;
				}
					
				if(iarr[0] != i){  // not expected
					return false;
				}

				act_flystick[i].id = iarr[0];
				act_flystick[i].quality = f;

				if(iarr[1] > DTRACK_FLYSTICK_MAX_BUTTON){
					return false;
				}
				if(iarr[1] > DTRACK_FLYSTICK_MAX_JOYSTICK){
					return false;
				}
				act_flystick[i].num_button = iarr[1];
				act_flystick[i].num_joystick = iarr[2];
				
				if(!(s = string_get_block(s, "fff", NULL, act_flystick[i].loc))){
					return false;
				}
				
				if(!(s = string_get_block(s, "fffffffff", NULL, act_flystick[i].rot))){
					return false;
				}

				strcpy(sfmt, "");
				j = 0;
				while(j < act_flystick[i].num_button){
					strcat(sfmt, "i");
					j += 32;
				}
				j = 0;
				while(j < act_flystick[i].num_joystick){
					strcat(sfmt, "f");
					j++;
				}
				
				if(!(s = string_get_block(s, sfmt, iarr, act_flystick[i].joystick))){
					return false;
				}

				k = l = 0;
				for(j=0; j<act_flystick[i].num_button; j++){
					act_flystick[i].button[j] = iarr[k] & 0x01;
					iarr[k] >>= 1;
					
					l++;
					if(l == 32){
						k++;
						l = 0;
					}
				}
			}
			
			continue;
		}

		// line for measurement tool data:

		if(!strncmp(s, "6dmt ", 5)){
			s += 5;
			
			if(!(s = string_get_i(s, &n))){               // get number of calibrated measurement tools
				return false;
			}

			loc_num_meatool = n;

			if(n != act_num_meatool){  // adjust length of vector
				act_meatool.resize(n);
				
				act_num_meatool = n;
			}
			
			for(i=0; i<n; i++){                           // get data of measurement tools
				if(!(s = string_get_block(s, "ifi", iarr, &f))){
					return false;
				}
				
				if(iarr[0] != i){  // not expected
					return false;
				}

				act_meatool[i].id = iarr[0];
				act_meatool[i].quality = f;

				act_meatool[i].num_button = 1;
				act_meatool[i].button[0] = iarr[1] & 0x01;
				
				if(!(s = string_get_block(s, "fff", NULL, act_meatool[i].loc))){
					return false;
				}
				
				if(!(s = string_get_block(s, "fffffffff", NULL, act_meatool[i].rot))){
					return false;
				}
			}
			
			continue;
		}
		
		// line for additional information about number of calibrated Fingertracking hands:

		if(!strncmp(s, "glcal ", 6)){
			s += 6;
			
			if(!(s = string_get_i(s, &loc_num_handcal))){  // get number of calibrated hands
				return false;
			}
			
			continue;
		}

		// line for A.R.T. Fingertracking hand data:

		if(!strncmp(s, "gl ", 3)){
			s += 3;
			
			for(i=0; i<act_num_hand; i++){  // disable all existing data
				memset(&act_hand[i], 0, sizeof(dtrack_hand_type));
				act_hand[i].id = i;
				act_hand[i].quality = -1;
			}

			if(!(s = string_get_i(s, &n))){               // get number of hands (in line)
				return false;
			}

			for(i=0; i<n; i++){                           // get data of hands
				if(!(s = string_get_block(s, "ifii", iarr, &f))){
					return false;
				}
				
				id = iarr[0];

				if(id >= act_num_hand){  // adjust length of vector
					act_hand.resize(id + 1);

					for(j=act_num_hand; j<=id; j++){
						memset(&act_hand[j], 0, sizeof(dtrack_hand_type));
						act_hand[j].id = j;
						act_hand[j].quality = -1;
					}
					
					act_num_hand = id + 1;
				}
				
				act_hand[id].id = iarr[0];
				act_hand[id].lr = iarr[1];
				act_hand[id].quality = f;
				
				if(iarr[2] > DTRACK_HAND_MAX_FINGER){
					return false;
				}
				act_hand[id].nfinger = iarr[2];

				if(!(s = string_get_block(s, "fff", NULL, act_hand[id].loc))){
					return false;
				}
				
				if(!(s = string_get_block(s, "fffffffff", NULL, act_hand[id].rot))){
					return false;
				}

				for(j=0; j<act_hand[id].nfinger; j++){     // get data of fingers
					if(!(s = string_get_block(s, "fff", NULL, act_hand[id].finger[j].loc))){
						return false;
					}
			
					if(!(s = string_get_block(s, "fffffffff", NULL, act_hand[id].finger[j].rot))){
						return false;
					}

					if(!(s = string_get_block(s, "ffffff", NULL, farr))){
						return false;
					}

					act_hand[id].finger[j].radiustip = farr[0];
					act_hand[id].finger[j].lengthphalanx[0] = farr[1];
					act_hand[id].finger[j].anglephalanx[0] = farr[2];
					act_hand[id].finger[j].lengthphalanx[1] = farr[3];
					act_hand[id].finger[j].anglephalanx[1] = farr[4];
					act_hand[id].finger[j].lengthphalanx[2] = farr[5];
				}
			}
			
			continue;
		}
		
		// line for single marker data:

		if(!strncmp(s, "3d ", 3)){
			s += 3;
			
			if(!(s = string_get_i(s, &act_num_marker))){  // get number of markers
				act_num_marker = 0;
				return false;
			}

			if(act_num_marker > (int )act_marker.size()){
				act_marker.resize(act_num_marker);
			}

			for(i=0; i<act_num_marker; i++){              // get data of single markers
				if(!(s = string_get_block(s, "if", &act_marker[i].id, &act_marker[i].quality))){
					return false;
				}

				if(!(s = string_get_block(s, "fff", NULL, act_marker[i].loc))){
					return false;
				}
			}
			
			continue;
		}

		// ignore unknown line identifiers (could be valid in future DTracks)
		
	}while((s = string_nextline(d_udpbuf, s, d_udpbufsize)));

	// set number of calibrated standard bodies, if necessary:

	if(loc_num_bodycal >= 0){  // '6dcal' information was available
		n = loc_num_bodycal - loc_num_flystick1 - loc_num_meatool;
		
		if(n > act_num_body){  // adjust length of vector
			act_body.resize(n);

			for(j=act_num_body; j<n; j++){
				memset(&act_body[j], 0, sizeof(dtrack_body_type));
				act_body[j].id = j;
				act_body[j].quality = -1;
			}
		}

		act_num_body = n;
	}

	// set number of calibrated Fingertracking hands, if necessary:

	if(loc_num_handcal >= 0){  // 'glcal' information was available
		if(loc_num_handcal > act_num_hand){  // adjust length of vector
			act_hand.resize(loc_num_handcal);

			for(j=act_num_hand; j<loc_num_handcal; j++){
				memset(&act_hand[j], 0, sizeof(dtrack_hand_type));
				act_hand[j].id = j;
				act_hand[j].quality = -1;
			}
		}

		act_num_hand = loc_num_handcal;
	}
	
	set_noerror();
	return true;
}


// Get data of last received DTrack data packet:
//  - currently not tracked bodies are getting a quality of -1

unsigned int DTrackStandalone::get_framecounter(void)  // frame counter
{
	return act_framecounter;
}

double DTrackStandalone::get_timestamp(void)       // timestamp (-1, if information not available)
{
	return act_timestamp;
}


int DTrackStandalone::get_num_body(void)              // number of calibrated standard bodies (as far as known)
{
	return act_num_body;  // best information known
}

dtrack_body_type DTrackStandalone::get_body(int id)   // standard body data (id (i): standard body id 0..max-1)
{

	if(id < act_num_body){
		return act_body[id];
	}
	
	dtrack_body_type dummy;
	
	memset(&dummy, 0, sizeof(dtrack_body_type));
	dummy.id = id;
	dummy.quality = -1;
	
	return dummy;
}


int DTrackStandalone::get_num_flystick(void)          // number of calibrated Flysticks
{
	return act_num_flystick;
}

dtrack_flystick_type DTrackStandalone::get_flystick(int id)  // Flystick data (id (i): Flystick id 0..max-1)
{

	if(id < act_num_flystick){
		return act_flystick[id];
	}
	
	dtrack_flystick_type dummy;
	
	memset(&dummy, 0, sizeof(dtrack_flystick_type));
	dummy.id = id;
	dummy.quality = -1;

	return dummy;
}


int DTrackStandalone::get_num_meatool(void)           // number of calibrated measurement tools
{
	return act_num_meatool;
}

dtrack_meatool_type DTrackStandalone::get_meatool(int id)  // measurement tool data (id (i): tool id 0..max-1)
{

	if(id < act_num_meatool){
		return act_meatool[id];
	}
	
	dtrack_meatool_type dummy;
	
	memset(&dummy, 0, sizeof(dtrack_meatool_type));
	dummy.id = id;
	dummy.quality = -1;

	return dummy;
}


int DTrackStandalone::get_num_hand(void)              // number of calibrated Fingertracking hands (as far as known)
{
	return act_num_hand;  // best information known
}

dtrack_hand_type DTrackStandalone::get_hand(int id)   // Fingertracking hand data (id (i): hand id 0..max-1)
{

	if(id < act_num_hand){
		return act_hand[id];
	}
	
	dtrack_hand_type dummy;
	
	memset(&dummy, 0, sizeof(dtrack_hand_type));
	dummy.id = id;
	dummy.quality = -1;
	
	return dummy;
}


int DTrackStandalone::get_num_marker(void)            // number of tracked single markers
{
	return act_num_marker;
}

dtrack_marker_type DTrackStandalone::get_marker(int index)  // single marker data (index (i): index 0..max-1)
{

	if(index >= 0 && index < act_num_marker){
		return act_marker[index];
	}
	
	dtrack_marker_type dummy;
	
	memset(&dummy, 0, sizeof(dtrack_marker_type));
	dummy.id = 0;
	dummy.quality = -1;
	
	return dummy;
}


// ---------------------------------------------------------------------------------------------------
// Send remote control commands (UDP; ASCII protocol) to DTrack:
//
// onoff (i): switch function on or off
//
// return value (o): sending of remote commands was successfull

// Control cameras:

bool DTrackStandalone::cmd_cameras(bool onoff)
{
	int cmd;
	
	if(!valid()){
		return false;
	}

	d_remote_cameras = onoff;
	
	if(d_remote_cameras){  // switch cameras on
		cmd = (d_remote_tracking) ? DTRACK_CMD_CAMERAS_AND_CALC_ON : DTRACK_CMD_CAMERAS_ON;
		
		if(!cmd_send(cmd)){
			return false;
		}

		if(d_remote_sending){
			return cmd_send(DTRACK_CMD_SEND_DATA);
		}
	}else{                 // switch cameras off
		if(d_remote_sending){
			cmd_send(DTRACK_CMD_STOP_DATA);
		}

		return cmd_send(DTRACK_CMD_CAMERAS_OFF);
	}

	return true;
}

// Control tracking calculation:

bool DTrackStandalone::cmd_tracking(bool onoff)
{
	
	if(!valid()){
		return false;
	}

	d_remote_tracking = onoff;
	
	if(d_remote_cameras){  // cameras are on, so send command
		if(d_remote_tracking){
			return cmd_send(DTRACK_CMD_CAMERAS_AND_CALC_ON);
		}else{
			return cmd_send(DTRACK_CMD_CAMERAS_ON);
		}
	}

	return true;
}

// Control sending of UDP output data:

bool DTrackStandalone::cmd_sending_data(bool onoff)
{
	
	if(!valid()){
		return false;
	}

	d_remote_sending = onoff;
	
	if(d_remote_cameras){  // cameras are on, so send command
		if(d_remote_sending){
			return cmd_send(DTRACK_CMD_SEND_DATA);
		}else{
			return cmd_send(DTRACK_CMD_STOP_DATA);
		}
	}

	return true;
}

// Start sending of a fixed number of UDP output frames:
//
// frames (i): number of frames

bool DTrackStandalone::cmd_sending_fixed_data(int frames)
{

	if(!valid()){
		return false;
	}

	if(d_remote_cameras){  // cameras are on, so send command
		return cmd_send(DTRACK_CMD_SEND_N_DATA, frames);
	}

	return true;
}


// Send one remote control command (UDP; ASCII protocol) to DTrack:
//
// cmd (i): command code
// val (i): additional value (if needed)
//
// return value (o): sending was successfull

bool DTrackStandalone::cmd_send(int cmd, int val)
{
	std::ostringstream cmd_stream;
	if(!valid()){
		return false;
	}

	if(!d_use_remote){
		return false;
	}

	// process command code:

	switch(cmd){
		case DTRACK_CMD_CAMERAS_OFF:
			cmd_stream << "dtrack 10 0";
			break;
			
		case DTRACK_CMD_CAMERAS_ON:
			cmd_stream << "dtrack 10 1";
			break;
			
		case DTRACK_CMD_CAMERAS_AND_CALC_ON:
			cmd_stream << "dtrack 10 3";
			break;
			
		case DTRACK_CMD_SEND_DATA:
			cmd_stream << "dtrack 31";
			break;
			
		case DTRACK_CMD_STOP_DATA:
			cmd_stream << "dtrack 32";
			break;
			
		case DTRACK_CMD_SEND_N_DATA:
			cmd_stream << "dtrack 33 " << val;
			break;

		default:
			return false;
	}

	// send UDP packet:

	try
	{
		d_udpsock->sendto(cmd_stream.str(),
				  cmd_stream.str().length() + 1, d_remote,
				  d_udptimeout);
	}
	catch (vpr::IOException&)
	{
		set_udperror();
		return false;
	}

	if(cmd == DTRACK_CMD_CAMERAS_AND_CALC_ON){
		vpr::System::sleep(1);  // some delay (actually only necessary for older DTrack versions...)
	}
	
	set_noerror();
	return true;
}


// ---------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------
// Parsing DTrack data:

// Search next line in buffer:
// str (i): buffer (total)
// start (i): start position within buffer
// len (i): buffer length in bytes
// return (i): begin of line, NULL if no new line in buffer

static char* string_nextline(char* str, char* start, int len)
{
	char* s = start;
	char* se = str + len;
	int crlffound = 0;

	while(s < se){
		if(*s == '\r' || *s == '\n'){  // crlf
			crlffound = 1;
		}else{
			if(crlffound){              // begin of new line found
				return (*s) ? s : NULL;  // first character is '\0': end of buffer
			}
		}

		s++;
	}

	return NULL;                      // no new line found in buffer
}


// Read next 'int' value from string:
// str (i): string
// i (o): read value
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_i(char* str, int* i)
{
	char* s;
	
	*i = (int )strtol(str, &s, 0);
	return (s == str) ? NULL : s;
}


// Read next 'unsigned int' value from string:
// str (i): string
// ui (o): read value
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_ui(char* str, unsigned int* ui)
{
	char* s;
	
	*ui = (unsigned int )strtoul(str, &s, 0);
	return (s == str) ? NULL : s;
}


// Read next 'double' value from string:
// str (i): string
// d (o): read value
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_d(char* str, double* d)
{
	char* s;
	
	*d = strtod(str, &s);
	return (s == str) ? NULL : s;
}


// Read next 'float' value from string:
// str (i): string
// f (o): read value
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_f(char* str, float* f)
{
	char* s;
	
	*f = (float )strtod(str, &s);   // strtof() only available in GNU-C
	return (s == str) ? NULL : s;
}


// Process next block '[...]' in string:
// str (i): string
// fmt (i): format string ('i' for 'int', 'f' for 'float')
// idat (o): array for 'int' values (long enough due to fmt)
// fdat (o): array for 'float' values (long enough due to fmt)
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_block(char* str, char* fmt, int* idat, float* fdat)
{
	char* strend;
	int index_i, index_f;

	if(!(str = strchr(str, '['))){       // search begin of block
		return NULL;
	}
	if(!(strend = strchr(str, ']'))){    // search end of block
		return NULL;
	}
	
	str++;                               // remove delimiters
	*strend = '\0';

	index_i = index_f = 0;

	while(*fmt){
		switch(*fmt++){
			case 'i':
				if(!(str = string_get_i(str, &idat[index_i++]))){
					*strend = ']';
					return NULL;
				}
				break;
				
			case 'f':
				if(!(str = string_get_f(str, &fdat[index_f++]))){
					*strend = ']';
					return NULL;
				}
				break;
				
			default:    // unknown format character
				*strend = ']';
				return NULL;
		}
	}

	// ignore additional data inside the block
	
	*strend = ']';
	return strend + 1;
}

