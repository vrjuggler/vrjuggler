/***********************************************************************
*
*       File Name:      isense.h
*       Description:    Header File for InterSense Tracker Driver
*       Created:        12/4/99
*       Author:         Yury Altshuler
*       Contact:        yurya@isense.com, 781-270-0090 ext. 124
*
*       Copyright:      InterSense 1999 - All rights Reserved.
*
*       Comments:       This program supports IS-300, IS-600, IS-900 and InterTrax.
*                       It can detect, configure, and get data from up to 4 trackers.
*                       InterTrax does not require any cofiguration changes, so most
*                       of the functions can be ignored.
*
*                       
************************************************************************/
#ifndef _ISD_isenseh
#define _ISD_isenseh

/* if not Win32 then it must be UNIX, no other OS is supported  */
#if !defined __WIN32__ && !defined WIN32 && !defined _Windows && !defined _WIN32
#define UNIX
#endif

/* -------------------------------------- */
/* Do not change anything below this line */
/* -------------------------------------- */

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

#define MAX_BAUD_RATE 115200

#if defined UNIX
    typedef unsigned long       DWORD;
    typedef int                 BOOL;
    typedef unsigned char       BYTE;
    typedef unsigned short      WORD;
    typedef float               FLOAT;
    typedef long                LONG;
    typedef int                 INT;
#else
#   include <windows.h>
#   include <winbase.h>
#endif

/* tracking system type */
#define ISD_NONE               0    /* none found, or can't identify */
#define ISD_PRECISION_SERIES   1    /* IS-300, IS-600 and IS-900 */
#define ISD_INTERTRAX_SERIES   2    /* InterTrax */

/* tracking system hardware model */
#define ISD_UNKNOWN     0    
#define ISD_IS300       1    /* 3DOF system */
#define ISD_IS600       2    /* 6DOF system */
#define ISD_IS900       3    /* 6DOF system */
#define ISD_INTERTRAX   4    /* InterTrax */

#define ISD_MAX_STATIONS       8

/* assume no more than 4 rs232 ports */
#define ISD_MAX_TRACKERS       4 

/* orientation format */
#define ISD_EULER              1
#define ISD_QUATERNION         2

/* number of supported stylus buttons */
#define MAX_NUM_BUTTONS        8

/* hardware is limited to 10 analog channels per station */
#define MAX_ANALOG_CHANNELS    10

#define MAX_LOG                50
#define MAX_RECORD_LENGTH      100

typedef int ISD_TRACKER_HANDLE;


typedef struct
{
    /* Following item are for information only and should not be changed */

    WORD   TrackerType;    /* IS Precision series or InterTrax. 
                              TrackerType can be: 
                              ISD_PRECISION_SERIES for IS-300, IS-600 and IS-900 model tracker, 
                              ISD_INTERTRAX_SERIES for InterTrax, or 
                              ISD_NONE if tracker is not initialized */

    WORD   TrackerModel;   /* ISD_UNKNOWN, ISD_IS300, ISD_IS600, ISD_IS900, ISD_INTERTRAX */
    
    WORD   CommPort;       /* Number of the rs232 port. Starts with 1. */

    /* Communications statistics. For information only. */

    WORD   RecordsPerSec;
    float  KBitsPerSec;    

    /* Following items are used to configure the tracker and can be set in
       the isenseX.ini file */

    WORD   SyncState;   /* 4 states: 0 - OFF, system is in free run 
                                     1 - ON, hardware genlock frequency is automatically 
                                         determined (if supported by the firmware)
                                     2 - ON, hardware genlock frequency is specified by the user
                                     3 - ON, no hardware sygnal, lock to the user specified frequency */ 

    float  SyncRate;    /* Sync frequency - number of hardware sync signals per second, 
                           or, if SyncState is 3 - data record output frequency */

    WORD   SyncPhase;   /* 0 to 100 per cent */

} ISD_TRACKER_TYPE;



/* ISD_STATION_CONFIG_TYPE can only be used with IS Precision Series tracking devices.
   If passed to ISD_SetStationState or ISD_GetStationState with InterTrax, FALSE is returned. */

