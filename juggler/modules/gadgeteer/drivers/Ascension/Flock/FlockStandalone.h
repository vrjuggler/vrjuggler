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
// aFlock
//
// Purpose:
//      Ascention Flock of birds tracking class
//
// Author:
//	Kevin Meinert
//
// Last Modified: 4-22-99
//===============================================================
#ifndef _ASCENSION_FLOCKOFBIRD_H_
#define _ASCENSION_FLOCKOFBIRD_H_

#include <vpr/IO/Port/SerialPort.h>

#define POSITION_RANGE 12.0f
#define ANGLE_RANGE   180.0f
#define MAX_SENSORS    128


typedef struct {
        float xmin, ymin, zmin;
        float xmax, ymax, zmax;
        float xloc[15][10][15],yloc[15][10][15],zloc[15][10][15];
} CalStruct;

enum BIRD_HEMI  {FRONT_HEM, AFT_HEM, UPPER_HEM, LOWER_HEM, LEFT_HEM, RIGHT_HEM};
enum BIRD_FILT  {AC_NARROW, AC_WIDE, DC_FILTER};

//: class for running a Flock of Birds.
//  aFlock is a positional device driver for the Flock of Birds <br>
//!PUBLIC_API
class aFlock
{
public:
	//: Configure Constructor
	//! ARGS: port - such as "/dev/ttyd3"                         <BR>
	//! ARGS: baud - such as 38400, 19200, 9600, 14400, etc...    <BR>
	//! ARGS: sync - sync type.                                   <BR>
	//! ARGS: block - blocking                                    <BR>
	//! ARGS: numBrds - number of birds in flock,                 <BR>
	//! ARGS: transmit - transmitter unit number,                 <BR>
	//! ARGS: hemi - hemisphere to track from,                    <BR>
	//! ARGS: filt - filtering type,                              <BR>
	//! ARGS: report - flock report rate.                         <BR>
	//! ARGS: calfile - a calibration file, if "", then use none. <BR>
	//                                                       <BR>
	//! POST: configures internal data members,
	//+         doesn't actually talk to the FOB yet.
	aFlock(const char* const port = "/dev/ttyd3",
		const int& baud = 38400,
		const int& sync = 1,
		const int& block = 0,
		const int& numBrds = 3,
		const int& transmit = 3,
		const BIRD_HEMI& hemi = LOWER_HEM,
		const BIRD_FILT& filt = AC_NARROW,
		const char& report = 'R',
		const char* const calfile = "");
	
	
	//: Destructor
	~aFlock();

	
	//: call this to connect to the flock device.
	//  NOTE: flock.isActive() must be false to use this function
	int	    start();
	
	//: stop the flock
	int	    stop();
	
	//: call this repeatedly to update the data from the birds.
	//  NOTE: flock.isActive() must be true to use this function
	int	    sample();
		
	//: see if the flock is active or not
	const bool& isActive() const;

	//: set the port to use
	//  this will be a string in the form of the native OS descriptor <BR>
	//  ex: unix - "/dev/ttyd3", win32 - "COM3" <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setPort( const char* const serialPort );
	
	//: get the port used
	//  this will be a string in the form of the native OS descriptor <BR>
	//  ex: unix - "/dev/ttyd3", win32 - "COM3"
	const char* getPort() const;
	
	
	//: set the baud rate
	//  this is generally 38400, consult flock manual for other rates. <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setBaudRate( const int& baud );
	
	//: get the baud rate
	//  this is generally 38400, consult flock manual for other rates. <BR>
	inline const int&  getBaudRate()  const { return _baud;}

	//: Set the unit number of the transmitter
	//  give - an integer that is the same as the dip switch
	//         setting on the transmitter box (for the unit number) <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setTransmitter( const int& Transmit );
	
	//: Get the unit number of the transmitter
	//  returns - an integer that is the same as the dip switch
	//         setting on the transmitter box (for the unit number) <BR>
	inline const int&  getTransmitter() const { return _xmitterUnitNumber; }


	//: Set the number of birds to use in the flock.
	//  give - an integer number not more than the number of
	//         birds attached to the system <BR>
	//  NOTE: flock.isActive() must be false to use this function
	void	    setNumBirds( const int& n );
	
