/***********************************************************************
*
*    File Name:      isense.c
*    Description:    wrappers for isdriver routines
*    Created:        12/4/98
*    Author:         Yury Altshuler
*
*    Copyright:      InterSense 1999 - All rights Reserved.
*
*                    
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "isense.h"

#if !defined UNIX
#include <windows.h>
#include <winbase.h>
#endif

#include "isdriver.h"
#include "timer.h"

InterSenseTrackerType ISD_tracker[ISD_MAX_TRACKERS];


/*****************************************************************************
*
*   functionName:   ISD_OpenTracker
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:       Returns -1 on failure. To detect tracker automatically 
*                   specify 0 for commPort    
*
******************************************************************************/
ISD_TRACKER_HANDLE ISD_OpenTrackerPort( const char* commPort, BOOL infoScreen, BOOL verbose, DWORD baud )
{
    ISD_TRACKER_HANDLE i;

    /* locate a free tracker */
    for(i=0; i<ISD_MAX_TRACKERS; i++)
    {
        /* tracker structure is available */
        if(ISD_tracker[i].hardware == ISD_NONE)
        {
            /* detect tracker and initialize it to defaults */
	  /* allow*/
	  if(ISD_openTracker( &ISD_tracker[i], commPort, infoScreen, verbose, baud ))
            {
                ISD_tracker[i].ID = i+1;

                /* configure tracker for best performance */
                ISD_configureTracker( &ISD_tracker[i], verbose );

                return i+1;
            }
            else
            {
                return -1;
            }
        }
    }
    return -1;
}

ISD_TRACKER_HANDLE ISD_OpenTracker( DWORD commPort, BOOL infoScreen, BOOL verbose, DWORD baud ) {
    char *portName[4] = {"/dev/ttyd1", "/dev/ttyd2", "/dev/ttyd3", "/dev/ttyd4"}; 
    
    if(commPort == 0) commPort = 1;
    
    return ISD_OpenTrackerPort(portName[commPort-1], infoScreen, verbose, baud);
}


/*****************************************************************************
*
*   functionName:   ISD_CloseTracker
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
BOOL  ISD_CloseTracker( ISD_TRACKER_HANDLE handle )
{
    int i;

    if( handle > 0 && handle <= ISD_MAX_TRACKERS )
    {
        return ISD_closeTracker( &ISD_tracker[ handle - 1 ] ); 
    }
    else if( handle == 0 )   /* close all */
    {
        for( i=0; i<ISD_MAX_TRACKERS; i++ )
        {
            ISD_closeTracker( &ISD_tracker[ i ] ); 
        }
        return TRUE;
    }
    return FALSE;
}


/*****************************************************************************
*
*   functionName:   ISD_NumOpenTrackers
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:       Returns the number of currently opened trackers.   
*
******************************************************************************/
BOOL  ISD_NumOpenTrackers( WORD *num )
{
    int i;

    *num = 0;

    for(i = 0; i < ISD_MAX_TRACKERS; i++)
    {
        /* tracker structure is in use */
        if(ISD_tracker[i].hardware != ISD_NONE)
        {
            *num += 1;
        }
    }
    return TRUE;
}


/*****************************************************************************
*
*   functionName:   ISD_GetCommState
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:       Get RecordsPerSec and KBitsPerSec without requesting 
*                   genlock settings from the tracker. Use this instead of 
*                   ISD_GetTrackerState to prevent your program from stalling while
*                   waiting for the tracker response   
*
******************************************************************************/
BOOL ISD_GetCommState( ISD_TRACKER_HANDLE handle, ISD_TRACKER_TYPE *Tracker )
{
    if( handle > 0 && handle <= ISD_MAX_TRACKERS)
    {
        return ISD_getCommState( &ISD_tracker[ handle - 1 ], Tracker );
    }
    return FALSE;
}


/*****************************************************************************
*
*   functionName:   ISD_GetTrackerState
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:          
*
******************************************************************************/
BOOL ISD_GetTrackerState( ISD_TRACKER_HANDLE handle, 
                          ISD_TRACKER_TYPE *Tracker, BOOL verbose )
{
    if( handle > 0 && handle <= ISD_MAX_TRACKERS)
    {
        return ISD_getTrackerState( &ISD_tracker[ handle - 1 ], Tracker, verbose );
    }
    return FALSE;
}


/*****************************************************************************
*
*   functionName:   ISD_SetTrackerState
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:        
*
******************************************************************************/
BOOL ISD_SetTrackerState( ISD_TRACKER_HANDLE handle, 
                          ISD_TRACKER_TYPE *Tracker, BOOL verbose )
{
    if( handle > 0 && handle <= ISD_MAX_TRACKERS)
    {
        return ISD_setTrackerState( &ISD_tracker[ handle - 1 ], Tracker, verbose );
    }
    return FALSE;
}


