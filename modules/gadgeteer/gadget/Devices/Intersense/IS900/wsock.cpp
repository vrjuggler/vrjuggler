/***********************************************************************
*
*  File Name:      wsock.c
*  Description:    TCP/UDP client.
*  Created:        8/20/2000
*  Author:         Yury Altshuler
*  Copyright:      InterSense 2001 - All rights Reserved.
*
*  Comments:
*
*
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __sun
#include <sys/filio.h>
#endif

#include "isense.h"
#include "isdriver.h"
#include "wsock.h"
#include "itcom.h"


#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

static void wsockInitConfig( wsockType *wsock, int type );
static const char *wsockErrorStrings( int error );
static int  wsockGetLastError (void);


/*****************************************************************************
*
*   functionName:   wsockGetLastError
*   Description:
*
*   Comments:           
*
******************************************************************************/
static int  wsockGetLastError (void)
{
#if defined _Windows || defined __WIN32__ || defined WINDOWS || defined WIN32
    return WSAGetLastError();
#endif

#if defined VPR_OS_IRIX || defined VPR_OS_Linux || defined VPR_OS_Solaris
    return 0;
#endif
}


/*****************************************************************************
*
*   functionName:   wsockCloseAll
*   Description:
*
*   Comments:           
*
******************************************************************************/
void wsockCloseAll( wsockType *wsock )
{
    if(wsock->data.wsockOpened)
    {
        closesocket(wsock->data.msgsock);
    }
    wsock->role = WSOCK_ROLE_NONE;
    wsock->udpHostInit = FALSE;
    wsock->data.wsockOpened = FALSE;
}

/*****************************************************************************
*
*   functionName:   WSAManage
*   Description:
*
*   Comments:           
*
******************************************************************************/
void WSAManage( int close )
{
#if defined _Windows || defined __WIN32__ || defined WINDOWS || defined WIN32

    WORD wVersionRequested; 
    WSADATA wsaData; 
    int WSALoaded = FALSE;
    int err; 
    
    if( close && WSALoaded )
    {
        WSACleanup();
        WSALoaded = FALSE;
        return;
    }

    wVersionRequested = MAKEWORD(1, 1); /* use version 1.1 */
    
    if( !WSALoaded )
    {
        err = WSAStartup(wVersionRequested, &wsaData); 
        
        if(err != 0) /* Couldn't find a useable winsock.dll */ 
            return; 
        
        /* Confirm that the Windows Sockets DLL supports 1.1.
           Note that if the DLL supports versions greater  
           than 1.1 in addition to 1.1, it will still return  
           1.1 in wVersion since that is the version we requested. */ 
        
        if( LOBYTE( wsaData.wVersion ) != 1 || 
             HIBYTE( wsaData.wVersion ) != 1 )  /* Couldn't find a useable winsock.dll */ 
        {            
            WSACleanup(); 
            return; 
        } 
        /* The Windows Sockets DLL is acceptable. */ 
        WSALoaded = TRUE;
    }
#endif
}


/*****************************************************************************
*
*   functionName:   wsockLocalAddress
*   Description:
*
*   Comments:           
*
******************************************************************************/
void wsockLocalAddress( wsockType *wsock )
{
    int  nAdapter = 0;

    struct in_addr local;
    struct hostent *hp;
    
    if(gethostname( wsock->localHostName, 
        sizeof( wsock->localHostName )) == SOCKET_ERROR) return;

    printf("Local Host: %s", wsock->localHostName);

    if((hp = gethostbyname( wsock->localHostName )) == NULL) return;

    while( hp->h_addr_list[nAdapter] )
    {
        memcpy((void *) &local, 
            hp->h_addr_list[nAdapter], sizeof(unsigned long));

        printf(", %s", (char *) inet_ntoa(local));

        nAdapter++;
    }
    printf("\n");
}


/*****************************************************************************
*
*   functionName:   wsockInit
*   Description:
*
*   Comments:           
*
******************************************************************************/
void wsockInit( wsockType *wsock )
{
    wsock->udpHostInit = FALSE;
    wsock->role = WSOCK_ROLE_NONE;

    WSAManage(FALSE);
    wsockInitConfig( wsock, -1 );
}


/*****************************************************************************
*
*   functionName:   wsockInitConfig
*   Description:
*
*   Comments:           
*
******************************************************************************/
static void wsockInitConfig( wsockType *wsock, int type )
{
    wsock->data.wsockOpened = FALSE;
    wsock->data.dwRead = 0;
    wsock->data.dwReturned = 0;

    wsock->udpHostInit = FALSE;
    wsock->socket_type = type;
    wsock->role = WSOCK_ROLE_NONE;

    wsock->data.nchars = 0;
    wsock->data.expectedSize = 0;
}


