/***********************************************************************
*
*    File Name:      itcom.h
*    Description:    header file for intersense communication routines
*    Created:        1998
*    Author:         Yury Altshuler
*
*    Copyright:      InterSense 1999 - All rights Reserved.
*
*    Comments:
*
*
************************************************************************/
#ifndef _ISD_itcomh
#define _ISD_itcomh

#include "isense.h"
#include <vpr/vpr.h>

#ifdef VPR_IS_Win32
#include <windows.h>
#include <winbase.h>
#endif

#include "isdriver.h"
#include "serial.h"

#define MAX_COMMAND_STRING 200

/* Manufacturer system record status bits */

/* low byte */
#define IT_COM_SYS_BORESIGHT_BIT       0x02
#define IT_COM_SYS_TIME_BIT            0x04

/* low byte */
#define IT_COM_SYS_OUT_BIT    0x01
#define IT_COM_SYS_UNITS_BIT  0x02
#define IT_COM_SYS_COMP_BIT   0x04
#define IT_COM_SYS_TRANS_BIT  0x08

/* medium byte */
#define IT_COM_SYS_CONF_BIT   0x01

#define IT_COM_TWO_PASS_ENABLE_BIT     0x01
#define IT_COM_SYS_BORESIGHT_BIT       0x02
#define IT_COM_SYS_TIME_BIT            0x04
#define IT_COM_LED_ENABLE_BIT          0x08

/* medium byte */
#define IT_COM_PLATFORM_BIT            0x01

#define IT_COM_SENSOR_YAW_COMP_MODE_BIT     0x02
#define IT_COM_SENSOR_JUMP_MODE_BIT         0x02

#define IT_COM_SYS_BINARY 1
#define IT_COM_SYS_ASCII  0

#define IT_COM_SYS_CENT  1
#define IT_COM_SYS_INCHES 0

#define IT_COM_SYS_UNKNOWN 0
#define IT_COM_SYS_TRACKER 1

#define IT_COM_SYS_CONTINUOUS 1
#define IT_COM_SYS_POLLED     0

#define IT_COM_STATUS_REC_TIMEOUT 2.0f

#define ASCII_OUTPUT    0
#define BINARY_OUTPUT   1
#define BINARY16_OUTPUT 2

#define MILLISECONDS    0
#define MICROSECONDS    1

#define MAX_PPF_LEVEL 2
#define MAX_PREDICTION 50

/* System status record can be directly copied to this structure.
   This simplifies decoding */

typedef struct
{
    char recordType;
    char stationNumber;
    char subRecordType;
    char systemFlag[3];
    char bitError[IT_COM_NUM_BIT_ERROR_BYTES];
    char blank[6];
    char ver[IT_COM_NUM_VER_BYTES+1];
    char id[IT_COM_NUM_ID_BYTES+1];
    char cr;
    char ln;
} ItComSystemStatusRecordType;

void serviceSerialPort( InterSenseTrackerType *tracker );
void ISD_INTERTRAX_serviceSerialPort(InterSenseTrackerType *tracker);
void requestTrackerUpdate( InterSenseTrackerType *tracker );
int  itSendCommand( InterSenseTrackerType *tracker, char *fs,... );
char stationToChar( DWORD stationNum );

BOOL itComUpdateStationStat( InterSenseTrackerType *tracker, DWORD sensorNum );
BOOL itComUpdateSensorStat( InterSenseTrackerType *tracker, DWORD sensorNum );
BOOL itComUpdateIntrackSystemStat( InterSenseTrackerType *tracker, float timeOut );
BOOL itComUpdateOutputList( InterSenseTrackerType *tracker, DWORD sensorNum );
BOOL itComUpdatePredictionString( InterSenseTrackerType *tracker, DWORD sensorNum );
BOOL itComUpdateStatusRecord( InterSenseTrackerType *tracker, float timeOut );
BOOL itComUpdateImu( InterSenseTrackerType *tracker, DWORD sensorNum );
BOOL itComUpdateSensitivityVal( InterSenseTrackerType *tracker, DWORD sensorNum );
BOOL itComUpdateSyncState( InterSenseTrackerType *tracker );

void ISD_INTERTRAX_2_serviceSerialPortPolled(InterSenseTrackerType *tracker);

float         byteOrderFloat(char *value);
long          byteOrderLong (char *value);
unsigned long byteOrderULong(char *value);
short         byteOrderShort(char *value);

#endif






