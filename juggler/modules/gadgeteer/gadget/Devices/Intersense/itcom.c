/***********************************************************************
*
*    File Name:    itcom.c
*    Description:  InterSense Tracker Testing Program
*    Created:      1998
*    Author:       Yury Altshuler
*
*    Copyright:    InterSense 1999 - All rights Reserved.
*
*    Comments:
*
*
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>

#include "isense.h"

#if !defined UNIX
#include <windows.h>
#include <winbase.h>
#endif

#include "isdriver.h"
#include "timer.h"
#include "itcom.h"
#include "serial.h"

static void  processSystemRecord(InterSenseTrackerType *tracker, char *cmdbuf,int numChars);
static void  processFastrackData(InterSenseTrackerType *tracker, char *cmdbuf,int numChars);
static void  processSystemDataRecord(InterSenseTrackerType *tracker, char *cmdbuf,int numChars);
static void  processSystemStatusRecord(InterSenseTrackerType *tracker, char *cmdbuf,int numChars);
static void  processSystemStationRecord(InterSenseTrackerType *tracker, char *cmdbuf,int numChars);
static void  processIntrackSensorRecord(InterSenseTrackerType *tracker, char *cmdbuf,int numChars);
static void  processIntrackSystemRecord(InterSenseTrackerType *tracker, char *cmdbuf,int numChars);
static void  processIntrackPredictionRecord(InterSenseTrackerType *tracker, char *cmdbuf,int numChars);
static void  processIntrackRecord(InterSenseTrackerType *tracker, char *cmdbuf,int numChars);
static BOOL  processInterTraxDataRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars);
static BOOL  processSystemOutputListRecord(InterSenseTrackerType *tracker, char *cmdbuf,int numChars);
static void  processIntrackPseRecord(InterSenseTrackerType *tracker, char *buf, int numChars);
static void  processSensitivityRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars);
static void  processSyncRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars);
static void  ISD_PRECISION_serviceSerialPort(InterSenseTrackerType *tracker);
static void  ISD_INTERTRAX_serviceSerialPortPolled(InterSenseTrackerType *tracker);
static void  ISD_INTERTRAX_serviceSerialPortContinuous(InterSenseTrackerType *tracker);

static int   computeRecordSize(InterSenseTrackerType *tracker, int station);
static DWORD charToNum(BYTE c);
static float byteOrder(char *value);


/***************************************************************************/
/* all commands must be sent with this function */

int itSendCommand(InterSenseTrackerType *tracker, char *fs,...)
{
    char sbuf[256];
    va_list argptr;

    va_start(argptr,fs);
    vsprintf(sbuf, fs,argptr);
    va_end(argptr);

    /* clear the applyPSEConfigPending flag if MCe or MCx commands are sent out */
    if(strncmp(sbuf, PSE_APPLY_COMMAND,  strlen(PSE_APPLY_COMMAND))  == 0 || 
       strncmp(sbuf, PSE_CANCEL_COMMAND, strlen(PSE_CANCEL_COMMAND)) == 0)
    {
        tracker->applyPSEConfigPending = FALSE;
    }

    /* set applyPSEConfigPending flag if a constellation type command is being sent out */
    else if(sbuf[0] == 'M' && sbuf[1] == 'C') 
    {
        switch(sbuf[2])
        {
            case 'i':   /* disassociate InertiaCube */
            case 'm':   /* remove mobile PSE */
            case 'f':   /* remove fixed PSE */
            case 'C':   /* clear all fixed PSEs */
            case 'c':   /* clear station */
                tracker->applyPSEConfigPending = TRUE;
                break;
            case 'I':
                if(strlen(sbuf) > 6) tracker->applyPSEConfigPending = TRUE;
                break;
            case 'M':
                if(strlen(sbuf) > 8) tracker->applyPSEConfigPending = TRUE;
                break;
            case 'F':
                if(strlen(sbuf) > 7) tracker->applyPSEConfigPending = TRUE;
                break;
        }
    }
    return (rs232SendString(&(tracker->CommPort), sbuf));
}
 
/****************************************************************************/

/* used to provide for the future expansion of the protocol to support more
   than 16 stations */

char stationToChar(DWORD stationNum)
{
    char station;

    station = '\0';
    if(stationNum < 10)
    {
        station = (BYTE) stationNum + '0';
    }
    else if(stationNum > 9 && stationNum < 30)
    {
        station = (BYTE) stationNum - 10 + 'a';
    }
    else
    {
        station = '0';
    }
    return station;
}

/****************************************************************************/
static DWORD charToNum(BYTE c)
{
    if(isalpha(c))
    {
        if(islower(c))  return((DWORD)(10 + c - 'a'));
        else            return((DWORD)(10 + c - 'A'));
    }
    if(isdigit(c))
    {
        return((DWORD)(c - 0x30));
    }
    return 1;
}


/************************* serviceSerialPort ***********************/
/*
   When InterTrax data is collected to a file it is stored in Fastrak
   format in ASCII to allow it to be imported to Excel. For that reason
   servicePCTSerialPort is only used when reading a port and not when
   playing back data stored in a file. Data collected from IS-300 or
   other Fastrak compatible InterSense trackers is stored in its native
   format and can be played back with the same record parsing routines
*/

void serviceSerialPort(InterSenseTrackerType *tracker)
{
    switch(tracker->hardware)
    {
        case ISD_INTERTRAX_SERIES:
            ISD_INTERTRAX_serviceSerialPort(tracker);
            break;

        case ISD_PRECISION_SERIES:
            ISD_PRECISION_serviceSerialPort(tracker);
            break;

        default:
            ISD_PRECISION_serviceSerialPort(tracker);
            break;
    }
}


