/***********************************************************************
*
*    File Name:      isdriver.c
*    Description:    contains main functionality of the DLL
*    Created:        11/15/97
*    Author:         Yury Altshuler
*
*    Copyright:      InterSense 1999 - All rights Reserved.
*
*                    
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdarg.h>

#include "isense.h"

#if !defined UNIX 
#include <windows.h>
#include <winbase.h>
#include <windowsx.h>
#endif

#include "isdriver.h"
#include "itcom.h"
#include "timer.h"
#include "serial.h"

#define ISD_INTERTRAX_SAMPLE_MODE  IT_COM_SYS_CONTINUOUS

static BOOL ISD_detectTracker(InterSenseTrackerType *tracker, const char* commPort, DWORD baud);
static BOOL ISD_resetInterTrax(InterSenseTrackerType *tracker);
static BOOL ISD_openInterTrax(InterSenseTrackerType *tracker);

extern InterSenseTrackerType ISD_tracker[ISD_MAX_TRACKERS];


/*****************************************************************************
*
*   functionName:   ISD_detectTracker
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:   MODIFIED 14/09/00 Chris Johanson
*
*               baudRate is now a userdefined variable.
*
*               no longer does ISD_detectTracker attempt to force the maximum
*               baud rate, MAX_BAUD_RATE (defined in isense.h), and write the
*               new baud rate to the interface box's permanent memory.
*
******************************************************************************/
BOOL ISD_detectTracker(InterSenseTrackerType *tracker, const char* commPort, DWORD baud)
{
    char   *systemName[MAX_HARDWARE_VERSIONS] =
              {"IS-300 Series", "IS-600 Series", "IS-900 Series", "InterTrax", "Unknown"};
    char   *message[4] = {"first", "second", "third", "fourth"};
    DWORD  baudRate = baud; /* set from fcn argument *//* {115200L, 57600L, 38400L, 19200L, 9600L}; */
    /* mugsy -> adjusted rates to allow only one option...
     baudrate now represents the actual baudrate (e.g., 115200L)*/
    WORD   numTracker;
    static int init = FALSE;

    if(!init)
    {
        ISD_printf(tracker, "InterSense Driver Ver. %.2f\n", ISLIB_VERSION);
        ISD_printf(tracker, "Copyright 1998-99 InterSense Inc.\n");
        init = TRUE;
    }

    ISD_NumOpenTrackers( &numTracker );
    ISD_printf(tracker, "\nLooking for %s InterSense tracking device on port %s. \nPlease wait ", message[numTracker],
        commPort);

#if defined UNIX
    ISD_printf(tracker, "\n");
#endif
    
       /* first try to detect InterTrax */
       
/*     // mugsy -> WARNING: hardcoded baud rate, but applies only to the Intertrax device  
       // mugsy -> commented out the Intertrax detection.  The problem is it forces a connect. 
       if(rs232InitCommunications(&(tracker->CommPort), port, 38400L)) 
         {  
            // port is available. Try to detect the tracker 
             if(ISD_openInterTrax(tracker)) 
             { 
                 tracker->hardware = ISD_INTERTRAX_SERIES; 
                 ISD_printf(tracker, "\nInterTrax detected on port %d\n", port); 
                 return PASS; 
             } 
         } 
*/

            if(baudRate > MAX_BAUD_RATE) {
                ISD_printf(tracker, "\nInterSense tracker not present\n");
                return FAIL;
            }

#if !defined UNIX
            ISD_printf(tracker, ".");
#endif

	    printf("\nBaud Rates:%d\n", baudRate);
            if(rs232InitCommunications(&(tracker->CommPort), commPort, baudRate))
            {
                itSendCommand(tracker, "c");

                if(itComUpdateIntrackSystemStat(tracker, 0.15f) == PASS)
                {
                    /* change baud rate to the maximum supported, defined in isense.h */
/*                    
                    if(baudRate != MAX_BAUD_RATE)
                    {
		                 printf("\nWARNING: call should be eliminated\nChange to max baud rate: %d\n", MAX_BAUD_RATE);
	           	         // mugsy -> Interface box now defaults to baudRate.
                         itSendCommand(tracker, "o%d,N,8,0\r\n", MAX_BAUD_RATE/100); 
                         tdelay(0.1f);
                         rs232InitCommunications(&(tracker->CommPort), commPort, baudRate);
                    }         */

                    ISD_printf(tracker, "\n\n%s device detected on port %s\n",
                        systemName[toIndex(tracker->state.hardwareVersion)], commPort);

                    ISD_printf(tracker, "Firmware Version: ");

                    if(itComUpdateStatusRecord(tracker, 2.0f) == PASS)
                    {
                        /* bug fix for Intrack versions 2.0.5b and below. Version string
                           starts with a blank */

                        if(tracker->state.ver[0] == ' ')  ISD_printf(tracker, "%s\n",  tracker->state.ver);
                        else                              ISD_printf(tracker, " %s\n", tracker->state.ver);
                    }
                    else
                    {
                        ISD_printf(tracker, " unknown\n");
                        return FAIL;
                    }
    
                    tracker->hardware = ISD_PRECISION_SERIES;
                    return PASS;
                }
            }
    rs232DeinitCommunications(&(tracker->CommPort));
    ISD_printf(tracker, "\nInterSense tracker not present\n");
    return FAIL;
}


