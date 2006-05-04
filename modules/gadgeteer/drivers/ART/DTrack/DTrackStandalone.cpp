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
 * Purpose: standalone driver class; derived from the A.R.T. sample source
 *          code 'DTracklib' (version v1.2.1), just modified for use with VPR
 *
 * Authors: Kurt Achatz, Advanced Realtime Tracking GmbH (http://www.ar-tracking.de)
 *
 * Last modified: 2005/06/14
 *
 * DTrackStandalone.cpp,v 1.2 2005/06/16 14:44:01 kurt Exp
 */

#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vpr/vpr.h>
#include <vpr/vprTypes.h>
#include <vpr/System.h>
#include <vpr/IO/Socket/SocketDatagram.h>
#include <vpr/Util/Interval.h>
#include <vpr/Util/Debug.h>

#include "DTrackStandalone.h"


// Local error codes:

#define DTRACKLIB_ERR_NONE       0
#define DTRACKLIB_ERR_TIMEOUT    1  // timeout while receiving data
#define DTRACKLIB_ERR_UDP        2  // UDP receive error
#define DTRACKLIB_ERR_PARSE      3  // error in UDP packet


// Local prototypes:

static char* string_nextline(char* str, char* start, unsigned long len);
static char* string_get_ul(char* str, unsigned long* ul);
static char* string_get_d(char* str, double* d);
static char* string_get_f(char* str, float* f);
static char* string_get_block(char* str, char* fmt, unsigned long* uldat, float* fdat);


// --------------------------------------------------------------------------
// Library class

// Constructor:
//
// udpport (i): UDP port number to receive data from DTrack
//
// remote_ip (i): DTrack remote control: ip address of DTrack PC (NULL if not used)
// remote_port (i): port number of DTrack remote control (0 if not used)
//
// udpbufsize (i): size of buffer for UDP packets (in bytes)
// udptimeout_us (i): UDP timeout (receiving and sending) in us (micro sec)

DTrackStandalone::DTrackStandalone(
	unsigned short udpport, const char* remote_ip, unsigned short remote_port,
	int udpbufsize, unsigned long udptimeout_us
)
{
	vpr::ReturnStatus stat;
	vpr::InetAddr addr;

	d_udpbuf = NULL;
	set_noerror();

	// creat UDP socket:

	if(remote_ip != NULL && remote_port != 0){
		stat = d_remote.setAddress(remote_ip, remote_port);
		
		if(!stat.success()){
			vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
				<< "[DTrackStandalone] invalid remote address " << remote_ip << ":" << remote_port
				<< std::endl << vprDEBUG_FLUSH;
					
			d_udpsock = NULL;
			return;
		}
		
		d_use_remote = true;
	}else{
		d_use_remote = false;
	}

	addr = vpr::InetAddr::AnyAddr;
	addr.setPort(udpport);

	d_udpsock = new vpr::SocketDatagram(addr, vpr::InetAddr::AnyAddr);

	stat = d_udpsock->open();

	if(!stat.success()){  // socket cannot be opened
		vprDEBUG(vprDBG_ALL, vprDBG_CRITICAL_LVL)
			<< "[DTrackStandalone] cannot open UDP socket, port " << udpport << std::endl << vprDEBUG_FLUSH;
				
		delete d_udpsock;
		d_udpsock = NULL;
		return;
	}
	
	stat = d_udpsock->bind();

	if(!stat.success()){  // socket cannot be bound
		delete d_udpsock;
		d_udpsock = NULL;
		return;
	}
	
	d_udptimeout.set(udptimeout_us, vpr::Interval::Usec);

	// creat UDP buffer:

	d_udpbufsize = udpbufsize;
	
	d_udpbuf = (char *)malloc(udpbufsize);
	
	if(!d_udpbuf){
		d_udpsock->close();
		delete d_udpsock;
		d_udpsock = NULL;
		return;
	}

	// reset actual DTrack data:

	act_framenr = 0;
	act_timestamp = -1;

	act_nbodycal = -1;
	act_nbody = act_nflystick = act_nmeatool = act_nmarker = act_nglove = 0;
}


// Destructor:

DTrackStandalone::~DTrackStandalone(void)
{

	// release buffer:

	if(d_udpbuf){
		free(d_udpbuf);
	}

	// release UDP socket:

	if(d_udpsock != NULL){
		d_udpsock->close();
		delete d_udpsock;
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
	return (d_lasterror == DTRACKLIB_ERR_TIMEOUT);
}

bool DTrackStandalone::udperror(void)        // 'udp error'
{
	return (d_lasterror == DTRACKLIB_ERR_UDP);
}

bool DTrackStandalone::parseerror(void)      // 'parse error'
{
	return (d_lasterror == DTRACKLIB_ERR_PARSE);
}

// Set last receive/send error:

void DTrackStandalone::set_noerror(void)     // 'no error'
{
	d_lasterror = DTRACKLIB_ERR_NONE;
}

void DTrackStandalone::set_timeout(void)     // 'timeout'
{
	d_lasterror = DTRACKLIB_ERR_TIMEOUT;
}

void DTrackStandalone::set_udperror(void)    // 'udp error'
{
	d_lasterror = DTRACKLIB_ERR_UDP;
}

void DTrackStandalone::set_parseerror(void)  // 'parse error'
{
	d_lasterror = DTRACKLIB_ERR_PARSE;
}


// --------------------------------------------------------------------------
// Receive and process one DTrack data packet (UDP; ASCII protocol):
//
// return value (o): receiving was successfull

bool DTrackStandalone::receive(void)
{
	vpr::ReturnStatus stat;
	vpr::Uint32 len;
	vpr::InetAddr addr;
	
	char* s;
	int i, j;
	unsigned long ul, ularr[2];
	float farr[6];

	if(!valid()){
		return false;
	}

	// Defaults:
	
	act_framenr = 0;
	act_timestamp = -1;   // i.e. not available
	act_nbodycal = -1;    // i.e. not available
	act_nbody = act_nflystick = act_nmeatool = act_nmarker = act_nglove = 0;
	
	// Receive UDP packet:

	do{
		stat = d_udpsock->recvfrom((void *)d_udpbuf, d_udpbufsize-1, addr, len, d_udptimeout);

		if(!stat.success()){
			if(stat.timeout()){
				set_timeout();
				return false;
			}

			set_udperror();
			return false;
		}
	}while(d_udpsock->availableBytes() > 0);

	s = d_udpbuf;
	s[len] = '\0';

	// Process lines:

	set_parseerror();

	do{
		// Line for frame counter:

		if(!strncmp(s, "fr ", 3)){
			s += 3;
			
			if(!(s = string_get_ul(s, &ul))){               // get frame counter
				return false;
			}

			act_framenr = ul;
			continue;
		}

		// Line for timestamp:

		if(!strncmp(s, "ts ", 3)){
			s += 3;
			
			if(!(s = string_get_d(s, &act_timestamp))){     // get timestamp
				act_timestamp = -1;
				return false;
			}
			continue;
		}
		
		// Line for additional information about number of calibrated bodies:

		if(!strncmp(s, "6dcal ", 6)){
			s += 6;
			
			if(!(s = string_get_ul(s, &ul))){               // get number of bodies
				return false;
			}

			act_nbodycal = (int )ul;
			continue;
		}

		// Line for 6d data:

		if(!strncmp(s, "6d ", 3)){
			s += 3;
			
			if(!(s = string_get_ul(s, &ul))){               // get number of bodies
				return false;
			}

			act_nbody = (int )ul;
			if(act_nbody > (int )act_body.size()){
				act_body.resize(act_nbody);
			}

			for(i=0; i<act_nbody; i++){                     // get data of body
				if(!(s = string_get_block(s, "uf", &act_body[i].id, &act_body[i].quality))){
					return false;
				}
			
				if(!(s = string_get_block(s, "ffffff", NULL, farr))){
					return false;
				}
				for(j=0; j<3; j++){
					act_body[i].loc[j] = farr[j];
					act_body[i].ang[j] = farr[j+3];
				}
			
				if(!(s = string_get_block(s, "fffffffff", NULL, act_body[i].rot))){
					return false;
				}
			}
			
			continue;
		}
		
		// Line for flystick data:

		if(!strncmp(s, "6df ", 4)){
			s += 4;
			
			if(!(s = string_get_ul(s, &ul))){               // get number of flysticks
				return false;
			}

			act_nflystick = (int )ul;
			if(act_nflystick > (int )act_flystick.size()){
				act_flystick.resize(act_nflystick);
			}

			for(i=0; i<act_nflystick; i++){                 // get data of body
				if(!(s = string_get_block(s, "ufu", ularr, &act_flystick[i].quality))){
					return false;
				}
					
				act_flystick[i].id = ularr[0];
				act_flystick[i].bt = ularr[1];
				
				if(!(s = string_get_block(s, "ffffff", NULL, farr))){
					return false;
				}
				for(j=0; j<3; j++){
					act_flystick[i].loc[j] = farr[j];
					act_flystick[i].ang[j] = farr[j+3];
				}
				
				if(!(s = string_get_block(s, "fffffffff", NULL, act_flystick[i].rot))){
					return false;
				}
			}
			
			continue;
		}
		
		// Line for measurement tool data:

		if(!strncmp(s, "6dmt ", 5)){
			s += 5;
			
			if(!(s = string_get_ul(s, &ul))){               // get number of measurement tools
				return false;
			}

			act_nmeatool = (int )ul;
			if(act_nmeatool > (int )act_meatool.size()){
				act_meatool.resize(act_nmeatool);
			}

			for(i=0; i<act_nmeatool; i++){                  // get data of body
				if(!(s = string_get_block(s, "ufu", ularr, &act_meatool[i].quality))){
					return false;
				}
				
				act_meatool[i].id = ularr[0];
				act_meatool[i].bt = ularr[1];
				
				if(!(s = string_get_block(s, "fff", NULL, act_meatool[i].loc))){
					return false;
				}
				
				if(!(s = string_get_block(s, "fffffffff", NULL, act_meatool[i].rot))){
					return false;
				}
			}
			
			continue;
		}
		
		// Line for single markers:

		if(!strncmp(s, "3d ", 3)){
			s += 3;
			
			if(!(s = string_get_ul(s, &ul))){               // get number of markers
				return false;
			}

			act_nmarker = (int )ul;
			if(act_nmarker > (int )act_marker.size()){
				act_marker.resize(act_nmarker);
			}

			for(i=0; i<act_nmarker; i++){                   // get marker data
				if(!(s = string_get_block(s, "uf", &act_marker[i].id, &act_marker[i].quality))){
					return false;
				}

				if(!(s = string_get_block(s, "fff", NULL, act_marker[i].loc))){
					return false;
				}
			}
			
			continue;
		}

		// Line for A.R.T. DataGloves:

		if(!strncmp(s, "gl ", 3)){
			s += 3;
			
			if(!(s = string_get_ul(s, &ul))){               // get number of DataGloves
				return false;
			}

			act_nglove = (int )ul;
			if(act_nglove > (int )act_glove.size()){
				act_glove.resize(act_nglove);
			}

			for(i=0; i<act_nglove; i++){                    // get data for a DataGlove
				if(!(s = string_get_block(s, "ufuu", ularr, &act_glove[i].quality))){
					return false;
				}
				act_glove[i].id = ularr[0];
				act_glove[i].lr = (int )ularr[1];
				act_glove[i].nfinger = (int )ularr[2];

				if(!(s = string_get_block(s, "fff", NULL, act_glove[i].loc))){
					return false;
				}
				
				if(!(s = string_get_block(s, "fffffffff", NULL, act_glove[i].rot))){
					return false;
				}

				for(j=0; j<act_glove[i].nfinger; j++){       // get data for a finger
					if(!(s = string_get_block(s, "fff", NULL, act_glove[i].finger[j].loc))){
						return false;
					}
			
					if(!(s = string_get_block(s, "fffffffff", NULL, act_glove[i].finger[j].rot))){
						return false;
					}

					if(!(s = string_get_block(s, "ffffff", NULL, farr))){
						return false;
					}

					act_glove[i].finger[j].radiustip = farr[0];
					act_glove[i].finger[j].lengthphalanx[0] = farr[1];
					act_glove[i].finger[j].lengthphalanx[1] = farr[3];
					act_glove[i].finger[j].lengthphalanx[2] = farr[5];
					act_glove[i].finger[j].anglephalanx[0] = farr[2];
					act_glove[i].finger[j].anglephalanx[1] = farr[4];
				}
			}
			
			continue;
		}
		
		// ignore unknown line identifiers (could be valid in future DTracks)
		
	}while((s = string_nextline(d_udpbuf, s, d_udpbufsize)));

	set_noerror();
	return true;
}


// Get data of last received DTrack data packet:

unsigned long DTrackStandalone::get_framenr(void)  // frame counter
{
	return act_framenr;
}

double DTrackStandalone::get_timestamp(void)       // timestamp (-1, if information not available)
{
	return act_timestamp;
}


int DTrackStandalone::get_nbodycal(void)           // number of calibrated bodies (-1, if information not available)
{
	return act_nbodycal;
}

int DTrackStandalone::get_nbody(void)                            // number of tracked bodies
{
	return act_nbody;
}

dtracklib_body_type DTrackStandalone::get_body(int ind)          // 6d data (ind (i): index 0..max-1)
{
	if(ind < 0 || ind >= act_nbody){
		dtracklib_body_type dummy;
		memset(&dummy, 0, sizeof(dtracklib_body_type));
		return dummy;
	}
	
	return act_body[ind];
}


int DTrackStandalone::get_nflystick(void)                        // number of calibrated flysticks
{
	return act_nflystick;
}

dtracklib_flystick_type DTrackStandalone::get_flystick(int ind)  // 6df data (ind (i): index 0..max-1)
{
	if(ind < 0 || ind >= act_nflystick){
		dtracklib_flystick_type dummy;
		memset(&dummy, 0, sizeof(dtracklib_flystick_type));
		return dummy;
	}
	
	return act_flystick[ind];
}


int DTrackStandalone::get_nmeatool(void)                         // number of calibrated measurement tools
{
	return act_nmeatool;
}

dtracklib_meatool_type DTrackStandalone::get_meatool(int ind)    // 6dmt data (ind (i): index 0..max-1)
{
	if(ind < 0 || ind >= act_nmeatool){
		dtracklib_meatool_type dummy;
		memset(&dummy, 0, sizeof(dtracklib_meatool_type));
		return dummy;
	}
	
	return act_meatool[ind];
}


int DTrackStandalone::get_nmarker(void)                          // number of tracked single markers
{
	return act_nmarker;
}

dtracklib_marker_type DTrackStandalone::get_marker(int ind)      // 3d data (ind (i): index 0..max-1)
{
	if(ind < 0 || ind >= act_nmarker){
		dtracklib_marker_type dummy;
		memset(&dummy, 0, sizeof(dtracklib_marker_type));
		return dummy;
	}
	
	return act_marker[ind];
}


int DTrackStandalone::get_nglove(void)                           // number of tracked DataGloves
{
	return act_nglove;
}

dtracklib_glove_type DTrackStandalone::get_glove(int ind)        // gl data (ind (i): index 0..max-1)
{
	if(ind < 0 || ind >= act_nglove){
		dtracklib_glove_type dummy;
		memset(&dummy, 0, sizeof(dtracklib_glove_type));
		return dummy;
	}
	
	return act_glove[ind];
}


// ---------------------------------------------------------------------------------------------------
// Send one remote control command (UDP; ASCII protocol) to DTrack:
//
// cmd (i): command code
// val (i): additional value (if needed)
//
// return value (o): sending was successfull

bool DTrackStandalone::send(unsigned short cmd, int val)
{
   std::ostringstream cmd_stream;
	vpr::ReturnStatus stat;
	vpr::Uint32 bytes;

	if(!valid()){
		return false;
	}

	if(!d_use_remote){
		return false;
	}

	// process command code:

	switch(cmd){
		case DTRACKLIB_CMD_CAMERAS_OFF:
         cmd_stream << "dtrack 10 0";
			break;
			
		case DTRACKLIB_CMD_CAMERAS_ON:
         cmd_stream << "dtrack 10 1";
			break;
			
		case DTRACKLIB_CMD_CAMERAS_AND_CALC_ON:
         cmd_stream << "dtrack 10 3";
			break;
			
		case DTRACKLIB_CMD_SEND_DATA:
			cmd_stream << "dtrack 31";
			break;
			
		case DTRACKLIB_CMD_STOP_DATA:
			cmd_stream << "dtrack 32";
			break;
			
		case DTRACKLIB_CMD_SEND_N_DATA:
         cmd_stream << "dtrack 33 " << val;
			break;

		default:
			return false;
	}

	// send UDP packet:

	stat = d_udpsock->sendto(cmd_stream.str(), cmd_stream.str().length(),
				 d_remote, bytes, d_udptimeout);

	if(!stat.success()){
		set_udperror();
		return false;
	}

	if(cmd == DTRACKLIB_CMD_CAMERAS_AND_CALC_ON){
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

static char* string_nextline(char* str, char* start, unsigned long len)
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


// Read next 'unsigned long' value from string:
// str (i): string
// ul (o): read value
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_ul(char* str, unsigned long* ul)
{
	char* s;
	
	*ul = strtoul(str, &s, 0);
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
// fmt (i): format string ('u' for 'unsigned long', 'f' for 'float')
// uldat (o): array for 'unsigned long' values (long enough due to fmt)
// fdat (o): array for 'float' values (long enough due to fmt)
// return value (o): pointer behind read value in str; NULL in case of error

static char* string_get_block(char* str, char* fmt, unsigned long* uldat, float* fdat)
{
	char* strend;
	int index_ul, index_f;

	if(!(str = strchr(str, '['))){       // search begin of block
		return NULL;
	}
	if(!(strend = strchr(str, ']'))){    // search end of block
		return NULL;
	}
	
	str++;                               // remove delimiters
	*strend = '\0';

	index_ul = index_f = 0;

	while(*fmt){
		switch(*fmt++){
			case 'u':
				if(!(str = string_get_ul(str, &uldat[index_ul++]))){
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

