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
#include <string>

#include <vpr/vpr.h>

/* if not Win32 then it must be UNIX, no other OS is supported  */
//#if !defined __WIN32__ && !defined WIN32 && !defined _Windows && !defined _WIN32 && !defined _WIN64
//#define UNIX 

/* Following defines the flavor of UNIX the program should be compiled for.
   Only one of the 3 lines can be used, the others should be commented out. */

//#define IRIX        /* use SGI-specific serial port configuration */
/* #define SOLARIS */   /* SUN Solaris OS */
/*#define LINUX*/

#if !defined VPR_OS_IRIX  /* Win32 always has RTS control available */
#define USE_RTS_LINE
#endif

/* Serial port initialization varies for diffent vesions of IRIX operating systems.
   If you have an older SGI, remove the comments from around the following line. I don't
   have the actual models and IRIX version numbers, so it may be necessary to try both
   with and without this flag. */

/* #define LEGACY_SGI */


/* handle difference between little and big endian systems */
#if defined VPR_OS_IRIX || defined VPR_OS_Solaris
#define REVERSE_BYTE_ORDER
#endif

#if (defined VPR_OS_IRIX && (defined VPR_OS_Linux || defined VPR_OS_Solaris)) || (defined VPR_OS_Linux && defined VPR_OS_Solaris)
#error More than one UNIX version is defined
#endif

/* maximum baud rate allowed by the operating system. Older SGI computers did not
   support 115200 */

// FIX THIS
/*#if defined UNIX && defined LEGACY_SGI
#define MAX_BAUD_RATE  38400L
#else
*/
#define MAX_BAUD_RATE  115200L
//#endif


/* if using a slower SGI system, serial port processing may be too taxing for the
   processor, so it may be preferable to run in polled mode. Continuous mode, however,
   is highly recommended for best performance. Win32 platform has efficient serial 
   port driver that handles any throughput without effecting applications. */

#if defined VPR_OS_IRIX
/* #define USE_POLLED_MODE */
#endif



/* -------------------------------------- */
/* Do not change anything below this line */
/* -------------------------------------- */

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

#ifdef VPR_OS_Windows
#include <windows.h>
#else

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef long                LONG;
typedef int                 INT;
#include "string.h"

#if defined VPR_OS_IRIX
#include "unistd.h"
#endif

#endif

/* tracking system type */
typedef enum
{
    ISD_NONE = 0,           /* none found, or can't identify */
    ISD_PRECISION_SERIES,   /* IS-300, IS-600 and IS-900 */
    ISD_INTERTRAX_SERIES    /* InterTrax */

} ISD_SYSTEM_TYPE;



/* tracking system model */
typedef enum
{
    ISD_UNKNOWN = 0,          
    ISD_IS300,          /* 3DOF system */
    ISD_IS600,          /* 6DOF system */
    ISD_IS900,          /* 6DOF system */  
    ISD_INTERTRAX,      /* InterTrax (Serial) */
    ISD_INTERTRAX_2     /* InterTrax (USB) */

} ISD_SYSTEM_MODEL;


#define ISD_MAX_STATIONS       8

/* assume no more than 4 rs232 ports */
#define ISD_MAX_TRACKERS       4 

/* orientation format */
#define ISD_EULER              1
#define ISD_QUATERNION         2