/*********************** ISD_INTERTRAX_serviceSerialPort *****************************/
void ISD_INTERTRAX_serviceSerialPort(InterSenseTrackerType *tracker)
{
    if(tracker->state.transmitMode == IT_COM_SYS_POLLED)
    {
        ISD_INTERTRAX_serviceSerialPortPolled( tracker );
    }
    else
    {
        ISD_INTERTRAX_serviceSerialPortContinuous( tracker );
    }
}


/*********************** ISD_INTERTRAX_serviceSerialPortPolled ***********************/
/*
    Unlike Fastrak version this procedure is limited to a very specific
    record format. If record is correctly parced it can be stored to
    file after being converted to Fastrak format for ease of display
    and playback.
*/

static void ISD_INTERTRAX_serviceSerialPortPolled(InterSenseTrackerType *tracker)
{
    while(rs232InChar(&tracker->CommPort, &tracker->cmdbuf[tracker->nchars], FALSE) != EOF)
    {
        tracker->nchars++;
        tracker->numBytesReceived++;

        if(tracker->nchars >= 6)
        {
            if(processInterTraxDataRecord(tracker, tracker->cmdbuf, tracker->nchars) == PASS)
            {
                tracker->numSamplesReceived++;
            }
            tracker->nchars = 0;

            /* First flush any characters in the serial buffer to prepare 
               for the next record. For some reason, records can go out of 
               allignment is this is not done. */

            rs232RxFlush(&tracker->CommPort, 0); 
        }
    }
}

/*********************** ISD_INTERTRAX_serviceSerialPortContinuous ***********************/
/*
    Unlike Fastrak version this procedure is limited to a very specific
    record format. 
*/

static void ISD_INTERTRAX_serviceSerialPortContinuous(InterSenseTrackerType *tracker)
{
    int i, j;
    unsigned char checkSum;
    

    /* read all available data from the serial port */
    while(rs232InChar(&tracker->CommPort, &tracker->cmdbuf[tracker->nchars], FALSE) != EOF)
    {
        tracker->nchars++;
        tracker->numBytesReceived++;
    }
    
    /* get the latest available record */
    if(tracker->nchars >= 8)
    {
        for(i = tracker->nchars-8; i >= 0; i--)
        {
            /* possibly a record header */
            if((unsigned char )(tracker->cmdbuf[i]) == 0x80)
            {
                /* check the last byte for checksum */
                for(checkSum = 0, j = i; j < i+8; j++)
                {
                    checkSum += (unsigned char )(tracker->cmdbuf[j]);
                }

                /* found a record */
                if(checkSum == 0)
                {
                    if(processInterTraxDataRecord(tracker, &(tracker->cmdbuf[i+1]), tracker->nchars) == PASS)
                    {
                        tracker->numSamplesReceived++;
                    }
                    
                    /* preserve the beginning of the next record */
                    memcpy((void *)&(tracker->cmdbuf[0]), (void *)&(tracker->cmdbuf[i+8]), 9);

                    /* i bytes were discarded, 8 bytes processed */
                    tracker->nchars -= i+8;
                    break;
                }
            }
        }
    }

    /* prevent overflow */
    if(tracker->nchars >= INPUT_BUFFER_SIZE-1) tracker->nchars = 0;
}


/*********************** serviceSerialPort ***********************/
static void ISD_PRECISION_serviceSerialPort(InterSenseTrackerType *tracker)
{
    char c;
    
    while(rs232InChar(&(tracker->CommPort), &c, FALSE) == 1)
    {
        tracker->numBytesReceived++;
        if(tracker->nchars == 0)
        {                            /* new record */
            switch(c)
            {
            case '0':              /* system data records */
            case '2':              /* system status records */
            case '3':              /* intrack status records */
                tracker->cmdbuf[tracker->nchars++] = c;
                tracker->recordStartTime = timeNow();
                break;
            default:
                break;
            }
        }
        else if(tracker->nchars == 1)
        {                              /* Second character is station number */
            switch(c)
            {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case 'A':
            case 'B':
            case 'C':
            case 'a':
            case 'b':
            case 'c':
            case ' ':     /* FASTRAK error record */
                tracker->cmdbuf[tracker->nchars++]=c;
                break;
            default:
                tracker->nchars=0;
                break;
            }
        }
        else
        {
            if(c=='\n')
            {
                tracker->cmdbuf[tracker->nchars++] = c;
                
                if(tracker->cprev1 == '\r')
                {
                    processFastrackData(tracker, tracker->cmdbuf, tracker->nchars);
                    tracker->nchars=0;
                }
            }
            else if((tracker->nchars >= 125 && tracker->cmdbuf[0] !='0') || 
                tracker->nchars >= 199)    /* run-on command, flush it */
            {
                tracker->nchars = 0;
            }
            else
            {
                tracker->cmdbuf[tracker->nchars++] = c;
            }
        }
        tracker->cprev2 = tracker->cprev1;
        tracker->cprev1 = c;
    }

    /* Bug fix for INTRACK version 2.0.5b and below. Output List record is not
       terminated by CR and LF. To compensate for than use timer to detect end
       of record and record type to determine if record should be processed. */

    if(tracker->nchars >= 3 && tracker->cmdbuf[2] == 'O')
    {
        if((timeNow() - tracker->recordStartTime) > .5)
        {
            processFastrackData(tracker, tracker->cmdbuf, tracker->nchars);
            tracker->nchars = 0;
            tracker->recordStartTime = 0.0f;
        }
    }
}


