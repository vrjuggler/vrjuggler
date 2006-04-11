/***********************************************************************
*
*       File Name:      wsock.h
*       Description:    
*       Created:        2001
*       Author:         Yury Altshuler
*
*       Copyright:      InterSense 2001 - All rights Reserved.
*
*       Comments:
*
*
************************************************************************/
#ifndef _ISD_wsockh
#define _ISD_wsockh

#if defined _Windows || defined __WIN32__ || defined WINDOWS || defined WIN32 || defined WIN64
#include <winsock.h>

#else /* UNIX */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#define closesocket(A) close(A)
#define ioctlsocket(A,B,C) ioctl(A,B,C)

#endif

#define UDP_STATION_DATA   1
#define UDP_CAMERA_DATA    2
#define UDP_START_BYTE     0xFF    

#define UDP_MAX_ANALOG_CHANNELS 8 

typedef struct
{
    unsigned char StartByte;
    unsigned char PacketType;
    unsigned char PacketSeqNum;
    unsigned char CheckSum;

    unsigned char Model;       /* tracker model */
    unsigned char StationNum;
    unsigned char Status;      /* tracking status byte */
    unsigned char ButtonState; 

    unsigned char AnalogData[UDP_MAX_ANALOG_CHANNELS]; 

    float         Yaw;        /* in degrees */
    float         Pitch;    
    float         Roll;    
    float         PosX;       /* Always in meters */
    float         PosY; 
    float         PosZ; 
    float         TimeStamp;  /* Seconds, reported only if requested */

} udpStationPacketType;


typedef struct
{
    unsigned char StartByte;
    unsigned char PacketType;
    unsigned char PacketSeqNum;
    unsigned char CheckSum;

    unsigned char StationNum;
    unsigned char bReserved1;
    unsigned char bReserved2;
    unsigned char bReserved3;

    unsigned long Timecode;           /* timecode, not implemented yet */
    long          ApetureEncoder;     /* Apeture encoder counts, relative to last reset of power up */
    long          FocusEncoder;       /* Focus encoder counts */
    long          ZoomEncoder;        /* Zoom encoded counts */
    unsigned long TimeCodeUserBits;   /* Time code user bits, not implemented yet */

    float         Apeture;            /* Computed Apeture value */
    float         Focus;              /* Computed focus value (mm), not implemented yet */
    float         FOV;                /* Computed vertical FOV value (degrees) */
    float         NodalPoint;         /* Nodal point offset due to zoom and focus (mm) */

} udpCameraPacketType;


#define WSOCK_ROLE_NONE        0
#define WSOCK_ROLE_SERVER      1
#define WSOCK_ROLE_CLIENT      2


typedef struct 
{
    SOCKET msgsock;
    int  wsockOpened;

    struct sockaddr_in client;
    struct sockaddr_in server;

    int  dwReturned;
    int  dwRead;
    char buf[2048];

    unsigned char cmdbuf[100];
    unsigned int  nchars;
    unsigned long expectedSize;

} wsockChannelType;

typedef struct 
{
    int socket_type;
    int udpHostInit;
    int role;

    char localHostName[100];
    char remoteHostName[100];
    char remoteHostVer[10];
    int  remoteHostType;

    wsockChannelType data;
    wsockChannelType info;

} wsockType;

void WSAManage( int close );

void wsockInit( wsockType *wsock );
void wsockCloseAll( wsockType * );
void wsockLocalAddress( wsockType * );

int  wsockState( wsockType *wsock );

int  wsockBroadcastReadByte( wsockType *wsock, char *c );
int  wsockBroadcastClientInit( wsockType *wsock, unsigned short networkPort );

#endif
