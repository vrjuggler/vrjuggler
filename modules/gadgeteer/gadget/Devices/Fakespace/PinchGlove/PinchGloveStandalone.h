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
// PinchGloveStandalone
//
// Purpose:
//    software interface to Fakespace Pinchglove hardware
//
// Author:
//	Kevin Meinert
//
// NOTES:
//      The long function names greatly decrease the 
//       ambiguity of what the functions do ... sorry. :)
//
// Date: 1-23-99
//===============================================================

#ifndef _GADGET_PINCH_GLOVE_STANDALONE_H_
#define _GADGET_PINCH_GLOVE_STANDALONE_H_

#include <string>

class PinchGloveStandalone
{
public:
    // Default constructor.
    PinchGloveStandalone();
    
    //: Connect to the pinch glove hardware
    bool    connectToHardware( const char* const ttyPort );
    
    //: call updateStringFromHardware to get most 
    //:  current pinch data. 
    void    updateStringFromHardware();
    
    //: get the last sampled string
    //  NOTE: call updateStringFromHardware to get most current pinch data.
    void    getSampledString( std::string& gestureString );
    
    //: Use one of these indices to index the string 
    //  returned by "GetSampledString()"
    enum finger 
    {
	LTHUMB = 0, LINDEX = 1, LMIDDLE = 2, LRING = 3, LPINKY = 4, 
	RTHUMB = 6, RINDEX = 7, RMIDDLE = 8, RRING = 9, RPINKY = 10
    };
    
protected:
    std::string 		mGestureString;
    std::string 		mPreviousGestureString;
    int pinchfd;

    // equal to "00000.00000"
    static const std::string 	mOpenHandString;
    
    /* functions provided by fakespace */
    int	    mConnectToHardware( const char* const ttyPort = "/dev/ttyd3" );
    int	    mSendCommandToHardware( const char* const command, char *reply );
    
    int	    mReadRecordsFromHardware( const int& rec_max_len, char *records );
    void    mGetStringFromHardware( char string[12] );
    
private:
    char                        mGestureStringTemp[12];
};

#endif