typedef struct
{
    WORD    ID;             /* unique number identifying a station. It is the same as that 
                               passed to the ISD_SetStationState and ISD_GetStationState   
                               functions and can be 1 to ISD_MAX_STATIONS */

    BOOL    State;          /* TRUE if ON, FALSE if OFF */

    BOOL    Compass;        /* 0, 1 or 2 for OFF, PARTIAL and FULL. Older versions of tracker
                               firmware supported only 0 and 1, which stood for ON or OFF. 
                               Please use the new notation. This API will correctly interpret 
                               the settings. Compass setting is ignored if station is configured 
                               for Fusion Mode operation */

    LONG    InertiaCube;    /* InertiaCube associated with this station. If no InertiaCube is
                               assigned, this number is -1. Otherwise, it is a positive number
                               1 to 4 */

    WORD    Enhancement;    /* levels 0, 1, or 2 */
    WORD    Sensitivity;    /* levels 1 to 4 */
    WORD    Prediction;     /* 0 to 50 ms */
    WORD    AngleFormat;    /* ISD_EULER or ISD_QUATERNION */
    BOOL    TimeStaMped;    /* TRUE if time stamp is requested */
    BOOL    GetButtons;     /* TRUE if joystick or stylus button state is requested */
    BOOL    GetAnalogData;  /* TRUE if analog joystick data is requested */

} ISD_STATION_CONFIG_TYPE;


typedef struct
{
    float Orientation[4];   /* Supports both Euler and Quaternion formats */
    float Position[3];      /* Always in meters */
    float TimeStaMp;        /* Only if requested */
    int   ButtonState[MAX_NUM_BUTTONS];  /* Only if requested */

    /* Current hardware is limited to 10 channels, only 2 are used. 
       The only device using this is the IS-900 wand that has a built-in
       analog joystick. Channel 1 is x-axis rotation, channel 2 is y-axis
       rotation */

    short  AnalogData[MAX_ANALOG_CHANNELS]; /* only if requested */

} ISD_STATION_DATA_TYPE;


typedef struct
{
 ISD_STATION_DATA_TYPE Station[ISD_MAX_STATIONS];

} ISD_DATA_TYPE;


/* Returns -1 on failure. To detect tracker automatically specify 0 for commPort */
ISD_TRACKER_HANDLE ISD_OpenTrackerPort( const char* commPort, BOOL infoScreen, BOOL verbose, DWORD baud );
ISD_TRACKER_HANDLE ISD_OpenTracker( DWORD commPort, BOOL infoScreen, BOOL verbose, DWORD baud );


BOOL  ISD_CloseTracker( ISD_TRACKER_HANDLE );

BOOL  ISD_GetTrackerState( ISD_TRACKER_HANDLE, ISD_TRACKER_TYPE *, BOOL verbose );
BOOL  ISD_SetTrackerState( ISD_TRACKER_HANDLE, ISD_TRACKER_TYPE *, BOOL verbose );


/* Get RecordsPerSec and KBitsPerSec without requesting genlock settings from the tracker.
   Use this instead of ISD_GetTrackerState to prevent your program from stalling while
   waiting for the tracker response. */

BOOL  ISD_GetCommState( ISD_TRACKER_HANDLE, ISD_TRACKER_TYPE * );


/* stationNum is a number from 1 to ISD_MAX_STATIONS */
BOOL  ISD_SetStationState( ISD_TRACKER_HANDLE, ISD_STATION_CONFIG_TYPE *, 
                           WORD stationNum, BOOL verbose );

BOOL  ISD_GetStationState( ISD_TRACKER_HANDLE, ISD_STATION_CONFIG_TYPE *, 
                           WORD stationNum, BOOL verbose );

/* Get data from all configured stations. */
BOOL  ISD_GetTrackerData( ISD_TRACKER_HANDLE, ISD_DATA_TYPE * );


/* Send a configuration script to the tracker. Script must consist of valid commands as
   described in the interface protocol. Commands in the script should be terminated by
   the New Line character '\n'. Line Feed character '\r' is added by the function and is
   not required. */

BOOL  ISD_SendScript( ISD_TRACKER_HANDLE, char * );
 
/* Returns number of configured tracking devices */
BOOL  ISD_NumOpenTrackers( WORD * );

/* returns number of records in the log. Keeps at most 50 most recent records */
int ISD_GetRecordLog( ISD_TRACKER_HANDLE, char *records[] );

#endif