/*****************************************************************************
*
*   functionName:   ISD_resetInterTrax
*   Description:
*   Created:
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static BOOL ISD_resetInterTrax(InterSenseTrackerType *tracker)
{
    char byte;
    int  status, pass, num;
    COMM_PORT *port;

    port = &(tracker->CommPort);

#if !defined UNIX /* Win32 */

    rs232SetSpeed(port, 1200L);
    for(pass = 0; pass < 2; pass++)
    {
        /* Toggle RTS line */
        rs232SetRTSState(port, FALSE);
        tdelay(0.15f);
        rs232SetRTSState(port, TRUE);

        rs232PutChar(port,  'c');
        status = waitForChar(port,  &byte);
        if(status && byte == 'o') return PASS;
    }
    return FAIL;

#endif

#if defined UNIX

    /* I'm not aware of any way to control RTS line under UNIX, so
       here we have to jump through a few hoops to connect to the tracker */

    /* First, check if tracker is already running */
    tracker->state.transmitMode = ISD_INTERTRAX_SAMPLE_MODE;
    rs232SetSpeed(port, 38400L);
    num = tracker->numSamplesReceived;
    tdelay(0.1);

    for(pass = 0; pass < 2; pass++)
    {
        ISD_INTERTRAX_serviceSerialPort(tracker);
        if(tracker->numSamplesReceived != num) return 2;
    }
    
    for(pass = 0; pass < 2; pass++)
    {
        /* try 38400 command mode */
        rs232SetSpeed(port, 38400L);
        rs232PutChar(port,  'c');
        status = waitForChar(port,  &byte);
        if(status && byte == 'o') return PASS;
        if(status && byte == '!')
        {
            waitForChar(port,  &byte);
            return PASS;    
        }

        /* try 1200 command mode */
        rs232SetSpeed(port, 1200L);
        rs232PutChar(port,  'c');
        status = waitForChar(port,  &byte);
        if(status && byte == 'o') return PASS;
        if(status && byte == '!')
        {
            waitForChar(port,  &byte);
            return PASS;
        }

        /* try 38400 reset */
        rs232SetSpeed(port, 38400L);
        rs232SendString(port, "*reset");
        tdelay(0.1f);

        rs232SetSpeed(port, 38400L);
        rs232PutChar(port,  'c');
        if(status && byte == 'o') return PASS;
        if(status && byte == '!')
        {
            waitForChar(port,  &byte);
            return PASS;
        }

        /* try 1200 reset */
        rs232SetSpeed(port, 1200L);
        rs232SendString(port, "*reset");
        tdelay(0.1f);

        rs232SetSpeed(port, 38400L);
        rs232PutChar(port,  'c');
        if(status && byte == 'o') return PASS;
        if(status && byte == '!')
        {
            waitForChar(port,  &byte);
            return PASS;
        }
    } 
#endif

    return FAIL;
}