	//: Get the number of birds to use in the flock.
	//  returns - an integer number not more than the number of
	//         birds attached to the system <BR>
	inline const int&  getNumBirds() const { return _numBirds; }


	//: set the video sync type
	//  this option allows the Flock to syncronize its pulses with
	//  your video display.  This will eliminate most flicker caused
	//  by the magnetic distortion. <BR>
	//  - Refer to your flock manual for what number to use.
	//  NOTE: flock.isActive() must be false to use this function
	void	    setSync( const int& sync );
	
	//: Get the video sync type
	//  this option allows the Flock to syncronize its pulses with
	//  your video display.  This will eliminate most flicker caused
	//  by the magnetic distortion. <BR>
	//  - Refer to your flock manual for what the return number means
	inline const int&  getSync() const {return _syncStyle; }


	//: Set blocking of flock
	//  see flock manual for details.
	//  NOTE: flock.isActive() must be false to use this function
	void	    setBlocking( const int& blVal );
	
	//: Get blocking flock's blocking type
	//  see flock manual for details.
	inline const int&  getBlocking() const { return _blocking; }


	//: Set the type of filtering that the flock uses
	//  NOTE: flock.isActive() must be false to use this function
	void	    setFilterType( const BIRD_FILT& f );
	
	//: Set the type of filtering that the flock uses
	inline const BIRD_FILT&  getFilterType() const { return _filter; }


	//: Set the hemisphere that the transmitter transmits from.
	//  NOTE: flock.isActive() must be false to use this function
	void	    setHemisphere( const BIRD_HEMI& h );
	
	//: Set the hemisphere that the transmitter transmits from.
	inline const BIRD_HEMI&  getHemisphere() const {return _hemisphere; }


	//: Set the report rate that the flock uses
	//  NOTE: flock.isActive() must be false to use this function
	void	    setReportRate( const char& rRate );

	//: Set the report rate that the flock uses
	inline const char& getReportRate() const {return _reportRate; }


	//: get the x position of the i'th reciever
	float&	    xPos( const int& i );
	
	//: get the y position of the i'th reciever
	float&	    yPos( const int& i );
	
	//: get the z position of the i'th reciever
	float&	    zPos( const int& i );
	
	//: get the z rotation of the i'th reciever
	float&	    zRot( const int& i );
	
	//: get the y rotation of the i'th reciever
	float&	    yRot( const int& i );
	
	//: get the x rotation of the i'th reciever
	float&	    xRot( const int& i );

// public methods
public:
	//: with the calibration table info, correct a given position
	//  give - a position in x, y, z euclidian coordinates
	//  returns - a corrected position in x, y, z euclidian coordinates
	void        positionCorrect( float& x, float& y, float& z );
	
	//: init the correction table from a file
	//  give - a file name of the calibration file.
	//  result - initializes the correction table with the file's info
	void        initCorrectionTable( const char* const );

// Private data members
private:
	CalStruct   caltable;
	char        _reportRate;
	BIRD_HEMI   _hemisphere;
	BIRD_FILT   _filter;
	
	static const int   MAXCHARSTRINGSIZE;
	char	    _port[256];
	char	    _calibrationFileName[256];

        vpr::SerialPort* _serial_port;
	int	    _baud;
	int	    _syncStyle;
	int	    _blocking;
	int	    _numBirds;
	int	    _xmitterUnitNumber;
	bool	    _usingCorrectionTable;
   
	
	//    x,y,z,        r,y,p
	float _position[MAX_SENSORS][3], _orientation[MAX_SENSORS][3];
	
	bool _active;

        const int mSleepFactor;

private:
        int open_port(void);
        void set_blocking(void);
        void set_sync(void);
        void set_group(void);
        void set_autoconfig(void);
        void set_transmitter(void);
        void set_filter(void);
        void set_hemisphere(void);
        void set_pos_angles(void);
        void pickBird(const int birdID);
        void set_rep_and_stream(void);

        //: get a jreading
        //  give - n:    the bird unit number  <BR>
        //  give - port: the flock port number <BR>
        //  give - xyz positions               <BR>
        //  give - zyx rotations
        int getReading(const int& n, float& xPos, float& yPos, float& zPos,
                       float& zRot, float& yRot, float& xRot );

        float rawToFloat(char& r1, char& r2);
};


#endif


