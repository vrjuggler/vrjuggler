/***********************************************************************
*
*    File Name:    serial.h
*    Description:  Header for serial.c
*    Created:      9/2/1997
*    Author:       Yury Altshuler
*
*    Copyright:    InterSense 1999 - All rights Reserved.
*
*    Comments:
*
*
************************************************************************/
#ifndef _ISD_serialh
#define _ISD_serialh

#include "isense.h"

#define RX_BUFFER_SIZE 1024

#if defined UNIX

typedef struct comm_port
{
    int         desc;
    WORD        portNumber;
    WORD        rx_bufsize;      /* size of the receive buffer */
    char        rxbuf[RX_BUFFER_SIZE];    /* receive buffer */
    long        dwRead;
    long        dwReturned;
} COMM_PORT;

#else

#include <windows.h>
#include <winbase.h>

typedef struct comm_port
{
    int desc;
    HANDLE      portHandle;
    WORD        portNumber;
    WORD        rx_bufsize;      /* size of the receive buffer */
    char        rxbuf[RX_BUFFER_SIZE];    /* receive buffer */
    OVERLAPPED  osWrite;        /* overlapped structure for writing */
    OVERLAPPED  osReader;       /* overlapped structure for reading */
    long        dwRead;
    long        dwReturned;
    BOOL        fWaitingOnRead;
} COMM_PORT;

#endif

int   rs232InitCommunications( COMM_PORT *port, const char* comPort, DWORD baudRate );
int   rs232DeinitCommunications( COMM_PORT *port );
int   rs232SetSpeed( COMM_PORT *port, DWORD baudRate );
int   rs232RxFlush( COMM_PORT *port, WORD numBytes );
int   rs232InChar( COMM_PORT *port, char *c, int flush );
WORD  rs232RxCount( COMM_PORT *port );
int   rs232SendString( COMM_PORT *port, char *commandString );
int   rs232PutChar( COMM_PORT *port, char c );
BOOL  rs232SetRTSState( COMM_PORT *port, DWORD value );
BOOL  waitForChar( COMM_PORT *port, char *ch );
BOOL  waitForShort( COMM_PORT *port, short *num );

#endif