/*****************************************************************************
*
*   functionName:   ISD_openInterTrax
*   Description:
*   Created:
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
static BOOL ISD_openInterTrax(InterSenseTrackerType *tracker)
{
    char byte;
    int  status;
    COMM_PORT *port;

    port = &(tracker->CommPort);

    if((status = ISD_resetInterTrax(tracker)) == FAIL) return FAIL;
    if(status ==  2) return PASS;

    /* set output mode to continuous */
    rs232PutChar(port, 't');
    rs232PutChar(port, 2);

    status = waitForChar(port,  &byte);
    if(!status || byte != 't')
    {
        return FAIL;
    }

    /* set baud rate */
    rs232PutChar(port, 'b');
    rs232PutChar(port, 4);

    status = waitForChar(port,  &byte);
    if(!status || byte != 'b')
    {
        return FAIL;
    }

    /* Finished setting up tracker, so switch to data mode */

    rs232PutChar(port,  'd');
    status = waitForChar(port,  &byte);
    if(!status || byte != 'd')
    {
        return FAIL;
    }

    /* Set baud rate to specified baud rate */
    rs232SetSpeed(port, 38400L);

    return PASS;
}


/*****************************************************************************
*
*   functionName:   ISD_sendCommand
*   Description:    Sends command string to the tracker
*   Created:        12/4/98
*   Author:         Yury Altshuler
*
*   Comments:       User is not allowed to send commands directly to the tracker. 
*                   Uses itSendCommand to make sure that applyPSEConfigPending is
*                   set correctly.    
*
******************************************************************************/
BOOL ISD_sendCommand(InterSenseTrackerType *tracker, char *command)
{
    if(tracker->hardware == ISD_PRECISION_SERIES)
    {
        /* add CR LF pair to command */
        if(strlen(command) > 1)
        {
            itSendCommand(tracker, "%s\r\n", command);
        }
        else
        {
            /* single character commands can be safely sent out without
               checking if they change PSE configuration */
            rs232PutChar(&(tracker->CommPort), command[0]);
        }
        return PASS;
    }
    return FAIL;
}


/*****************************************************************************
*
*   functionName:   ISD_requestTrackerUpdate
*   Description:    
*   Created:        12/4/98
*   Author:         Yury Altshuler
*
*   Comments:       RecvUpdateFlag is set to True when update is received.
*                   If it is set then send command requesting another update.
*                   If it's False then check if timed out and send a request
*                   again. This prevents procedure from sending too many commands
*                   when communication is slow or failed 
*
******************************************************************************/
void ISD_requestTrackerUpdate(InterSenseTrackerType *tracker)
{
    switch(tracker->hardware)
    {
        case ISD_PRECISION_SERIES:

            if(tracker->state.transmitMode == IT_COM_SYS_POLLED)
            {
                if(tracker->RecvUpdateFlag >= tracker->state.numActive ||
                     (timeNow() - tracker->LastReqTime) > 0.1)
                {
                    tracker->LastReqTime = timeNow();
                    rs232PutChar(&tracker->CommPort, 'P');
                    tracker->RecvUpdateFlag = 0;
                }
            }
            break;

        case ISD_INTERTRAX_SERIES:

            if(tracker->state.transmitMode == IT_COM_SYS_POLLED)
            {
                if(tracker->RecvUpdateFlag > 0 ||
                    (timeNow() - tracker->LastReqTime) > 0.1)
                {
                    tracker->LastReqTime = timeNow();
                    tracker->RecvUpdateFlag = 0;

                    /* Flush any characters in the serial buffer */
                    rs232RxFlush(&tracker->CommPort, 0);

                    /* Request a 3DOF data packet from the tracker */
                    rs232PutChar(&tracker->CommPort, '3'); 
                }
            }
            break;

        default:
            break;
    }
}


/*****************************************************************************
*
*   functionName:   ISD_getTrackerData
*   Description:
*   Created:        12/4/98
*   Author:         Yury Altshuler
*
*   Comments:      
*
******************************************************************************/
void ISD_getTrackerData(InterSenseTrackerType *tracker)
{
    float currentTime;

    ISD_requestTrackerUpdate(tracker);

    currentTime = timeNow();
    
    if(currentTime - tracker->statsTime > 1.0)
    {
        tracker->RecordsPerSec = (WORD) (tracker->numSamplesReceived/(currentTime - tracker->statsTime));
        tracker->KBitsPerSec = (float) (tracker->numBytesReceived/100.0f)/(currentTime - tracker->statsTime);

        tracker->numBytesReceived = 0;
        tracker->numSamplesReceived = 0;

        tracker->statsTime = currentTime;
    }
    serviceSerialPort(tracker);
}