/******************* processInterTraxDataRecord *******************/
static BOOL processInterTraxDataRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{    
    struct INT_ANGLES_3DOF {
        short azIntAngle, elIntAngle, twIntAngle;
    };
    static union {
        BYTE bytes[6];
        struct INT_ANGLES_3DOF angles;
    } angleBuffer;

    union {
        char    c[sizeof(short)];
        short   value;
    } endian;

    endian.value = 256;

    /* Big endian. */
    if ( endian.c[0] ) {
        char buf[6];

        buf[0] = cmdbuf[1];
        buf[1] = cmdbuf[0];
        buf[2] = cmdbuf[3];
        buf[3] = cmdbuf[2];
        buf[4] = cmdbuf[5];
        buf[5] = cmdbuf[4];
        memcpy((void *)angleBuffer.bytes, (void *)buf, 6);
    }
    /* Little endian. */
    else {
        memcpy((void *)angleBuffer.bytes, (void *)cmdbuf, 6);
    }

    tracker->station[0].Orientation[0] = -((float)angleBuffer.angles.azIntAngle * 180.0f) / 32768L;
    tracker->station[0].Orientation[1] =  ((float)angleBuffer.angles.elIntAngle * 180.0f) / 32768L;
    tracker->station[0].Orientation[2] = -((float)angleBuffer.angles.twIntAngle * 180.0f) / 32768L;

    tracker->station[0].Position[0] = 0.0f;
    tracker->station[0].Position[1] = 0.0f;
    tracker->station[0].Position[2] = 0.0f;

    tracker->RecvUpdateFlag++;

    return PASS;
}



/********************* computeRecordSize ***********************/
/* Compute expected record size 
 */
static int computeRecordSize(InterSenseTrackerType *tracker, int station)
{
    int i, done=FALSE;
    int pos=0;

    for(i = 0; i < MAX_OUTPUT_LIST_ITEMS && !done; i++)
    {
        switch(tracker->station[station].outputList[i])
        {
            case -1:
                done=TRUE;
                break;

            case 0:
                ++pos;
                break;

            case 1:
                pos+=2;
                done=TRUE;
                break;

            case 2:
            case 4:
            case 5:
            case 6:
            case 7:
                if(tracker->state.outputFormat == IT_COM_SYS_BINARY)
                    pos+=12;
                else
                    pos+=21;
                break;
 
            case 11:
                if(tracker->state.outputFormat == IT_COM_SYS_BINARY)
                    pos+=16;
                else
                    pos+=28;
                break;

            case 16:
                pos+=2;
                break;

            case 18:
            case 19:
                pos+=6;
                break;

            case 20:
                pos+=8;
                break;

            case 21:        /* time stamp */
                if(tracker->state.outputFormat == IT_COM_SYS_BINARY)
                    pos+=4;
                else
                    pos+=14;
                break;

            case 22:        /* joystick buttons */
                if(tracker->state.outputFormat == IT_COM_SYS_BINARY)
                    pos+=1;
                else
                    pos+=5;
                break;

            case 23:        /* analog joystick */
                if(tracker->state.outputFormat == IT_COM_SYS_BINARY)
                    pos+=2;
                else
                    pos+=8;
                break;

            default:
                break;
        }
    }
    /* record header */
    pos += 3;

    return pos;
}


/**************************** byteOrder ******************************/
static float byteOrder(char *value)
{
    float num;

    union {
        char    c[sizeof(short)];
        short   value;
    } endian;

    endian.value = 256;

    /* Big endian. */
    if ( endian.c[0] ) {
        char buf[4];

        buf[0] = value[3];
        buf[1] = value[2];
        buf[2] = value[1];
        buf[3] = value[0]; 
    
        memcpy((void *)&num, (void *)buf, 4);
    }
    /* Little endian. */
    else {
        memcpy((void *)&num, (void *)value, 4);
    }

    return num;
}