/* Coordinate frame in which position and orientation data is reported */
#define ISD_DEFAULT_FRAME      1    /* InterSense default */
#define ISD_VSET_FRAME         2    /* Virtual set frame, use for camera tracker only */

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

    float  LibVersion;     /* InterSense Library version */

    DWORD  TrackerType;    /* IS Precision series or InterTrax. 
                              TrackerType can be: 
                              ISD_PRECISION_SERIES for IS-300, IS-600 and 
                              IS-900 model tracker, ISD_INTERTRAX_SERIES for 
                              InterTrax, or ISD_NONE if tracker is not initialized */

    DWORD  TrackerModel;   /* ISD_UNKNOWN, ISD_IS300, ISD_IS600, ISD_IS900, ISD_INTERTRAX */
    
    DWORD  CommPort;       /* Number of the rs232 port. Starts with 1. */

    /* Communications statistics. For information only. */

    DWORD  RecordsPerSec;
    float  KBitsPerSec;    

    /* Following items are used to configure the tracker */

    DWORD  SyncState;   /* 4 states: 0 - OFF, system is in free run 
                                     1 - ON, hardware genlock frequency is automatically determined
                                     2 - ON, hardware genlock frequency is specified by the user
                                     3 - ON, no hardware sygnal, lock to the user specified frequency */ 

    float  SyncRate;    /* Sync frequency - number of hardware sync signals per second, 
                           or, if SyncState is 3 - data record output frequency */

    DWORD  SyncPhase;   /* 0 to 100 per cent */   

    DWORD  dwReserved1; /* reserved for future use */
    DWORD  dwReserved2;  
    DWORD  dwReserved3;  
    DWORD  dwReserved4;

    float  fReserved1;  
    float  fReserved2;
    float  fReserved3;
    float  fReserved4;

    BOOL   bReserved1;
    BOOL   bReserved2;
    BOOL   bReserved3;
    BOOL   bReserved4;

} ISD_TRACKER_INFO_TYPE;



/* ISD_STATION_INFO_TYPE can only be used with IS Precision Series tracking devices.
   If passed to ISD_SetStationConfig or ISD_GetStationConfig with InterTrax, FALSE is returned. */

typedef struct
{
    DWORD   ID;             /* unique number identifying a station. It is the same as that 
                               passed to the ISD_SetStationConfig and ISD_GetStationConfig   
                               functions and can be 1 to ISD_MAX_STATIONS */

    BOOL    State;          /* TRUE if ON, FALSE if OFF */

    BOOL    Compass;        /* 0, 1 or 2 for OFF, PARTIAL and FULL. Older versions of tracker
                               firmware supported only 0 and 1, which stood for ON or OFF. Please
                               use the new notation. This API will correctly interpret the settings.
                               Compass setting is ignored if station is configured for 
                               Fusion Mode operation. */

    LONG    InertiaCube;    /* InertiaCube associated with this station. If no InertiaCube is
                               assigned, this number is -1. Otherwise, it is a positive number
                               1 to 4 */

    DWORD   Enhancement;    /* levels 0, 1, or 2 */
    DWORD   Sensitivity;    /* levels 1 to 5 */
    DWORD   Prediction;     /* 0 to 50 ms */
    DWORD   AngleFormat;    /* ISD_EULER or ISD_QUATERNION */
    
    BOOL    TimeStamped;    /* TRUE if time stamp is requested */
    BOOL    GetButtons;     /* TRUE if wand or stylus button state is requested */
    BOOL    GetCameraDataRaw;  /* TRUE if encoder data is requested */
    BOOL    GetCameraDataComputed;  /* TRUE if encoder data is requested */

    DWORD   CoordFrame;     /* coord frame in which position and orientation data is reported */ 
    DWORD   dwReserved2;    /* reserved for future use */
    DWORD   dwReserved3;     
    DWORD   dwReserved4;

    float   fReserved1;  
    float   fReserved2;
    float   fReserved3;
    float   fReserved4;

    BOOL    GetAnalogData;  /* TRUE if analog joystick data is requested */
    BOOL    bReserved2;     
    BOOL    bReserved3;
    BOOL    bReserved4;

} ISD_STATION_INFO_TYPE;