/*****************************************************************************
*
*   functionName:   ISD_openTracker
*   Description:
*   Created:
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
BOOL ISD_openTracker(InterSenseTrackerType *tracker, 
                     const char* commPort, BOOL infoScreen, BOOL verbose, DWORD baud)
{
    WORD i;
    BOOL status;
    static timerInit = FALSE;
    ISD_STATION_CONFIG_TYPE Station;

    if(!timerInit)
    {
        /* set up the timer */
        initTimer();   
    }

    /* init variables */
    tracker->nchars = 0;
    tracker->statsTime = timeNow();
    tracker->recordStartTime = 0.0f;
    tracker->numBytesReceived = 0;
    tracker->numSamplesReceived = 0;
    tracker->hardware = ISD_NONE;
    tracker->state.numActive = 0;
    tracker->RecordsPerSec = 0;
    tracker->KBitsPerSec = 0.0f;   
    tracker->applyPSEConfigPending = FALSE;
    itComLogClear( tracker );

    for(i=0; i<MAX_NUM_STATIONS; i++)
    {
        tracker->station[i].Position[0] = 0.0f;
        tracker->station[i].Position[1] = 0.0f;
        tracker->station[i].Position[2] = 0.0f;
            
        tracker->station[i].Orientation[0] = 0.0f; 
        tracker->station[i].Orientation[1] = 0.0f; 
        tracker->station[i].Orientation[2] = 0.0f; 
        tracker->station[i].Orientation[3] = 0.0f; 

        tracker->station[i].TimeStamp = 0.0f;

        memset((void *)tracker->station[i].ButtonState, 0, sizeof(tracker->station[i].ButtonState));
        memset((void *)tracker->station[i].AnalogData, 0, sizeof(tracker->station[i].AnalogData));
    }
    
    if(!ISD_detectTracker(tracker, commPort, baud))
    {
        /* failed to detect an InterSense tracking device */
        return FAIL;
    }

    if(tracker->hardware == ISD_INTERTRAX_SERIES)
    {
        tracker->state.transmitMode = ISD_INTERTRAX_SAMPLE_MODE;
        return PASS;
    }

    /* switch to polled mode during the configuration session */
    itSendCommand(tracker, "c");

    /* set default station configuration */
    for (i = 1, status = TRUE; i <= ISD_MAX_STATIONS && status;  i++)
    {
        /* no joystick button state unless requested by application */
        Station.GetButtons = FALSE;

        /* no analog joystick data */
        Station.GetAnalogData = FALSE;

        /* no time stamp unless requested by application */
        Station.TimeStamped = FALSE;
        Station.AngleFormat = ISD_EULER;

        ISD_setOutputRecordList(tracker, &Station, i);  

        status = itComUpdateOutputList(tracker, i);
    }
    return PASS;
}


/*****************************************************************************
*
*   functionName:   ISD_closeTracker
*   Description:
*   Created:        12/3/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
BOOL ISD_closeTracker(InterSenseTrackerType *tracker)
{
    /* switch to polled mode */
    itSendCommand(tracker, "c");
    tdelay(0.1f);

    rs232DeinitCommunications(&(tracker->CommPort));
    tracker->hardware = ISD_NONE;
    return PASS;
}