/******************* processSystemDataRecord *******************/
static void processSystemDataRecord(InterSenseTrackerType *tracker, 
                                    char *cmdbuf, int numChars)
{
    float  values[6], linMult;
    int    numConverted, i, done, pos;
    char  *buffer;
    int    station;
    int    int14bit;
    BYTE   lo, hi;
    unsigned char joystickByte1, joystickByte2;
    int  joystickInt1, joystickInt2;

    station = cmdbuf[1] - 0x30-1;
    buffer  = &cmdbuf[3];

    done = FALSE;
    pos = 3;

    if(station >= 0 && station < MAX_NUM_STATIONS)
    {
        /* only process a record if size is correct */
        if(computeRecordSize(tracker, station) == numChars)
        {
            for(i = 0; i < MAX_OUTPUT_LIST_ITEMS && pos < numChars-2 && !done; i++)
            {
                switch(tracker->station[station].outputList[i])
                {
                    case 0:
                        buffer = &cmdbuf[++pos];
                        break;

                    case 1:
                        done = TRUE;
                        break;

                    case 2: /* position in meters */
                           
                        linMult = (tracker->state.units==IT_COM_SYS_INCHES ? 0.0254f : 0.01f);

                        if(tracker->state.outputFormat == IT_COM_SYS_BINARY)
                        {
                            if(numChars-pos >= 12)
                            {
                                tracker->station[station].Position[0] = linMult * byteOrder(&buffer[0]);
                                tracker->station[station].Position[1] = linMult * byteOrder(&buffer[4]);
                                tracker->station[station].Position[2] = linMult * byteOrder(&buffer[8]);
                            }
                            pos = min(numChars-1, pos+12);
                            buffer = &cmdbuf[pos];
                        }
                        else
                        {
                            numConverted = sscanf((const char *) buffer, "%f %f %f",
                                                  &values[0], &values[1], &values[2]);

                            if(numConverted == 3)
                            {
                                tracker->station[station].Position[0] = linMult * values[0];
                                tracker->station[station].Position[1] = linMult * values[1];
                                tracker->station[station].Position[2] = linMult * values[2];
                            }
                            pos = min(numChars-1, pos+21);
                            buffer = &cmdbuf[pos];
                        }
                        break;

                    case 4: /* euler orientation angles in degrees */
                        if(tracker->state.outputFormat == IT_COM_SYS_BINARY)
                        {
                            if(numChars-pos >= 12)
                            {
                                tracker->station[station].Orientation[YAW]   = byteOrder(&buffer[0]);
                                tracker->station[station].Orientation[PITCH] = byteOrder(&buffer[4]);
                                tracker->station[station].Orientation[ROLL]  = byteOrder(&buffer[8]);
                            }
                            pos = min(numChars-1, pos+12);
                            buffer = &cmdbuf[pos];
                        }
                        else
                        {
                            numConverted = sscanf((const char *) buffer, "%f %f %f",
                                                  &values[0], &values[1], &values[2]);

                            if(numConverted == 3)
                            {
                                tracker->station[station].Orientation[YAW]   = (float)values[0];
                                tracker->station[station].Orientation[PITCH] = (float)values[1];
                                tracker->station[station].Orientation[ROLL]  = (float)values[2];
                            }
                            pos = min(numChars-1, pos+21);
                            buffer = &cmdbuf[pos];
                        }
                        break;

                    case 5:
                        break;
                    case 6:
                        break;
                    case 7:
                        break;

                    case 11:  /* orientation quaternion format */
                        if(tracker->state.outputFormat == IT_COM_SYS_BINARY)
                        {
                            if(numChars-pos >= 16)
                            {
                                tracker->station[station].Orientation[0] = byteOrder(&buffer[0]);
                                tracker->station[station].Orientation[1] = byteOrder(&buffer[4]);
                                tracker->station[station].Orientation[2] = byteOrder(&buffer[8]);
                                tracker->station[station].Orientation[3] = byteOrder(&buffer[12]);
                            }
                            pos = min(numChars-1, pos+16);
                            buffer = &cmdbuf[pos];
                        }
                        else
                        {
                            numConverted = sscanf((const char *) buffer, "%f %f %f %f", &values[0],
                                                  &values[1], &values[2], &values[3]);

                            if(numConverted == 4)
                            {
                                tracker->station[station].Orientation[0] = (float) values[0];
                                tracker->station[station].Orientation[1] = (float) values[1];
                                tracker->station[station].Orientation[2] = (float) values[2];
                                tracker->station[station].Orientation[3] = (float) values[3];
                            }
                            pos = min(numChars-1, pos+28);
                            buffer = &cmdbuf[pos];
                        }
                        break;

                    case 18:    /* position in binary16 format (meters) */
                        if(numChars-pos >= 6)
                        {
                            lo = (buffer[0] & 0x007F);
                            hi = (buffer[1] & 0x007F);
                            int14bit = (lo<<2) | (hi<<9);
                            tracker->station[station].Position[X_INDEX] = (float)int14bit * 3.0f / 32768.0f;

                            lo = (buffer[2] & 0x007F);
                            hi = (buffer[3] & 0x007F);
                            int14bit = (lo<<2) | (hi<<9);
                            tracker->station[station].Position[Y_INDEX] = (float)int14bit * 3.0f / 32768.0f;

                            lo = (buffer[4] & 0x007F);
                            hi = (buffer[5] & 0x007F);
                            int14bit = (lo<<2) | (hi<<9);
                            tracker->station[station].Position[Z_INDEX] = (float)int14bit * 3.0f / 32768.0f;
                        }
                        pos = min(numChars-1, pos+6);
                        buffer = &cmdbuf[pos];
                        break;

                    case 19:    /* orientation in binary16 format */
                        if(numChars-pos >= 6)
                        {
                            lo = (buffer[0] & 0x007F);
                            hi = (buffer[1] & 0x007F);
                            int14bit = (lo<<2) | (hi<<9);
                            tracker->station[station].Orientation[YAW] = (float)int14bit * 180.0f / 32768.0f;

                            lo = (buffer[2] & 0x007F);
                            hi = (buffer[3] & 0x007F);
                            int14bit = (lo<<2) | (hi<<9);
                            tracker->station[station].Orientation[PITCH] = (float)int14bit * 180.0f / 32768.0f;

                            lo = (buffer[4] & 0x007F);
                            hi = (buffer[5] & 0x007F);
                            int14bit = (lo<<2) | (hi<<9);
                            tracker->station[station].Orientation[ROLL] = (float)int14bit * 180.0f / 32768.0f;
                        }
                        pos = min(numChars-1, pos+6);
                        buffer = &cmdbuf[pos];
                        break;

                    case 21:    /* time stamp */
                        if(tracker->state.outputFormat == IT_COM_SYS_BINARY)
                        {
                            if(numChars-pos >= 4)
                            {
                                tracker->station[station].TimeStaMp = byteOrder(buffer);

                                if(tracker->state.timeUnits == MICROSECONDS)
                                {
                                    tracker->station[station].TimeStaMp /= 1000000.0f;
                                }
                                else
                                {
                                    tracker->station[station].TimeStaMp /= 1000.0f;
                                }
                            }
                            pos = min(numChars-1, pos+4);
                            buffer = &cmdbuf[pos];
                        }
                        else
                        {
                            if(sscanf((const char *) buffer, "%f", &values[0]) == 1)
                            {
                                tracker->station[station].TimeStaMp = values[0];

                                if(tracker->state.timeUnits == MICROSECONDS)
                                {
                                    tracker->station[station].TimeStaMp /= 1000000.0f;
                                }
                                else
                                {
                                    tracker->station[station].TimeStaMp /= 1000.0f;
                                }
                            }
                            pos = min(numChars-1, pos+14);
                            buffer = &cmdbuf[pos];
                        }
                        break;

                    case 22:
                        if(tracker->state.outputFormat == IT_COM_SYS_BINARY)
                        {
                            if(numChars-pos >= 1)
                            {
                                memcpy((void *)&joystickByte1, buffer, 1);
                            }
                            pos = min(numChars-1, pos+1);
                            buffer = &cmdbuf[pos];
                        }
                        else
                        {
                            if(sscanf(buffer, "%d", &joystickInt1) != 1)
                            {
                                joystickInt1 = 0;
                            }
                            joystickByte1 = (unsigned char) joystickInt1;

                            pos = min(numChars-1, pos+5);
                            buffer = &cmdbuf[pos];
                        }

                        tracker->station[station].ButtonState[0] = (BOOL)((joystickByte1 & 0x01) ? 1 : 0);
                        tracker->station[station].ButtonState[1] = (BOOL)((joystickByte1 & 0x02) ? 1 : 0);
                        tracker->station[station].ButtonState[2] = (BOOL)((joystickByte1 & 0x04) ? 1 : 0);
                        tracker->station[station].ButtonState[3] = (BOOL)((joystickByte1 & 0x08) ? 1 : 0);
                        tracker->station[station].ButtonState[4] = (BOOL)((joystickByte1 & 0x10) ? 1 : 0);
                        tracker->station[station].ButtonState[5] = (BOOL)((joystickByte1 & 0x20) ? 1 : 0);
                        tracker->station[station].ButtonState[6] = (BOOL)((joystickByte1 & 0x40) ? 1 : 0);
                        tracker->station[station].ButtonState[7] = (BOOL)((joystickByte1 & 0x80) ? 1 : 0);
                        break;

                    case 23:
                        if(tracker->state.outputFormat == IT_COM_SYS_BINARY)
                        {
                            if(numChars-pos >= 2)
                            {
                                memcpy((void *)&joystickByte1, buffer, 1);
                                memcpy((void *)&joystickByte2, buffer+1, 1);
                            }
                            pos = min(numChars-1, pos+2);
                            buffer = &cmdbuf[pos];
                        }
                        else
                        {
                            if(sscanf(buffer, "%d %d", &joystickInt1, &joystickInt2) != 2)
                            {
                                joystickInt1 = 0;
                                joystickInt2 = 0;
                            }
                            joystickByte1 = (unsigned char) joystickInt1;
                            joystickByte2 = (unsigned char) joystickInt2;

                            pos = min(numChars-1, pos+8);
                            buffer = &cmdbuf[pos];
                        }
                        tracker->station[station].AnalogData[0] = joystickByte1;
                        tracker->station[station].AnalogData[1] = joystickByte2;
                        break;

                    default:
                        break;                
                }
            }
        }
    }
    tracker->RecvUpdateFlag++;
}


