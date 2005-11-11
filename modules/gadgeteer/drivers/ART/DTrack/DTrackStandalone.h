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
 * Last modified: 2005/05/28
 *
 * DTrackStandalone.h,v 1.2 2005/06/16 14:44:01 kurt Exp
 */

#ifndef _GADGET_DTRACK_STANDALONE_H_
#define _GADGET_DTRACK_STANDALONE_H_

#include <vector>

#include <vpr/vpr.h>
#include <vpr/Util/Interval.h>
#include <vpr/IO/Socket/SocketDatagram.h>

// --------------------------------------------------------------------------
// Data types and macros

// Body data (6DOF):

typedef struct{
	unsigned long id;     // id number
	float quality;        // quality (0 <= qu <= 1)
	
	float loc[3];         // location (in mm)
	float ang[3];         // orientation angles (eta, theta, phi; in deg)
	float rot[9];         // rotation matrix (column-wise)
} dtracklib_body_type;

// A.R.T. FlyStick data (6DOF + buttons):

typedef struct{
	unsigned long id;     // id number
	float quality;        // quality (0 <= qu <= 1, no tracking if -1)
	unsigned long bt;     // pressed buttons (binary coded)
	
	float loc[3];         // location (in mm)
	float ang[3];         // orientation angles (eta, theta, phi; in deg)
	float rot[9];         // rotation matrix (column-wise)
} dtracklib_flystick_type;

// Measurement tool data (6DOF + buttons):

typedef struct{
	unsigned long id;     // id number
	float quality;        // quality (0 <= qu <= 1, no tracking if -1)
	unsigned long bt;     // pressed buttons (binary coded)
	
	float loc[3];         // location (in mm)
	float rot[9];         // rotation matrix (column-wise)
} dtracklib_meatool_type;

// Single marker data (3DOF):

typedef struct{
	unsigned long id;     // id number
	float quality;        // quality (0 <= qu <= 1)
	
	float loc[3];         // location (in mm)
} dtracklib_marker_type;

// A.R.T. DataGlove data (6DOF + fingers):

#define DTRACKLIB_GLOVE_MAX_NFINGER   3  // maximum number of fingers

typedef struct{
	unsigned long id;     // id number
	float quality;        // quality (0 <= qu <= 1)
	int lr;               // left (0) or right (1) hand
	int nfinger;          // number of fingers

	float loc[3];         // back of the hand: location (in mm)
	float rot[9];         // back of the hand: rotation matrix (column-wise) 

	struct{
		float loc[3];            // finger: location (in mm)
		float rot[9];            // finger: rotation matrix (column-wise) 
		
		float radiustip;         // finger: radius of tip
		float lengthphalanx[3];  // finger: length of phalanxes; order: outermost, middle, innermost
		float anglephalanx[2];   // finger: angle between phalanxes
	} finger[DTRACKLIB_GLOVE_MAX_NFINGER];  // order: thumb, index finger, ...
} dtracklib_glove_type;


// DTrack remote commands:

#define DTRACKLIB_CMD_CAMERAS_OFF           0x1000
#define DTRACKLIB_CMD_CAMERAS_ON            0x1001
#define DTRACKLIB_CMD_CAMERAS_AND_CALC_ON   0x1003

#define DTRACKLIB_CMD_SEND_DATA             0x3100
#define DTRACKLIB_CMD_STOP_DATA             0x3200
#define DTRACKLIB_CMD_SEND_N_DATA           0x3300


// --------------------------------------------------------------------------
// Library class

class DTrackStandalone
{
public:

// Constructor:
//
// udpport (i): UDP port number to receive data from DTrack
//
// remote_ip (i): DTrack remote control: ip address of DTrack PC (NULL if not used)
// remote_port (i): port number of DTrack remote control (0 if not used)
//
// udpbufsize (i): size of buffer for UDP packets (in bytes)
// udptimeout_us (i): UDP timeout (receiving and sending) in us (micro sec)

	DTrackStandalone(
		unsigned short udpport = 5000, const char* remote_ip = NULL, unsigned short remote_port = 0,
		int udpbufsize = 10000, unsigned long udptimeout_us = 1000000
	);

// Destructor:

	~DTrackStandalone(void);

// Check if initialization was successfull:
//
// return value (o): boolean

	bool valid(void);


// Check last receive/send error:
//
// return value (o): boolean

	bool timeout(void);     // 'timeout'
	bool udperror(void);    // 'udp error'
	bool parseerror(void);  // 'parse error'


// Receive and process one DTrack data packet (UDP; ASCII protocol):
//
// return value (o): receiving was successfull

	bool receive(void);

// Get data of last received DTrack data packet:

	unsigned long get_framenr(void);  // frame counter
	double get_timestamp(void);       // timestamp (-1, if information not available)

	int get_nbodycal(void);           // number of calibrated bodies (-1, if information not available)
	int get_nbody(void);                            // number of tracked bodies
	dtracklib_body_type get_body(int ind);          // 6d data (ind (i): index 0..max-1)

	int get_nflystick(void);                        // number of calibrated flysticks
	dtracklib_flystick_type get_flystick(int ind);  // 6df data (ind (i): index 0..max-1)

	int get_nmeatool(void);                         // number of calibrated measurement tools
	dtracklib_meatool_type get_meatool(int ind);    // 6dmt data (ind (i): index 0..max-1)

	int get_nmarker(void);                          // number of tracked single markers
	dtracklib_marker_type get_marker(int ind);      // 3d data (ind (i): index 0..max-1)

	int get_nglove(void);                           // number of tracked DataGloves
	dtracklib_glove_type get_glove(int ind);        // gl data (ind (i): index 0..max-1)


// Send one remote control command (UDP; ASCII protocol) to DTrack:
//
// cmd (i): command code
// val (i): additional value (if needed)
//
// return value (o): sending was successfull

	bool send(unsigned short cmd, int val = 0);


private:
	vpr::SocketDatagram* d_udpsock; // socket for UDP
	vpr::Interval d_udptimeout;     // timeout for receiving and sending

	int d_udpbufsize;               // size of UDP buffer
	char* d_udpbuf;                 // UDP buffer

	bool d_use_remote;              // use DTrack remote command access
	vpr::InetAddr d_remote;         // DTrack remote address

	unsigned long act_framenr;                          // frame counter
	double act_timestamp;                               // timestamp (-1, if information not available)
	
	int act_nbodycal;                                   // number of calibrated bodies (-1, if information not available)
	int act_nbody;                                      // number of tracked bodies
	std::vector<dtracklib_body_type> act_body;          // array containing 6d data

	int act_nflystick;                                  // number of calibrated flysticks
	std::vector<dtracklib_flystick_type> act_flystick;  // array containing 6df data

	int act_nmeatool;                                   // number of calibrated measurement tools
	std::vector<dtracklib_meatool_type> act_meatool;    // array containing 6dmt data

	int act_nmarker;                                    // number of tracked single markers
	std::vector<dtracklib_marker_type> act_marker;      // array containing 3d data
	
	int act_nglove;                                     // number of tracked DataGloves
	std::vector<dtracklib_glove_type> act_glove;        // array containing gl data

	int d_lasterror;                // last receive/send error
	
	void set_noerror(void);         // set last receive/send error to 'no error'
	void set_timeout(void);         // set last receive/send error to 'timeout'
	void set_udperror(void);        // set last receive/send error to 'udp error'
	void set_parseerror(void);      // set last receive/send error to 'parse error'
};


// ---------------------------------------------------------------------------------------------------

#endif  // _GADGET_DTRACK_STANDALONE_H_