/*****************************************************************************
*
*   functionName:   ISD_getCommState
*   Description:
*   Created:        12/4/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
BOOL ISD_getCommState(InterSenseTrackerType *tracker, 
                                 ISD_TRACKER_TYPE *Tracker)
{
    WORD systemModel[MAX_HARDWARE_VERSIONS] =
              {ISD_IS300, ISD_IS600, ISD_IS900, ISD_INTERTRAX, ISD_UNKNOWN};
    //BOOL status = PASS;

    Tracker->TrackerType = tracker->hardware;  
    
    if(tracker->hardware == ISD_PRECISION_SERIES)
    {
        Tracker->TrackerModel = systemModel[toIndex(tracker->state.hardwareVersion)];   
    }
    else
    {
        Tracker->TrackerModel = ISD_INTERTRAX;   
    }

    Tracker->CommPort      = tracker->CommPort.portNumber;
    Tracker->RecordsPerSec = tracker->RecordsPerSec;
    Tracker->KBitsPerSec   = tracker->KBitsPerSec;

    return PASS;
}


/*****************************************************************************
*
*   functionName:   ISD_getTrackerState
*   Description:
*   Created:        12/4/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
BOOL ISD_getTrackerState(InterSenseTrackerType *tracker, 
                         ISD_TRACKER_TYPE *Tracker, BOOL verbose)
{
   BOOL status = PASS;
        
    WORD systemModel[MAX_HARDWARE_VERSIONS] =
              {ISD_IS300, ISD_IS600, ISD_IS900, ISD_INTERTRAX, ISD_UNKNOWN};
    
    Tracker->TrackerType = tracker->hardware;  
    
    if(tracker->hardware == ISD_PRECISION_SERIES)
    {
        Tracker->TrackerModel = systemModel[toIndex(tracker->state.hardwareVersion)];   
    }
    else
    {
        Tracker->TrackerModel = ISD_INTERTRAX;   
    }

    Tracker->CommPort      = tracker->CommPort.portNumber;
    Tracker->RecordsPerSec = tracker->RecordsPerSec;
    Tracker->KBitsPerSec   = tracker->KBitsPerSec;
  
    if(tracker->hardware == ISD_PRECISION_SERIES)
    {
        if(verbose)
            ISD_printf(tracker, "Reading tracker system information ... ");

        if((status = itComUpdateStatusRecord(tracker, IT_COM_STATUS_REC_TIMEOUT)) == PASS)
        {
            /* genlock feature is available with firmware versions 3.016 and above */
            if(tracker->state.firmwareVersion >= 3.0160f)
            {
                /* get sync state */
                status = itComUpdateSyncState(tracker);

                Tracker->SyncState = tracker->SyncState;  
                Tracker->SyncRate  = tracker->SyncRate;     
                Tracker->SyncPhase = tracker->SyncPhase; 
            }
        }

        if(verbose)
        {
            ISD_printf(tracker, "%s\n", status == PASS ? "done" : "failed");
        }
    }

    return status;
}


/*****************************************************************************
*
*   functionName:   ISD_setTrackerState
*   Description:
*   Created:        12/4/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
BOOL ISD_setTrackerState(InterSenseTrackerType *tracker, 
                         ISD_TRACKER_TYPE *Tracker, BOOL verbose)
{
    //BOOL status = FAIL;

    if(tracker->hardware == ISD_PRECISION_SERIES)
    {
        /* genlock is supported by firmware versions 3.0160 and higher */
        if(tracker->state.firmwareVersion >= 3.0160f)
        {
            /* send sync command */
            itSendCommand(tracker, "MG%d,%.2f\r\n", (int) Tracker->SyncState, (float) Tracker->SyncRate);
            itSendCommand(tracker, "MGP%d\r\n", (int) Tracker->SyncPhase);
        }
    }

    return ISD_getTrackerState(tracker, Tracker, verbose);
}


