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
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


//===============================================================
// fsPinchGlove
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
#ifndef PINCH_GLOVE_INCLUDED
#define PINCH_GLOVE_INCLUDED

class fsPinchGlove
{
public:
    // Default constructor.
    fsPinchGlove();
    
    //: Connect to the pinch glove hardware
    bool    connectToHardware( const char* const ttyPort );
    
    //: call ReSampleStringFromHardware to get most 
    //:  current pinch data. 
    void    reSampleStringFromHardware();
    
    //: get the last sampled string, 
    //  NOTE: call ReSampleData to get most current pinch data.
    void    getSampledString( char string[12] );
    
    //: Use one of these indices to index the string 
    //  returned by "GetSampledString()"
    enum finger 
    {
	LPINKY = 0, LRING = 1, LMIDDLE = 2, LINDEX = 3, LTHUMB = 4, 
	RTHUMB = 6, RINDEX = 7, RMIDDLE = 8, RRING = 9, RPINKY = 10
    };
    
protected:

    // I use this for type compatibility when passing 
    //   char[12] into a function.
    // This way I can use a pointer to the shared memory, 
    //   and still be type safe. 
    // NOTE: is it really more type safe? or am i just full of it...
    struct charString12
    {
	char  data[12]; //one more for the null char.
    };
    
    //TODO: put these into shared memory
    volatile charString12* _gestureString;
    volatile charString12* _previousGestureString;
    
    // equal to "00000.00000"
    static const char* const _openHandString;
    
    /* functions provided by fakespace */
    int	    _connectToHardware( const char* const ttyPort = "/dev/ttyd3" );
    int	    _sendCommandToHardware( const char* const command, char *reply );
    
    int	    _readRecordsFromHardware( const int& rec_max_len, char *records );
    void    _getStringFromHardware( char string[12] );
};

#endif
