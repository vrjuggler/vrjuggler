/*
 *  File:	    $RCSfile$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


///////////////////////////////////////////////////////////////////////////
// Contains: class prototypes and data types for logiclass.c++
//	    (based on Logitech's "logidrvr.h")
//
// Author:   Andy Himberger
//	    Allen Bierbaum
//	    (original) Terry Fong <terry@ptolemy.arc.nasa.gov>
// History:  27-Mar-92  original version
//	    25-Jan-95  adapted to class structure by Allen
//	    09-Mar-95	Added a device baseclass -- Allen
//          30-Apr-97   adapted for use in the C2
//
// Copyright (C) 1992, National Aeronautics and Space Administration
// NASA Ames Research Center, Moffett Field, CA 94035
//////////////////////////////////////////////////////////////////////////

#ifndef _LOGICLASS_H_
#define _LOGICLASS_H_

#include <vjConfig.h>
#include <Input/vjPosition/vjPosition.h>    // The base class for vr_devices
#include <Math/vjVec3.h>
//#include <Inventor/SbLinear.h>	// For the vec classes

#ifndef TRUE
#define TRUE			1
#endif

#ifndef FALSE
#define FALSE			0
#endif

#define DIAGNOSTIC_SIZE		2
#define EULER_RECORD_SIZE 	16

#define logitech_FLAGBIT        0x80
#define logitech_FRINGEBIT      0x40
#define logitech_OUTOFRANGEBIT  0x20
#define logitech_RESERVED       0x10
#define logitech_SUSPENDBUTTON  0x08
#define logitech_LEFTBUTTON     0x04
#define logitech_MIDDLEBUTTON   0x02
#define logitech_RIGHTBUTTON    0x01



//: vjThreeDMouse a positional device driver for the Logitech ThreeD mouse.
//
//  This is a vj hack of Allen's standalone driver for the ThreeD mouse.
//
//  Digital driver support could/should be added in the future.
//
//!PUBLIC_API:
class vjThreeDMouse : public vjPosition
{
public:
	//: Default constructor
	vjThreeDMouse() {
	   myThreadID = NULL;
	}

   virtual bool config(vjConfigChunk* c);

	/** vjInput pure virtual functions **/
	int startSampling();
	int stopSampling();
	void updateData();
	int sample()
	    { return getRecord(&theData[current]); }

	/** vjInput virtual functions **/
	char* getDeviceName() { return "vjThreeDMouse"; }
	static std::string getChunkType() { return std::string("ThreeDMouse");}

	/** vjPosition pure virtual functions **/
	vjMatrix* getPosData(int devNum = 0);
	void getPosData(vjPOS_DATA* &data);

	/** @name Internal functions from original implementation
	 *
	 *  not to be used on the outside
	 */
	//@{
	int openMouse(char* portName);
	int closeMouse();

	void cuDemandReporting ();
	void cuEulerMode ();
	void cuHeadtrackerMode ();
	void cuMouseMode ();
	void cuRequestDiagnostics ();
	void cuRequestReport ()	    // Inline, most called function in package
		// Demand a single report
	    {write (mouseFD, "*d", 2); }

	void cuResetControlUnit ();

	void getDiagnostics (char data[]);
	int  getRecord (vjPOS_DATA *data);
	void resetControlUnit ();


	void setBaseOrigin();
	    // PURPOSE: Sets the current mouse X,Y,Z position to be the base origin

	float getX()
	{ return theData[current].pos[0] + baseVector[0]; }
	float getY()
	{ return theData[current].pos[1] + baseVector[1]; }
	float getZ()
	{ return theData[current].pos[2] + baseVector[2]; }
	float getPitch()    { return theData[current].orient[0]; }
	float getYaw()	    { return theData[current].orient[1]; }
	float getRoll()	    { return theData[current].orient[2]; }
//	vjVec3 getLocation() { return SbVec3f(GetX(), GetY(), GetZ()); }

//	int buttonPressed() { return currentMouseReadings.buttons; }
	    // PURPOSE: Examine what buttons are pressed
	    // Returns packed int,
	    // EX:  if(ButtonPressed() & logitech_LEFTBUTTON) then
	    //		    Button was pressed.

	//@}
    private:
	int mouseFD;
	vjPOS_DATA theData[3];

   vjThread*   myThreadID;	// Ptr to the thread object
	vjVec3      baseVector;	// Used to store the base location tooffset from
				// Originally set to 0,0,0

	int  logitechOpen (char *port_name);

	void eulerToAbsolute (byte record[], vjPOS_DATA * data);
	void printBin (char a);
};


#endif	/* _LOGICLASS_H_ */