/*****************************************************************************
*
*   functionName:   ISD_getStationState
*   Description:
*   Created:        12/4/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
BOOL ISD_getStationState(InterSenseTrackerType *tracker, 
                         ISD_STATION_CONFIG_TYPE *Station,
                         WORD stationNum, BOOL verbose)
{
    BOOL status = TRUE;
    int i;

    /* this function should only be used with IS series devices  */
    if(tracker->hardware != ISD_PRECISION_SERIES)
    {
        return FAIL;
    }

    if(verbose)
        ISD_printf(tracker, "Reading configuration for station %d ... ", (int) stationNum);

    Station->ID = stationNum;

    status = itComUpdateOutputList(tracker, stationNum);
    if(status)
    {
        Station->GetButtons = FALSE;
        Station->GetAnalogData = FALSE;
        Station->TimeStamped = FALSE;
        Station->AngleFormat = ISD_EULER;

        /* look for the time stamp item in the output record list */
        for(i=0; i<MAX_OUTPUT_LIST_ITEMS && tracker->station[stationNum-1].outputList[i] != -1; i++)
        {
            if(tracker->station[stationNum-1].outputList[i] == 21)
            {
                Station->TimeStamped = TRUE;
            }
            if(tracker->station[stationNum-1].outputList[i] == 22)
            {
                Station->GetButtons = TRUE;
            }
            if(tracker->station[stationNum-1].outputList[i] == 23)
            {
                Station->GetAnalogData = TRUE;
            }              
            if(tracker->station[stationNum-1].outputList[i] == 4)
            {
                Station->AngleFormat = ISD_EULER;
            }
            if(tracker->station[stationNum-1].outputList[i] == 11)
            {
                Station->AngleFormat = ISD_QUATERNION;
            }   
        }

        status = itComUpdateStationStat(tracker, stationNum);
        if(status)
        {
            Station->State = tracker->station[stationNum-1].state;

            status = itComUpdateSensorStat(tracker, stationNum);
            if(status)
            {
                Station->Enhancement = tracker->station[stationNum-1].enhancement;
                Station->Compass     = tracker->station[stationNum-1].compass;

                status = itComUpdatePredictionString(tracker, stationNum);
                if(status)
                {
                    Station->Prediction = tracker->station[stationNum-1].prediction;

                    /* InertiaCube assignment is a feature of firmware 3.0 and above */
                    if(tracker->state.firmwareVersion >= 3.0f)
                    {
                        status = itComUpdateImu(tracker, stationNum);
                        if(status)
                        {
                            Station->InertiaCube = tracker->station[stationNum-1].InertiaCube;

                            status = itComUpdateSensitivityVal(tracker, stationNum);
                            if(status)
                            {
                                Station->Sensitivity = tracker->station[stationNum-1].sensitivity;
                            }
                        }
                    }
                    else
                    {
                        Station->InertiaCube = 1;
                        Station->Sensitivity = 1;
                    }
                }
            }
        }
    }
    
    if(verbose)
        ISD_printf(tracker, "%s\n", status == PASS ? "done" : "failed");

    return status;
}


/*****************************************************************************
*
*   functionName:   ISD_setOutputRecordList
*   Description:    set output record list for a station
*   Created:        12/8/98
*   Author:         Yury Altshuler
*
*   Comments:       Output record list is determined by the tracker model,
*                   firmware version, and time stamp request status.
*
******************************************************************************/
BOOL ISD_setOutputRecordList (InterSenseTrackerType *tracker, 
                              ISD_STATION_CONFIG_TYPE *Station,
                              WORD stationNum)
{
    char command[100];
    int pos = 0;

    /* this function should only be used with IS series devices  */
    if(tracker->hardware != ISD_PRECISION_SERIES)
    {
        return FAIL;
    }

    /* only use time stamps if firmware version is greater than 3.0 */
    if(tracker->state.firmwareVersion >= 3.0f)
    {
        switch(tracker->state.hardwareVersion)
        {
            case IS300:
                pos += sprintf(&command[pos], "O%c,%d", stationToChar(stationNum),
                        Station->AngleFormat == ISD_QUATERNION ? 11 : 4);
                break;

            case IS600:
            case IS900:
                pos += sprintf(&command[pos], "O%c,2,%d", stationToChar(stationNum),
                        Station->AngleFormat == ISD_QUATERNION ? 11 : 4);
                break;
        }

        if(Station->TimeStamped) pos += sprintf(&command[pos], ",21");

        if(tracker->state.hardwareVersion & IS900)
        {
            if(Station->GetButtons)     pos += sprintf(&command[pos], ",22");
            if(Station->GetAnalogData)  pos += sprintf(&command[pos], ",23");
        }

        pos += sprintf(&command[pos], ",1\r\n");
        
        itSendCommand(tracker, command);
    }

    /* firmware versions prior to 3.0 did not have hardware identifier, so it is not
       possible to determine if the tracker is 3 or 6 DOF. Also, not all versions 
       supported time stamp */
    else
    {
        itSendCommand(tracker, "O%c,2,%d,1\r\n", stationToChar(stationNum),
            Station->AngleFormat == ISD_QUATERNION ? 11 : 4);
    }

    return TRUE;
}