/*****************************************************************************
*
*   functionName:   wsockConfigChannel
*   Description:
*
*   Comments:           
*
******************************************************************************/
int wsockConfigChannel( SOCKET wsocket )
{
    unsigned long ioctl_opt = 1;

    if(ioctlsocket(wsocket, FIONBIO, &ioctl_opt) == SOCKET_ERROR) 
    {
        printf("ioctl failed %d\n%s", wsockGetLastError(), 
            wsockErrorStrings( wsockGetLastError() ));
        
        closesocket(wsocket);
        return -1;
    }

    /* setup for non-blockin operation */
#if defined _Windows || defined __WIN32__ || defined WINDOWS || defined WIN32
    {
    BOOL tndelay_opt = TRUE;
    
    if(setsockopt(wsocket, SOL_SOCKET, TCP_NODELAY, 
        (const char *)&tndelay_opt, sizeof(tndelay_opt)) == SOCKET_ERROR) 
    {
            printf("setsockopt failed %d\n%s", wsockGetLastError(), 
            wsockErrorStrings( wsockGetLastError() ));

	    closesocket(wsocket);
            return -1;
    }
    }
#endif

#if defined VPR_OS_IRIX || defined VPR_OS_Linux || defined VPR_OS_Solaris
#if defined VPR_OS_Solaris
    if(fcntl(wsocket, F_SETFL, O_NDELAY) < 0)
    {
        perror( "Socket Setup");
        return FALSE;
    }
#else
    if(fcntl(wsocket, F_SETFL, FNDELAY) < 0)
    {
        perror( "Socket Setup");
        return FALSE;
    }
#endif  
#endif  
    return PASS;
}