/******************* processFastrackData ***********************/
static void processFastrackData(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{
    switch(cmdbuf[0])
    {
        case '0':
            tracker->numSamplesReceived++;
            processSystemDataRecord(tracker, cmdbuf, numChars);
            break;
        case '2':
            itComLogAdd(tracker, cmdbuf);
            processSystemRecord(tracker, cmdbuf, numChars);
            break;
        case '3':
            itComLogAdd(tracker, cmdbuf);
            processIntrackRecord(tracker, cmdbuf, numChars);
            break;
        default:
            break;
    }
}


/******************* processSystemRecord ***********************/
static void processSystemRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{
    switch(cmdbuf[2])
    {
        case 'l':
            processSystemStationRecord(tracker, cmdbuf, numChars);
            break;
        case 'S':
            processSystemStatusRecord(tracker, cmdbuf, numChars);
            break;
        case 'O':
            processSystemOutputListRecord(tracker, cmdbuf, numChars);
            break;
        default:
            break;
     }
}


/******************** processIntrackRecord *****************/
static void processIntrackRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{
    switch(cmdbuf[2])
    {
        case 'S':
            processIntrackSystemRecord(tracker, cmdbuf, numChars);
            break;
        case 's':
            processIntrackSensorRecord(tracker, cmdbuf, numChars);
            break;
        case 'p':
            processIntrackPredictionRecord(tracker, cmdbuf, numChars);
            break;
        case 'I':
            processIntrackPseRecord(tracker, cmdbuf, numChars);
            break;
        case 'Q':
            processSensitivityRecord(tracker, cmdbuf, numChars);
            break;
        case 'G':
            processSyncRecord(tracker, cmdbuf, numChars);
            break;
        default:
            break;
    }
}


/********************** processIntrackPseRecord ****************************/
static void processIntrackPseRecord(InterSenseTrackerType *tracker, char *buf, int numChars)
{
    int   num, numConverted;
    DWORD stationNum;

    switch (buf[2])
    {
        case 'I':

            stationNum = charToNum(buf[1])-1;
            numConverted = sscanf((const char *) &buf[3], "%d", &num);

            if(numConverted == 1)
            {
                if(stationNum < MAX_NUM_STATIONS &&
                   num >= -1  && num <= MAX_NUM_IMUS)
                {
                    tracker->station[stationNum].InertiaCube = num;
                    tracker->ItComImuFlag[stationNum] = TRUE;
                }
            }
            break;

        default:
            break;
    }
}



/******************** processSystemOutputListRecord *****************/

/* Process Output Item List record. Record type '2', Sub-record type 'O'
   Data items are identified by two character numeric codes in ASCII
   format. itComOutputListCodes contains all valid parameters. If during
   record conversion an invalid code is detected procedure fails. */

static BOOL processSystemOutputListRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{
    int i, j, status, value, counter, numItems;
    DWORD station;
    BYTE conv[4];

    /* Ouput record list codes */

    int itComOuputListCodes[IT_COM_NUM_LIST_CODES] =
                         {0, 1, 2, 4, 5, 6, 7, 11, 18, 19, 20, 21, 22, 23};


    station = charToNum(cmdbuf[1])-1;

    if(station >= MAX_NUM_STATIONS) return FAIL;

    if(tracker->state.firmwareVersion < 3.0f)
    {
        /* Bug fix for INTRACK vervion 2.0.5b and below. Output List records
           are not terminated by CR and LF. To find the end of the output
           list record look for the beginning of the next one. */

        for(i=3; i < numChars-1; i++)
        {
            if(!( ( isdigit(cmdbuf[i]) && isdigit(cmdbuf[i+1]) ) ||
                  ( cmdbuf[i] == ' '   && isdigit(cmdbuf[i+1]) ) ||
                  ( isdigit(cmdbuf[i]) && cmdbuf[i+1] == ' '   )))
            {
                numChars = i+1;
                break;
            }
        }
    }
    cmdbuf[numChars] = '\0';

    numItems = (numChars-3)/2;

    /* clear the output list array */
    for(i=0; i<MAX_OUTPUT_LIST_ITEMS; i++)
    {
        tracker->station[station].outputList[i] = -1;
    }

    for(i=0, counter=0; i < numItems && counter < MAX_OUTPUT_LIST_ITEMS; i++)
    {
        conv[0] = cmdbuf[3+i*2];
        conv[1] = cmdbuf[4+i*2];
        conv[2] = '\0';
        sscanf(conv, "%d", &value);
        status = FAIL;

        for(j=0; j<IT_COM_NUM_LIST_CODES; j++)
        {
            if(value == itComOuputListCodes[j])
            {
                tracker->station[station].outputList[counter++] = value;
                status = PASS;
                break;
            }
        }
        if(status == FAIL) break;
    }

    tracker->ItComOutputListFlag = TRUE;

    return status;
}


/*********************** processSystemStatusRecord ***********************/
static void processSystemStatusRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{
    ItComSystemStatusRecordType  status;
    int statLow;
    int statMed;
    int statHigh;
    char   statMess[4];

    /* copy the input buffer to a local struct */

    memcpy((char *)&status, cmdbuf, sizeof(ItComSystemStatusRecordType));

    /* now go through and conver the compact struct to one easier to */
    /* deal with */

    statMess[1] = 0x00;

    statMess[0] = cmdbuf[3];
    sscanf(statMess,"%x",&statHigh);
    statMess[0] = cmdbuf[4];
    sscanf(statMess,"%x",&statMed);
    statMess[0] = cmdbuf[5];
    sscanf(statMess,"%x",&statLow);

    if((statLow&IT_COM_SYS_OUT_BIT) == IT_COM_SYS_OUT_BIT)
    {
        tracker->state.outputFormat = IT_COM_SYS_BINARY;
    }
    else
    {
        tracker->state.outputFormat = IT_COM_SYS_ASCII;
    }

    if((statLow&IT_COM_SYS_UNITS_BIT) == IT_COM_SYS_UNITS_BIT)
    {
        tracker->state.units = IT_COM_SYS_CENT;
    }
    else
    {
        tracker->state.units = IT_COM_SYS_INCHES;
    }

    if((statLow&IT_COM_SYS_TRANS_BIT) == IT_COM_SYS_TRANS_BIT)
    {
        tracker->state.transmitMode = IT_COM_SYS_CONTINUOUS;
    }
    else
    {
        tracker->state.transmitMode = IT_COM_SYS_POLLED;
    }

    strncpy((char *)tracker->state.bitError, (char *)status.bitError, IT_COM_NUM_BIT_ERROR_BYTES);
    strncpy((char *)tracker->state.ver, (char *)status.ver, IT_COM_NUM_VER_BYTES);
    strncpy((char *)tracker->state.id, (char *)status.id, IT_COM_NUM_ID_BYTES);

    tracker->state.ver[IT_COM_NUM_VER_BYTES] = '\0';
    tracker->state.id[IT_COM_NUM_ID_BYTES] = '\0';

    sscanf(tracker->state.ver, "%f", &tracker->state.firmwareVersion);

    tracker->ItComStatusUpdateFlag = TRUE;
}


/********************* processIntrackSensorRecord *******************/
static void processIntrackSensorRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{
    int  statLow;
    int  statMed;
    int  statHigh;
    BYTE  statMess[4];
    DWORD station;

    station = charToNum(cmdbuf[1])-1;

    if(station >= MAX_NUM_STATIONS) return;

    statMess[1] = 0x00;

    statMess[0] = cmdbuf[3];
    sscanf((const char *) statMess, "%hx", &statHigh);
    statMess[0] = cmdbuf[4];
    sscanf((const char *) statMess, "%hx", &statMed);
    statMess[0] = cmdbuf[5];
    sscanf((const char *) statMess, "%hx", &statLow);

    if((statMed&IT_COM_SENSOR_JUMP_MODE_BIT) == IT_COM_SENSOR_JUMP_MODE_BIT)
    {
        tracker->station[station].jumpMode = OFF;
        tracker->station[station].enhancement = 2;
    }
    else
    {
        tracker->station[station].jumpMode=ON;
        tracker->station[station].enhancement = 0;
    }

    if(tracker->state.firmwareVersion < 3.0157f)
    {    
        if((statLow&IT_COM_SENSOR_YAW_COMP_MODE_BIT) == IT_COM_SENSOR_YAW_COMP_MODE_BIT)
        {
            tracker->station[station].compass = ON;
        }
        else
        {
            tracker->station[station].compass = OFF;
        }
    }
    else
    {
        tracker->station[station].compass = 
            (((statLow & 0x02) == 0x02) ? 2 : 0) + 
            (((statLow & 0x04) == 0x04) ? 1 : 0);
    }

    if(tracker->state.firmwareVersion >= 3.0f)
    {
        tracker->station[station].enhancement = min(statMed, MAX_PPF_LEVEL);
    }

    tracker->ItComSensorUpdateFlag = TRUE;
}


/********************* processIntrackSystemRecord *******************/
static void processIntrackSystemRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{
    int statLow;
    int statMed;
    int statHigh;
    BYTE statMess[4];

    statMess[1] = 0x00;

    statMess[0] = cmdbuf[3];
    sscanf((const char *) statMess,"%x",&statHigh);
    statMess[0] = cmdbuf[4];
    sscanf((const char *) statMess,"%x",&statMed);
    statMess[0] = cmdbuf[5];
    sscanf((const char *) statMess,"%x",&statLow);


    if((statLow&IT_COM_SYS_TIME_BIT)==IT_COM_SYS_TIME_BIT)
    {
        tracker->state.timeUnits = MICROSECONDS;
    }
    else
    {
        tracker->state.timeUnits = MILLISECONDS;
    }

    tracker->state.boresightMode = (statLow & IT_COM_SYS_BORESIGHT_BIT) ? 1 : 0;
    tracker->state.ledEnable = (statLow & IT_COM_LED_ENABLE_BIT) ? 1 : 0;
    tracker->state.win32Platform = statMed & IT_COM_PLATFORM_BIT;
    tracker->state.productID = (BYTE) charToNum(cmdbuf[3]);

    tracker->ItComSystemUpdateFlag = TRUE;
}



/******************* processIntrackPredictionRecord *********************/
static void processIntrackPredictionRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{
    DWORD station;

    station = charToNum(cmdbuf[1])-1;

    if(station >= MAX_NUM_STATIONS) return;

    tracker->station[station].prediction = (WORD)atoi((const char *) &cmdbuf[3]);

    if(tracker->station[station].prediction > 200)
    {
        tracker->station[station].prediction = 0;
    }

    tracker->ItComPredictionFlag=TRUE;
}


/********************** processSystemStationRecord *********************/
static void processSystemStationRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{
    int i;
    int stationNum;

    stationNum = charToNum(cmdbuf[1])-1;

    if(stationNum < MAX_NUM_STATIONS)
    {
        for(i=0; i<MAX_NUM_STATIONS && i+3<numChars && cmdbuf[i+3] != '\r'; i++)
        {
            tracker->station[i].state = (WORD)(cmdbuf[i+3]-0x30);
        }
        /* first element corresponds to specified station */
       /* tracker->station[stationNum].state = (WORD)(cmdbuf[3]-0x30); */
        tracker->ItComStationUpdateFlag = TRUE;
    }

    /* recompute number of active stations */
    tracker->state.numActive = 0;

    for(i=0; i<MAX_NUM_STATIONS; i++)
    {
        if(tracker->station[i].state)
        {
            tracker->state.numActive++;
        }
    }

    tracker->ItComStationUpdateFlag = TRUE;
}


/*************************** processSensitivityRecord ******************************/
static void processSensitivityRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{
    int numConverted, num;
    int stationNum;

    stationNum = charToNum(cmdbuf[1])-1;

    switch(cmdbuf[2])
    {
        case 'Q':     /* Filter Size */
            numConverted = sscanf(&cmdbuf[3], "%lu", &num);

            if(numConverted == 1)
            {
                tracker->station[stationNum].sensitivity = num;
                tracker->ItComSensitivityValFlag = TRUE;
            }
            break;

        default:
            break;
    }
}



/********************* processSyncRecord *********************/
static void processSyncRecord(InterSenseTrackerType *tracker, char *cmdbuf, int numChars)
{
    int state, numFrames, numConverted;
    float rate;

    tracker->ItComSyncRecordFlag = TRUE;

    numConverted = sscanf(&cmdbuf[2], "%d %f %d", &state, &rate, &numFrames);

    if(numConverted >= 2)
    {
        tracker->SyncState = (WORD) state;
        tracker->SyncRate = rate;

        tracker->ItComSyncRecordFlag = TRUE;
    }
    /* internal, computer automaticaly by the tracker */
    if(numConverted == 3)
    {
        tracker->syncNumFrames = numFrames;
    }
}


/************************************************************************/
/*                                                                      */ 
/*              itComUpdate functions                                   */
/*                                                                      */ 
/************************************************************************/


/********************* itComUpdateOutputList ********************/
BOOL itComUpdateOutputList(InterSenseTrackerType *tracker, DWORD stationNum)
{
    float startTime;
    int attempt;

    tracker->ItComOutputListFlag = FALSE;

    for(attempt = 0; attempt < 2; attempt++)
    {
        startTime = timeNow();

        itSendCommand(tracker, "O%c\r\n", stationToChar(stationNum));

        while(1)
        {
            serviceSerialPort(tracker);

            if(tracker->ItComOutputListFlag == TRUE)
            {
                return PASS;
            }
            if((timeNow()-startTime) > IT_COM_STATUS_REC_TIMEOUT)
            {
                break;
            }
        }
    }
    return FAIL;
}


/********************** itComUpdateStatusRecord *******************/
BOOL itComUpdateStatusRecord(InterSenseTrackerType *tracker, float timeOut)
{
    float startTime;
    int attempt;

    tracker->ItComStatusUpdateFlag = FALSE;

    for(attempt = 0; attempt < 2; attempt++)
    {
        startTime = timeNow();

        itSendCommand(tracker, "S");

        while(1)
        {
            serviceSerialPort(tracker);

            if(tracker->ItComStatusUpdateFlag == TRUE)
            {
                return PASS;
            }
            if((timeNow()-startTime) > timeOut)
            {
                break;
            }
        }
    }
    return FAIL;
}


/********************** itComUpdateStationStat **************************/
BOOL itComUpdateStationStat(InterSenseTrackerType *tracker, DWORD stationNum)
{
    float startTime;
    int attempt;

    tracker->ItComStationUpdateFlag = FALSE;

    for(attempt = 0; attempt < 2; attempt++)
    {
        startTime = timeNow();

        if(tracker->state.firmwareVersion < 3.0f)
            itSendCommand(tracker, "l%c\r\n",stationToChar(stationNum));
        else
            itSendCommand(tracker, "l*\r\n");

        while(1)
        {
            serviceSerialPort(tracker);

            if(tracker->ItComStationUpdateFlag == TRUE)
            {
                return PASS;
            }
            if((timeNow()-startTime) > IT_COM_STATUS_REC_TIMEOUT)
            {
                break;
            }
        }
    }
    return FAIL;
}


/********************** itComUpdateIntrackSystemStat **************************/
BOOL itComUpdateIntrackSystemStat(InterSenseTrackerType *tracker, float timeOut)
{
    float startTime;
    int attempt;

    tracker->ItComSystemUpdateFlag = FALSE;

    for(attempt = 0; attempt < 2; attempt++)
    {
        startTime = timeNow();

        itSendCommand(tracker, "MS\r\n");

        while(1)
        {
            serviceSerialPort(tracker);

            if(tracker->ItComSystemUpdateFlag == TRUE)
            {
                /* default to IS-600 */
                tracker->state.hardwareVersion = IS600;

                if(tracker->state.productID & IT_IS300)
                    tracker->state.hardwareVersion = IS300;

                if(tracker->state.productID & IT_IS900)
                    tracker->state.hardwareVersion = IS900;

                return PASS;
            }

            if((timeNow()-startTime) > timeOut)
            {
                break;
            }
        }
    }
    tracker->state.hardwareVersion = IS300;
    return FAIL;
}


/********************* itComUpdateSensorStat ************************/
BOOL itComUpdateSensorStat(InterSenseTrackerType *tracker, DWORD stationNum)
{
    float startTime;
    int attempt;

    tracker->ItComSensorUpdateFlag = FALSE;

    for(attempt = 0; attempt < 2; attempt++)
    {
        startTime = timeNow();

        itSendCommand(tracker, "Ms%c\r\n", stationToChar(stationNum));

        while(1)
        {
            serviceSerialPort(tracker);

            if(tracker->ItComSensorUpdateFlag == TRUE)
            {
                return PASS;
            }
            if((timeNow()-startTime) > IT_COM_STATUS_REC_TIMEOUT)
            {
                break;
            }
        }
    }
    return FAIL;
}


/********************* itComUpdatePredictionString ******************/
BOOL itComUpdatePredictionString(InterSenseTrackerType *tracker, DWORD stationNum)
{
    float startTime;
    int attempt;

    tracker->ItComPredictionFlag = FALSE;

    for(attempt = 0; attempt < 2; attempt++)
    {
        startTime = timeNow();

        itSendCommand(tracker, "Mp%c\r\n", stationToChar(stationNum));

        while(1)
        {
            serviceSerialPort(tracker);

            if(tracker->ItComPredictionFlag == TRUE)
            {
                return PASS;
            }
            if((timeNow()-startTime) > IT_COM_STATUS_REC_TIMEOUT)
            {
                break;
            }
        }
    }
    return FAIL;
}


/******************* itComUpdateImu *************************/
BOOL itComUpdateImu(InterSenseTrackerType *tracker, DWORD stationNum)
{
    float startTime;
    int attempt;

    tracker->ItComImuFlag[stationNum-1] = FALSE;

    for(attempt = 0; attempt < 2; attempt++)
    {
        startTime = timeNow();

        itSendCommand(tracker, "MCI%c\r\n", stationToChar(stationNum));

        while(1)
        {
            serviceSerialPort(tracker);

            if(tracker->ItComImuFlag[stationNum-1])
            {
                 return PASS;
            }
            if((timeNow()-startTime) > IT_COM_STATUS_REC_TIMEOUT)
            {
                break;
            }
        }
    }
    return FAIL;
}


/********************* itComUpdateSensitivityVal **********************/
BOOL itComUpdateSensitivityVal(InterSenseTrackerType *tracker, DWORD stationNum)
{
    float startTime;
    int attempt;

    tracker->ItComSensitivityValFlag = FALSE;

    for(attempt = 0; attempt < 2; attempt++)
    {
        startTime = timeNow();

        itSendCommand(tracker, "MQ%c\r\n", stationToChar(stationNum));

        while(1)
        {
            serviceSerialPort(tracker);

            if(tracker->ItComSensitivityValFlag)
            {
                 return PASS;
            }
            if((timeNow()-startTime) > IT_COM_STATUS_REC_TIMEOUT)
            {
                break;
            }
        }
    }
    return FAIL;
}


/************************ itComUpdateSyncState ********************/
BOOL itComUpdateSyncState(InterSenseTrackerType *tracker)
{
    float startTime;
    int attempt;

    tracker->ItComSyncRecordFlag = FALSE;

    for(attempt = 0; attempt < 2; attempt++)
    {
        startTime = timeNow();

        itSendCommand(tracker, "MG\r\n");

        while(1)
        {
            serviceSerialPort(tracker);

            if(tracker->ItComSyncRecordFlag)
            {
                 return PASS;
            }
            if((timeNow()-startTime) > IT_COM_STATUS_REC_TIMEOUT)
            {
                break;
            }
        }
    }
    return FAIL;
}


/******************************************************************************/
void itComLogAdd( InterSenseTrackerType *tracker, char *record )
{
    int head;
    char *buf;
    
    buf = record;
    while(*buf != '\0' && *buf != '\n' && *buf != '\r') buf++;
    *buf = '\0';
    
    strcpy(&tracker->log.records[tracker->log.head][0], record);
    
    head = tracker->log.head + 1;
    if( head >= MAX_LOG ) head = 0;
    if( head == tracker->log.tail ) tracker->log.tail++;
    if( tracker->log.tail >= MAX_LOG ) tracker->log.tail = 0;
    tracker->log.head = head;
}


/*****************************************************************************/
void itComLogClear( InterSenseTrackerType *tracker )
{
    tracker->log.head = tracker->log.tail = 0;
}