/*****************************************************************************
*
*   functionName:   ISD_setStationState
*   Description:
*   Created:        12/4/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
BOOL ISD_setStationState (InterSenseTrackerType *tracker, 
                          ISD_STATION_CONFIG_TYPE *Station,
                          WORD stationNum, BOOL verbose, BOOL apply)
{
    ISD_STATION_CONFIG_TYPE PrevStationState;

    /* this function should only be used with IS series devices  */
    if(tracker->hardware != ISD_PRECISION_SERIES)
    {
        return FAIL;
    }

    /* start by obtaining current state of the station */
    if(ISD_getStationState(tracker, &PrevStationState, stationNum, FALSE))
    {
        /* check if any changes are requested */
        if(memcmp((void *)&PrevStationState, 
                   (void *)&Station, sizeof(ISD_STATION_CONFIG_TYPE)) != 0)
        {
            if(verbose)
                ISD_printf(tracker, "Sending configuration for station %d ... ", (int) stationNum);

            /* set output record list */
            ISD_setOutputRecordList(tracker, Station, stationNum);

            itSendCommand(tracker, "Mp%c,%d\r\n", stationToChar(stationNum), (short)Station->Prediction);
            itSendCommand(tracker, "MQ%c,%d\r\n", stationToChar(stationNum), (short)Station->Sensitivity);
            itSendCommand(tracker, "MF%c,%d\r\n", stationToChar(stationNum), (short)Station->Enhancement);

            if(Station->State)   itSendCommand(tracker, "l%c,1\r\n", stationToChar(stationNum));
            else                 itSendCommand(tracker, "l%c,0\r\n", stationToChar(stationNum));

            if(tracker->state.firmwareVersion < 3.0160f)
            {    
                if(Station->Compass != 0) itSendCommand(tracker, "MH%c\r\n", stationToChar(stationNum));
                else                      itSendCommand(tracker, "Mh%c\r\n", stationToChar(stationNum));
            }
            else
            {
                itSendCommand(tracker, "MH%c,%d\r\n", stationToChar(stationNum), (int) Station->Compass);
            }

            /* only send InertiaCube command if different from current configuration */
            if(PrevStationState.InertiaCube != Station->InertiaCube)
            {
                /* disassociate InertiaCube from the station */
                if(Station->InertiaCube == -1 && PrevStationState.InertiaCube > 0)
                {
                    itSendCommand(tracker, "MCi%c,%d\r\n", stationToChar(stationNum), 
                        PrevStationState.InertiaCube);

                    if(apply)
                    {
                        ISD_applyConfiguration(tracker, verbose);
                    }
                    else
                    {
                        tracker->applyPSEConfigPending = TRUE;
                    }
                }
                if(Station->InertiaCube > 0)
                {
                    itSendCommand(tracker, "MCI%c,%d\r\n", stationToChar(stationNum), 
                        Station->InertiaCube);
                    
                    if(apply)
                    {
                        ISD_applyConfiguration(tracker, verbose);
                    }
                    else
                    {
                        tracker->applyPSEConfigPending = TRUE;
                    }
                }
            }

            if(verbose)
                ISD_printf(tracker, "done\n");

            /* confirm new configuration */
            return(ISD_getStationState(tracker, Station, stationNum, verbose));
        }
    }
    return FAIL;
}