/*****************************************************************************
*
*   functionName:   wsockBroadcastClientInit
*   Description:
*
*   Comments:           
*
******************************************************************************/
int wsockBroadcastClientInit( wsockType *wsock, unsigned short networkPort )
{
    int err;

    wsockInitConfig( wsock, SOCK_DGRAM );
    wsockLocalAddress( wsock );
    
    if(INVALID_SOCKET == (wsock->data.msgsock = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        printf("Error creating socket: %d\n%s", wsockGetLastError(), 
            wsockErrorStrings( wsockGetLastError() ));
        return -1;
    }
    wsock->data.wsockOpened = TRUE;

    wsock->data.client.sin_family = AF_INET;
    wsock->data.client.sin_addr.s_addr = htonl ( INADDR_ANY );
    wsock->data.client.sin_port = htons ( networkPort );

    err = bind(wsock->data.msgsock, 
        (const struct sockaddr *) &wsock->data.client, sizeof(struct sockaddr_in));

    wsockConfigChannel( wsock->data.msgsock );

    if(SOCKET_ERROR == err)
    {
        printf("Error binding socket: %d\n%s", wsockGetLastError(), 
            wsockErrorStrings( wsockGetLastError() ));

	    closesocket(wsock->data.msgsock);
        return -1;
    }
    wsock->role = WSOCK_ROLE_CLIENT;

    return TRUE;
}

  
/*****************************************************************************
*
*   functionName:   wsockBroadcastReadByte
*   Description:
*
*   Comments:           
*
******************************************************************************/
int wsockBroadcastReadByte( wsockType *wsock, char *c )
{
    struct sockaddr_in server;


    /* return data already in the buffer */
    if(wsock->data.dwRead > wsock->data.dwReturned)
    {
        *c = wsock->data.buf[wsock->data.dwReturned++];
        return 1;
    }

    wsock->data.dwRead = 0;
    wsock->data.dwReturned = 0;
/*
#if defined _Windows || defined __WIN32__ || defined WINDOWS || defined WIN32
    int fromlen = sizeof(struct sockaddr_in); 
    wsock->data.dwRead = recvfrom(wsock->data.msgsock, 
        wsock->data.buf, sizeof(wsock->data.buf), 0, 
        (struct sockaddr *)&server, &fromlen);
#endif

#if defined VPR_OS_IRIX || defined VPR_OS_Linux || defined VPR_OS_Solaris
    int fromlen = sizeof(struct sockaddr_in); 
    wsock->data.dwRead = recvfrom(wsock->data.msgsock, 
        wsock->data.buf, sizeof(wsock->data.buf), 0, 
        (struct sockaddr_in *)&server, &fromlen);
#endif
*/
    if(wsock->data.dwRead == SOCKET_ERROR)
    {
#if defined _Windows || defined __WIN32__ || defined WINDOWS || defined WIN32
        int lasterr;
    
        lasterr = wsockGetLastError();

        /* WSAEWOULDBLOCK is not an error, there is no data available */
        if(lasterr != WSAEWOULDBLOCK) 
        {
            printf("recvfrom() failed: error %d\n%s",
                wsockGetLastError(), wsockErrorStrings( wsockGetLastError() ));
        }
#endif
    }
    if(wsock->data.dwRead > wsock->data.dwReturned)
    {
        *c = wsock->data.buf[wsock->data.dwReturned++];

        if(!wsock->udpHostInit)
        {
            /* UDP server info */
            if(memcmp((void *) &server, 
                (void *) &wsock->data.server, sizeof(struct sockaddr_in)) != 0)
            {
                memcpy((void *) &wsock->data.server, (void *) &server,
                  sizeof(struct sockaddr_in));
            
                printf("\nReceiving data from %s", inet_ntoa(server.sin_addr));
            }
            wsock->udpHostInit = TRUE;
        }
        return 1;
    }
    return EOF;
}


/*****************************************************************************
*
*   functionName:   wsockErrorStrings
*   Description:
*
*   Comments:           
*
******************************************************************************/
const char *wsockErrorStrings( int err )
{
#if defined _Windows || defined __WIN32__ || defined WINDOWS || defined WIN32

    switch(err)
    {
    case WSANOTINITIALISED: return("Could not locate usable winsock implementation"); break;
    case WSAENETDOWN:       return("Network subsystem is down."); break;
    case WSAHOST_NOT_FOUND: return("Host not found."); break;
    case WSATRY_AGAIN:      return("Host not found"); break;
    case WSANO_RECOVERY:    return("Non-recoverable errors."); break;
    case WSANO_DATA:        return("Valid name, no data record of requested type."); break;
    case WSAEINPROGRESS:    return("Blocking call in progress"); break;
    case WSAEINTR:          return("The (blocking) call was interrupted"); break;    
    case WSAENOBUFS:        return("Buffer space is insufficient or unavailable."); break;
    case WSAEWOULDBLOCK:    return("Socket marked as non-blocking"); break;
    case WSASYSNOTREADY:    return("Network subsystem not ready"); break;
    case WSAVERNOTSUPPORTED:return("The version of Winsock support requested is not available."); break;
    case WSAEPROTONOSUPPORT:return("Protocol is not supported"); break;	 
    case WSAESOCKTNOSUPPORT:return("Socket type not supported by this address family"); break;
    case WSAEINVAL:         return("The Winsock version specified by the application is not supported."); break; 
    case WSAEACCES:         return("Requested broadcast address, but the appropriate flag was not set."); break;
    case WSAEFAULT:         return("Bad address"); break;
    case WSAENETRESET:	    return("Connection has been dropped."); break;
    case WSAENOTCONN:	    return("The socket is not connected (TCP only)."); break;
    case WSAENOTSOCK:	    return("The descriptor is not a socket."); break;
    case WSAEOPNOTSUPP:	    return("Option not supported"); break;
    case WSAESHUTDOWN:	    return("The socket has been shut down; it is not possible to send."); break;
    case WSAEMSGSIZE:	    return("The socket is of UDP type, and the datagram to large"); break;
    case WSAECONNABORTED:   return("Connection was aborted due to timeout or other failure."); break;
    case WSAECONNRESET:	    return("Connection was reset by the remote side."); break;
    case WSAEADDRNOTAVAIL:  return("The specified address is not available from the local computer."); break;
    case WSAEAFNOSUPPORT:   return("Addresses in the specified family can't be used with this socket."); break;
    case WSAEDESTADDRREQ:   return("A destination address is required."); break;
    case WSAENETUNREACH:    return("The network can't be reached from this host at this time."); break;
    case WSAEADDRINUSE:     return("The specified address is already in use."); break;
    case WSAEISCONN:	    return("The socket is already connected."); break;
    case WSAEMFILE:         return("Too many open files."); break;
    case WSAECONNREFUSED:   return("The attempt to connect was rejected."); break;
    case WSAETIMEDOUT:	    return("Command timed out"); break;
    case WSAENOPROTOOPT:    return("Protocol option not supported for this socket type"); break;
    case WSAEHOSTUNREACH:   return("Host is unreachable."); break;
    case WSAEBADF:          return("Bad file number"); break;	           
    case WSAEALREADY:       return("The asynchronous routine being canceled has already completed"); break;
    case WSAEPROTOTYPE:     return("Protocol is of wrong type for this socket"); break;	      
    case WSAEPFNOSUPPORT: 
    case WSAETOOMANYREFS:
    case WSAELOOP:     
    case WSAENAMETOOLONG: 
    case WSAEHOSTDOWN:  
    case WSAENOTEMPTY:    
    case WSAEPROCLIM:   
    case WSAEUSERS:       
    case WSAEDQUOT:    
    case WSAESTALE:      
    case WSAEREMOTE:  
    default:	            return("Unknown error"); break;
    }
#endif

#if defined VPR_OS_IRIX || defined VPR_OS_Linux || defined VPR_OS_Solaris
    return("");
#endif
}                     

/*****************************************************************************
*
*   functionName:   wsockState
*   Description:
*
*   Comments:           
*
******************************************************************************/
int wsockState( wsockType *wsock )
{
    return wsock->data.wsockOpened;
}

/*****************************************************************************
*
*   functionName:   wsockIsClient
*   Description:
*
*   Comments:           
*
******************************************************************************/
int wsockIsClient( InterSenseTrackerType *tracker )
{
    return (tracker->wsock.role == WSOCK_ROLE_CLIENT && wsockState( &tracker->wsock ));
}


/*****************************************************************************
*
*   functionName:   wsockClientDecodeData
*   Description:
*
*   Comments:           
*
******************************************************************************/
int wsockClientDecodeData( InterSenseTrackerType *tracker )
{
    unsigned i;
    unsigned char cSum;
    unsigned char *buf;

    ItComSensorStateType *station;
    udpStationPacketType  stationData;
    udpCameraPacketType   cameraData;


    buf = tracker->wsock.data.cmdbuf;

    if(*buf++ != UDP_START_BYTE) return FALSE;

    switch(*buf--)
    {
    case UDP_STATION_DATA:

        memcpy((void *)&stationData, buf, sizeof(udpStationPacketType));
        buf = (unsigned char *) &stationData + 4;
        
        for(cSum = 0, i = 0; i < sizeof(udpStationPacketType) - 4; i++)
        {
            cSum += *buf++;
        }
        if(stationData.CheckSum != cSum)
        {
           return FALSE;
        }
        if(stationData.StationNum != 
            MINMAX(1, stationData.StationNum, MAX_NUM_STATIONS)) return FALSE;

        station = &tracker->station[stationData.StationNum - 1];

        station->Orientation[0] = byteOrderFloat((char *)&stationData.Yaw);
        station->Orientation[1] = byteOrderFloat((char *)&stationData.Pitch);
        station->Orientation[2] = byteOrderFloat((char *)&stationData.Roll);
        station->Position[0]    = byteOrderFloat((char *)&stationData.PosX);
        station->Position[1]    = byteOrderFloat((char *)&stationData.PosY);
        station->Position[2]    = byteOrderFloat((char *)&stationData.PosZ);
        station->Status         = stationData.Status;
        station->TimeStamp      = byteOrderFloat((char *)&stationData.TimeStamp);   

        tracker->hardware = ISD_PRECISION_SERIES;
        tracker->state.hardwareVersion = stationData.Model;
        tracker->station[stationData.StationNum - 1].state = ON;
        tracker->station[stationData.StationNum - 1].NewData = TRUE;
        
        for(i = 0; i < UDP_MAX_ANALOG_CHANNELS; i++)
        {
            station->AnalogData[i] = (short) stationData.AnalogData[i];
        }
        for(i = 0; i < MAX_NUM_BUTTONS; i++)
        {
            if(stationData.ButtonState & (1 << i))
                station->ButtonState[i] = 1;
            else
                station->ButtonState[i] = 0;
        }
        
        break;

    case UDP_CAMERA_DATA:

        memcpy((void *)&cameraData, buf, sizeof(udpCameraPacketType));
        buf = (unsigned char *) &cameraData + 4;
        
        for(cSum = 0, i = 0; i < sizeof(udpCameraPacketType) - 4; i++)
        {
            cSum += *buf++;
        }
        if(cameraData.CheckSum != cSum) return FALSE;
       
        if(cameraData.StationNum != 
            MINMAX(1, cameraData.StationNum, MAX_NUM_STATIONS)) return FALSE;

        station = &tracker->station[cameraData.StationNum - 1];

        station->Timecode         = byteOrderULong((char *)&cameraData.Timecode);
        station->ApetureEncoder   = byteOrderLong ((char *)&cameraData.ApetureEncoder);
        station->FocusEncoder     = byteOrderLong ((char *)&cameraData.FocusEncoder);
        station->ZoomEncoder      = byteOrderLong ((char *)&cameraData.ZoomEncoder);
        station->TimeCodeUserBits = byteOrderULong((char *)&cameraData.TimeCodeUserBits);
        station->Apeture          = byteOrderFloat((char *)&cameraData.Apeture);
        station->Focus            = byteOrderFloat((char *)&cameraData.Focus);
        station->FOV              = byteOrderFloat((char *)&cameraData.FOV);
        station->NodalPoint       = byteOrderFloat((char *)&cameraData.NodalPoint);

        break;

    default:
        return FALSE;
    }

    return TRUE;
}


/*****************************************************************************
*
*   functionName:   wsockReceiveData
*   Description:
*
*   Comments:           
*
******************************************************************************/
int wsockReceiveData( InterSenseTrackerType *tracker )
{
    unsigned char c;
    int num = 0, numSamples = 0;
    wsockChannelType *data;
    
    if(!wsockIsClient(tracker)) return 0;

    data = &tracker->wsock.data;

    while(wsockBroadcastReadByte(&tracker->wsock, (char *) &c) != EOF)
    {
        num++;
        tracker->numBytesReceived++;
 
        switch(data->nchars)
        {
        case 0:                             /* new record */
            if(c == UDP_START_BYTE)
            {                              
                data->cmdbuf[data->nchars++]=c;
            }
            break;
            
        case 1:                             
            if(c == UDP_STATION_DATA)       /* still a valid packet */
            {
                data->cmdbuf[data->nchars++] = c;
                data->expectedSize = sizeof(udpStationPacketType);
            }
            else if(c == UDP_CAMERA_DATA)
            {
                data->cmdbuf[data->nchars++] = c;
                data->expectedSize = sizeof(udpCameraPacketType);
            }
            else                            /* error, reset */
            {
                data->expectedSize = 0;
                data->nchars = 0;
            }
            break;
            
        default:                            /* continuing record */
            data->cmdbuf[data->nchars++]=c;
            if(data->nchars == data->expectedSize)
            {
                data->expectedSize = 0;
                data->nchars = 0;

                if(wsockClientDecodeData(tracker))
                {
                    tracker->numSamplesReceived++;
                    numSamples++;
                }
            }
            break;
        }
        if(num > 1000)
        {
            break;
        }
    }
    
    return numSamples;
}


/*****************************************************************************
*
*   functionName:   wsockGetStationConfig
*   Description:
*
*   Comments:       
*
******************************************************************************/
BOOL wsockGetStationConfig( InterSenseTrackerType *tracker, 
                            ISD_STATION_INFO_TYPE *Station,
                            WORD stationNum, BOOL verbose )
{
    /* fill in the defaults */
    memset((void *) Station, 0, sizeof(ISD_STATION_INFO_TYPE));

    Station->ID             = stationNum;
    Station->CoordFrame     = ISD_DEFAULT_FRAME;
    Station->InertiaCube    = 1;
    Station->Sensitivity    = 3;
    Station->Enhancement    = 2;
    Station->Compass        = 2;
    Station->Prediction     = 0;
    Station->State          = tracker->station[stationNum-1].state;

    Station->TimeStamped = TRUE;
    Station->AngleFormat = ISD_EULER;
    
    if(tracker->state.hardwareVersion == IS900)
    {
        Station->GetAnalogData = TRUE;
        Station->GetButtons = TRUE;
        Station->GetCameraDataRaw = TRUE;
        Station->GetCameraDataComputed = FALSE;
    }
    else
    {
        Station->GetAnalogData = FALSE;
        Station->GetButtons = FALSE;
        Station->GetCameraDataRaw = FALSE;
        Station->GetCameraDataComputed = FALSE;
    }

    return PASS;
}

/*****************************************************************************
*
*   functionName:   wsockSetStationConfig
*   Description:
*
*   Comments:       
*
******************************************************************************/
BOOL wsockSetStationConfig ( InterSenseTrackerType *tracker, 
                             ISD_STATION_INFO_TYPE *Station,
                             WORD stationNum, BOOL verbose )
{
    return wsockGetStationConfig(tracker, Station, stationNum, verbose);
}