typedef struct
{
    BOOL    NewData;          /* True only if this data has not been reported before */
    float   Orientation[4];   /* Supports both Euler and Quaternion formats */
    float   Position[3];      /* Always in meters */
    float   TimeStamp;        /* Seconds, reported only if requested */
    BOOL    ButtonState[MAX_NUM_BUTTONS];    /* Only if requested */

    /* Current hardware is limited to 10 channels, only 2 are used. 
       The only device using this is the IS-900 wand that has a built-in
       analog joystick. Channel 1 is x-axis rotation, channel 2 is y-axis
       rotation */

    short  AnalogData[MAX_ANALOG_CHANNELS]; /* only if requested */

} ISD_STATION_DATA_TYPE;


typedef struct
{
    BYTE    Status;             /* tracking status byte */
    BYTE    bReserved1;         /* pack to 4 byte boundary */
    BYTE    bReserved2;
    BYTE    bReserved3;

    BOOL    NewData;            /* True only if this data has not been reported before */
    DWORD   Timecode;           /* timecode, not implemented yet */
    LONG    ApetureEncoder;     /* Apeture encoder counts, relative to last reset of power up */
    LONG    FocusEncoder;       /* Focus encoder counts */
    LONG    ZoomEncoder;        /* Zoom encoded counts */
    DWORD   TimeCodeUserBits;   /* Time code user bits, not implemented yet */

    float   Apeture;            /* Computed Apeture value */
    float   Focus;              /* Computed focus value (mm), not implemented yet */
    float   FOV;                /* Computed vertical FOV value (degrees) */
    float   NodalPoint;         /* Nodal point offset due to zoom and focus (mm) */

    LONG    lReserved1;
    LONG    lReserved2;
    LONG    lReserved3;
    LONG    lReserved4;

    DWORD   dwReserved1;
    DWORD   dwReserved2;
    DWORD   dwReserved3;
    DWORD   dwReserved4;

    float   fReserved1;
    float   fReserved2;
    float   fReserved3;
    float   fReserved4;

} ISD_CAMERA_ENCODER_DATA_TYPE;


typedef struct
{
    ISD_STATION_DATA_TYPE Station[ISD_MAX_STATIONS];

} ISD_DATA_TYPE;

typedef struct
{
    ISD_CAMERA_ENCODER_DATA_TYPE Camera[ISD_MAX_STATIONS];

} ISD_CAMERA_DATA_TYPE;



/* Returns -1 on failure. To detect tracker automatically specify 0 for commPort */
ISD_TRACKER_HANDLE ISD_OpenTracker( std::string commPort, int baud_rate, BOOL infoScreen, BOOL verbose );

BOOL  ISD_CloseTracker( ISD_TRACKER_HANDLE );

BOOL  ISD_GetTrackerConfig( ISD_TRACKER_HANDLE, ISD_TRACKER_INFO_TYPE *, BOOL verbose );
BOOL  ISD_SetTrackerConfig( ISD_TRACKER_HANDLE, ISD_TRACKER_INFO_TYPE *, BOOL verbose );


/* Get RecordsPerSec and KBitsPerSec without requesting genlock settings from the tracker.
   Use this instead of ISD_GetTrackerConfig to prevent your program from stalling while
   waiting for the tracker response. */

BOOL  ISD_GetCommInfo( ISD_TRACKER_HANDLE, ISD_TRACKER_INFO_TYPE * );


/* stationNum is a number from 1 to ISD_MAX_STATIONS */
BOOL  ISD_SetStationConfig( ISD_TRACKER_HANDLE, ISD_STATION_INFO_TYPE *, 
                            WORD stationNum, BOOL verbose );

BOOL  ISD_GetStationConfig( ISD_TRACKER_HANDLE, ISD_STATION_INFO_TYPE *, 
                            WORD stationNum, BOOL verbose );

/* Get data from all configured stations. */
BOOL  ISD_GetTrackerData( ISD_TRACKER_HANDLE, ISD_DATA_TYPE * );

/* Get camera encoder data from all configured stations, must call ISD_GetTrackerData before
   this function to service serial port */
BOOL  ISD_GetCameraData( ISD_TRACKER_HANDLE, ISD_CAMERA_DATA_TYPE * );

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