/*****************************************************************************
*
*   functionName:   ISD_configureTracker
*   Description:    Configure tracker for best performance
*   Created:        12/30/98
*   Author:         Yury Altshuler
*
*   Comments:       
*
******************************************************************************/
BOOL ISD_configureTracker(InterSenseTrackerType *tracker, BOOL verbose)
{
    BOOL status;
    ISD_TRACKER_TYPE Tracker;
    char c;

    /* this function should only be used with IS series devices  */
    if(tracker->hardware != ISD_PRECISION_SERIES)
    {
        return FAIL;
    }

    /* output record list for stations is first set in ISD_openTracker, then
       in ISD_configureFromFile. In both cases settings are confirmed and there is
       no need to repeat that here. */
    
    if(verbose)
       ISD_printf(tracker, "Finalizing tracker configuration ... ");

    /* binary mode */
    itSendCommand(tracker, "f");

    /* centimeters */
    itSendCommand(tracker, "u");

#ifdef USE_POLLED_MODE
    /* polled mode */
    itSendCommand(tracker, "c");
#else
    /* continuous mode */
    itSendCommand(tracker, "C");
#endif

    /* make sure output is not suspended */
    c = 'Q' - 'A' + 1; /* RESUME command */
    itSendCommand(tracker, &c);

    /* get active station list, particularly important in polled mode */
    status = itComUpdateStationStat(tracker, 1);

    /* Get system status parameters so that data records can be
       correctly interpreted */

    status = ISD_getTrackerState(tracker, &Tracker, FALSE);

    if(verbose)
        ISD_printf(tracker, "%s\n", status == PASS ? "done" : "failed");

    return status;
}


/*****************************************************************************
*
*   functionName:   ISD_applyConfiguration
*   Description:
*   Created:        12/23/98
*   Author:         Yury Altshuler
*
*   Comments:       apply PSE configuration   
*
******************************************************************************/
BOOL ISD_applyConfiguration(InterSenseTrackerType *tracker, BOOL verbose)
{
    float startTime;
    BOOL status = PASS;

    if(tracker->applyPSEConfigPending)
    {
        if(verbose)
        {
            ISD_printf(tracker, "Tracker configration changed, sending MCe command to apply ... ");
        }

        itSendCommand(tracker, "MCe\r\n");

        /* wait no more that 7 seconds for tracker to finish configuration */
        for(startTime = timeNow(); timeNow() - startTime < 7.0;)
        {
            if((status = itComUpdateIntrackSystemStat(tracker, 0.5f)) == PASS)
            {
                break;
            }
        }
        if(verbose)
        {
            if(status)  ISD_printf(tracker, "done\n");
            else        ISD_printf(tracker, "failed\n");
        }            
    }
    return status;
}


/*****************************************************************************
*
*   functionName:   ISD_allowUserCommand
*   Description:
*   Created:        12/23/98
*   Author:         Yury Altshuler
*
*   Comments:       Check if this configuration file or script command is allowed   
*
******************************************************************************/
BOOL ISD_allowUserCommand(char *cmd)
{
    switch(*cmd)
    {
        /* following configuration parameters are determined by the DLL and should 
           not be changed by the user */

        case 'O':   /* output record list */
        case 'C':   /* continuous mode */
        case 'c':   /* polled mode */
        case 'F':   /* ASCII mode */
        case 'f':   /* binary mode */
        case 'o':   /* communication parameters */
        case 'U':   /* units inches */
        case 'u':   /* units centimeters */
            return FALSE;

        default:
            return TRUE;
    }
}


/***************************************************************************/
void ISD_printf(InterSenseTrackerType *tracker, char *fs,...)
{
    char sbuf[256];
    va_list argptr;

    va_start(argptr,fs);
    vsprintf(sbuf, fs,argptr);
    va_end(argptr);

    printf("%s", sbuf);
}


/******************************************************************************/
int itComLogCount( InterSenseTrackerType *tracker )
{
    if( tracker->log.head >= tracker->log.tail )
    {
        return tracker->log.head - tracker->log.tail;
    }
    else
    {
        return MAX_LOG - tracker->log.tail + tracker->log.head;
    }
}


/*******************************************************************************/
int ISD_GetRecordLog( ISD_TRACKER_HANDLE handle, char *records[] )
{
    InterSenseTrackerType *tracker;
    int i, count = 0, index;

    if( handle > 0 && handle <= ISD_MAX_TRACKERS )
    {
        tracker = &ISD_tracker[ handle - 1 ];
        serviceSerialPort(tracker);

        count = itComLogCount( tracker );
        index = tracker->log.head;
        
        for( i = 0; i < count; i++ )
        {
            if( --index < 0 ) index = MAX_LOG - 1;
            records[i] = tracker->log.records[index];
        }
    }
    return count;
}