/*****************************************************************************
*
*   functionName:   ISD_SetStationState
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:          
*
******************************************************************************/
BOOL ISD_SetStationState( ISD_TRACKER_HANDLE handle, 
                          ISD_STATION_CONFIG_TYPE *Station, 
                          WORD stationNum, BOOL verbose )
{
    if( handle > 0 && handle <= ISD_MAX_TRACKERS)
    {
        return ISD_setStationState( &ISD_tracker[ handle - 1 ], Station, 
                                     stationNum, verbose, TRUE );
    }
    return FALSE;
}


/*****************************************************************************
*
*   functionName:   ISD_GetStationState
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:          
*
******************************************************************************/
BOOL ISD_GetStationState( ISD_TRACKER_HANDLE handle, 
                          ISD_STATION_CONFIG_TYPE *Station,
                          WORD stationNum, BOOL verbose )
{
    if( handle > 0 && handle <= ISD_MAX_TRACKERS )
    {
        return ISD_getStationState( &ISD_tracker[ handle - 1 ], Station, 
                                     stationNum, verbose );
    }
    return FALSE;
}


/*****************************************************************************
*
*   functionName:   ISD_GetTrackerData
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:       get data from all configured stations    
*
******************************************************************************/
BOOL ISD_GetTrackerData( ISD_TRACKER_HANDLE handle, ISD_DATA_TYPE *Data )
{
    InterSenseTrackerType *tracker;
    int i;

    if( handle > 0 && handle <= ISD_MAX_TRACKERS )
    {
        tracker = &ISD_tracker[ handle - 1 ];

        ISD_getTrackerData( tracker );
        
        /* copy tracker data from internal structures to the user data */
        for(i=0; i<ISD_MAX_STATIONS; i++)
        {
            Data->Station[i].Orientation[0] = tracker->station[i].Orientation[0];
            Data->Station[i].Orientation[1] = tracker->station[i].Orientation[1];
            Data->Station[i].Orientation[2] = tracker->station[i].Orientation[2];
            Data->Station[i].Orientation[3] = tracker->station[i].Orientation[3];

            Data->Station[i].Position[0] = tracker->station[i].Position[0];
            Data->Station[i].Position[1] = tracker->station[i].Position[1];
            Data->Station[i].Position[2] = tracker->station[i].Position[2];

            Data->Station[i].TimeStaMp = tracker->station[i].TimeStaMp;

            memcpy((void *)Data->Station[i].ButtonState, (void *)tracker->station[i].ButtonState, 
                sizeof(Data->Station[i].ButtonState));

            memcpy((void *)Data->Station[i].AnalogData, (void *)tracker->station[i].AnalogData, 
                sizeof(Data->Station[i].AnalogData));

        }
        return TRUE;
    }
    return FALSE;
}


/*****************************************************************************
*
*   functionName:   ISD_SendScript
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:       Send a configuration script to the tracker. Script must 
*                   consist of valid commands as described in the interface 
*                   protocol. Commands in the script should be terminated by
*                   the New Line character '\n'. Line Feed character '\r' is 
*                   added by the function and is not required.   
*
******************************************************************************/
BOOL ISD_SendScript( ISD_TRACKER_HANDLE handle, char *script )
{
    char *buf;
    char command[120];
    InterSenseTrackerType *tracker;
    unsigned int i, len;
    int stringReady = FALSE;

    if( handle > 0 && handle <= ISD_MAX_TRACKERS )
    {
        tracker = &ISD_tracker[ handle - 1 ];
        buf = command;
        len = strlen(script);

        for(i=0; i<len; i++)
        {
            /* time to send the command */
            if(script[i] == '\n' || script[i] == '\r' || script[i] == '\0')
            {
                stringReady = TRUE;
            }
            /* skip the white space */
            else if(script[i] != ' ' && script[i] != '\t')
            {
                *buf++ = script[i];
            }
            if((stringReady && buf != command) || i==len-1)
            {
                *buf = '\0';
                
                /* do not allow user to change some of the settings */
                if(ISD_allowUserCommand( &command[0] ))
                {
                    /* give the tracker time to process previous command */
                    tdelay(0.05f);
                    
                    /* CR LF pair is added in ISD_sendCommand */
                    ISD_sendCommand( tracker, command );
                }
                buf = command;
                stringReady = FALSE;
            }
        }
        return TRUE;
    }
    return FALSE;
}
