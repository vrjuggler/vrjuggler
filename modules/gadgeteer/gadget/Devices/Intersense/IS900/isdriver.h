/***********************************************************************
*
*       File Name:      isdemo.h
*       Description:    Header File for InterSense Tracker Testing Program
*       Created:        1998
*       Author:         Yury Altshuler
*
*       Copyright:      InterSense 1999 - All rights Reserved.
*
*       Comments:
*
*
************************************************************************/
#ifndef _ISD_isdriverh
#define _ISD_isdriverh

#include "isense.h"
#include "serial.h"
#include "wsock.h"

#include <vpr/vpr.h>

#ifdef VPR_OS_Win32
#include <windows.h>
#include <winbase.h>
#endif

#define ISLIB_VERSION 2.09

#define IS300       0x01    /* 3DOF system */
#define IS600       0x02    /* 6DOF system */
#define IS900       0x04    /* camera tracker */

#define IT_IS300           (1 << 2)
#define IT_IS300PRO        (IT_IS300 | PRO_SERIES)
#define IT_IS3000          (IT_IS300 | ONE_K_SERIES)
#define IT_IS3000PRO       (IT_IS3000 | PRO_SERIES)
#define IT_IS600           (2 << 2)
#define IT_IS600PRO        (IT_IS600 | PRO_SERIES)
#define IT_IS6000          (IT_IS600 | ONE_K_SERIES)
#define IT_IS6000PRO       (IT_IS6000 | PRO_SERIES)
#define IT_IS900           (4 << 2)
#define IT_IS9000          (IT_IS900 | ONE_K_SERIES)

#define LF          0X0A
#define CR          0X0D

#ifndef EOF
#define EOF         (-1)
#endif

/* Constants */
#define YAW     0
#define PITCH   1
#define ROLL    2

#define X_INDEX 0
#define Y_INDEX 1
#define Z_INDEX 2

#define PI      3.14159265359f

#define PASS    TRUE
#define FAIL    FALSE

#define OK      1
#define ERROR   0

#define HIGH    1
#define LOW     0

#define ON      1
#define OFF     0

#define MAX_NUM_STATIONS ISD_MAX_STATIONS
#define MAX_NUM_IMUS    4

#define MAX_COM_PORTS   4
#define MAX_BAUD_RATES  5

#define validStation(sNum) (sNum >= 0 && sNum < MAX_NUM_STATIONS)

/* convert hardware id to array index (just a lookup) */
#define toIndex(A)   (A==1?0: A==2?1: A==4?2: A==8?3: A==16?4: A==32?5: A==64?6: A==128?7: A==256?8: 0)

#define NELEM( array ) ( sizeof( array ) / sizeof( array[0] ) )

#define MIN( x, y ) ( (x) < (y) ? (x) : (y) )
#define MAX( x, y ) ( (x) > (y) ? (x) : (y) )
#define MINMAX( lo, x, hi ) ( MAX( lo, MIN( x, hi ) ) )


#define MAX_HARDWARE_VERSIONS       5

#define MAX_OUTPUT_LIST_ITEMS       20
#define IT_COM_NUM_LIST_CODES       26
#define IT_COM_NUM_BIT_ERROR_BYTES  3
#define IT_COM_NUM_VER_BYTES        6
#define IT_COM_NUM_ID_BYTES         32
#define NUM_SPATIAL_DIMS            3

#define PSE_APPLY_COMMAND           "MCe"
#define PSE_CANCEL_COMMAND          "MCx"

#define INPUT_BUFFER_SIZE           4096

typedef struct
{
    DWORD  state;

    DWORD  compass;      /* yaw comp */
    DWORD  jumpMode;     /* compatibility with old firmware */       
    DWORD  enhancement;  /* levels 0, 1, or 2 */
    DWORD  sensitivity;  /* levels 1 to 5 */
    DWORD  prediction;   /* 0 to 50 ms */
    LONG   InertiaCube;

    LONG   outputList[MAX_OUTPUT_LIST_ITEMS];

    float  Position[3];
    float  Orientation[4];
    float  TimeStamp;

    int    ButtonState[MAX_NUM_BUTTONS];
    short  AnalogData[MAX_ANALOG_CHANNELS]; 

    BYTE   Status;             /* tracking status byte */
    DWORD  Timecode;           
    LONG   ApetureEncoder;
    LONG   FocusEncoder;
    LONG   ZoomEncoder;
    DWORD  TimeCodeUserBits;
    float  Apeture;
    float  Focus;
    float  FOV;
    float  NodalPoint;

    DWORD  NewData;
    DWORD  NewCameraData;

}ItComSensorStateType;


typedef struct
{
    BYTE  productID;
    BYTE  hardwareVersion;
    BYTE  win32Platform;
    BYTE  boresightMode;
    BYTE  ledEnable;

    DWORD outputFormat;
    DWORD units;
    DWORD transmitMode;
    DWORD timeUnits;
    DWORD numActive;
    float firmwareVersion;

    char  bitError[IT_COM_NUM_BIT_ERROR_BYTES];
    char  ver[IT_COM_NUM_VER_BYTES+1];
    char  id[IT_COM_NUM_ID_BYTES+1];

} ItComSystemStateType;


typedef struct
{
    int head;
    int tail;
    char records[MAX_LOG][MAX_RECORD_LENGTH];

} ItComLogType;


typedef struct
{
    /* private variables */
    BYTE hardware;
    DWORD ID;

    COMM_PORT  CommPort;
    wsockType  wsock;
    
    ItComSystemStateType     state;
    ItComSensorStateType     station[MAX_NUM_STATIONS];
    ItComLogType             log;

    int numBytesReceived;
    int numSamplesReceived;

    WORD   RecordsPerSec;
    float  KBitsPerSec;    

    char   cmdbuf[INPUT_BUFFER_SIZE];
    char   cprev2, cprev1;

    int    nchars;
    float  recordStartTime;
    float  statsTime;

    DWORD  ItComStationStatus[MAX_NUM_STATIONS];
    DWORD  ItComImuFlag[MAX_NUM_STATIONS];
    DWORD  ItComStatusUpdateFlag;
    DWORD  ItComSensorUpdateFlag;
    DWORD  ItComSystemUpdateFlag;
    DWORD  ItComOutputListFlag;
    DWORD  ItComPredictionFlag;
    DWORD  ItComSensitivityValFlag;
    DWORD  ItComStationUpdateFlag;
    DWORD  ItComSyncRecordFlag;
    DWORD  RecvUpdateFlag;
    float  LastReqTime;

    BOOL   applyPSEConfigPending;

    /* user accessible variables */

    WORD   SyncState;   /* 4 states: 0 - OFF, system is in free run 
                                     1 - ON, hardware genlock frequency is automatically determined
                                     2 - ON, hardware genlock frequency is specified by the user
                                     3 - ON, no hardware sygnal, lock to the user specified frequency */ 

    float  SyncRate;    /* Genlock frequency - number of hardware sync signals per second, 
                           or, if SyncState is 3 - record output frequency */

    WORD   syncNumFrames;
    WORD   SyncPhase;   

} InterSenseTrackerType;


BOOL ISD_openTrackerOld( InterSenseTrackerType *, DWORD, BOOL, BOOL );
BOOL ISD_openTracker( InterSenseTrackerType *, std::string, int, BOOL, BOOL );
BOOL ISD_closeTracker( InterSenseTrackerType * );
BOOL ISD_getCommInfo( InterSenseTrackerType *, ISD_TRACKER_INFO_TYPE * );
BOOL ISD_getTrackerConfig( InterSenseTrackerType *, ISD_TRACKER_INFO_TYPE *, BOOL );
BOOL ISD_setTrackerConfig( InterSenseTrackerType *, ISD_TRACKER_INFO_TYPE *, BOOL );
BOOL ISD_getStationConfig( InterSenseTrackerType *, ISD_STATION_INFO_TYPE *, WORD, BOOL );
BOOL ISD_setStationConfig( InterSenseTrackerType *, ISD_STATION_INFO_TYPE *, WORD, BOOL, BOOL );
void ISD_getTrackerData( InterSenseTrackerType * );
 
BOOL processInterTraxDataRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars);

BOOL ISD_sendCommand( InterSenseTrackerType * , char * );
BOOL ISD_setOutputRecordList( InterSenseTrackerType *, ISD_STATION_INFO_TYPE *, WORD );
BOOL ISD_configureTracker( InterSenseTrackerType *tracker, BOOL verbose );
BOOL ISD_allowUserCommand( char * );
BOOL ISD_applyConfiguration( InterSenseTrackerType *tracker, BOOL verbose );
void ISD_displayTransferRate( float bps, float sps );
void ISD_printf( InterSenseTrackerType *tracker, char *fs,... );

int  wsockIsClient( InterSenseTrackerType *tracker );
int  wsockReceiveData( InterSenseTrackerType *tracker );
BOOL wsockGetStationConfig( InterSenseTrackerType *, ISD_STATION_INFO_TYPE *, WORD, BOOL );
BOOL wsockSetStationConfig( InterSenseTrackerType *, ISD_STATION_INFO_TYPE *, WORD, BOOL );

void itComLogAdd( InterSenseTrackerType *tracker, char *record );
void itComLogClear( InterSenseTrackerType *tracker );
int  itComLogCount( InterSenseTrackerType *tracker );

#endif
