/***********************************************************************
*
*    File Name:    serial.c
*    Description:  rs232 comminications interface.
*    Created:      9/2/1997
*    Author:       Yury Altshuler
*
*    Copyright:    InterSense 1999 - All rights Reserved.
*
*    Comments:     Win32 and UNIX rs232 driver
*
*
************************************************************************/
/* Header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "isense.h"
#include <vpr/vpr.h>

#include <boost/concept_check.hpp>

#ifdef VPR_OS_Win32

#include <windows.h>
#include <process.h>
#include <winbase.h>
#include <time.h>

#include "serial.h"
#include "isdriver.h"
#include "timer.h"

static BOOL setCommStateBYTE(COMM_PORT *port, DCB *dcb, BYTE *target, BYTE value);
static BOOL setCommStateDWORD(COMM_PORT *port, DCB *dcb, DWORD *target, DWORD value);
static void errorMessage(char *message);


/************************** errorMessage ***************************/
static void errorMessage(char *message)
{
    printf("%s\n", message);
}

/********************** rs232InitCommunications ********************/
//int rs232InitCommunicationsOld(COMM_PORT *port, DWORD comPort, DWORD baudRate)
int rs232InitCommunications(COMM_PORT *port, std::string comPort, int baud_rate)
{
    COMMTIMEOUTS timeout;
    DCB dcb;
    const char *openString = "9600,N,8,1";

    /* close port if one is open */
    rs232DeinitCommunications(port);

    FillMemory(&dcb, sizeof(dcb), 0);
    port->portHandle = CreateFile((LPCSTR)comPort.c_str(),
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            0,
                            OPEN_EXISTING,
                            FILE_FLAG_OVERLAPPED,
                            0);

    if(port->portHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    if (!GetCommTimeouts(port->portHandle,&timeout))
    {
        errorMessage( "Could not get timeout info" );
        CloseHandle(port->portHandle);
        port->portHandle = NULL;
        return FALSE;
    }

    timeout.ReadIntervalTimeout = MAXDWORD;
    timeout.ReadTotalTimeoutMultiplier = 0;
    timeout.ReadTotalTimeoutConstant = 0;
    timeout.WriteTotalTimeoutMultiplier = 10;
    timeout.WriteTotalTimeoutConstant = 100;

    if(!SetCommTimeouts(port->portHandle, &timeout))
    {
        errorMessage( "Could not set timeout info");
        CloseHandle(port->portHandle);
        port->portHandle = NULL;
        return FALSE;
    }

    FillMemory(&dcb, sizeof(dcb), 0);
    dcb.DCBlength = sizeof(dcb);

    if (!GetCommState(port->portHandle, &dcb))
    {
        errorMessage( "Failed to get communications state");
        CloseHandle(port->portHandle);
        port->portHandle = NULL;
        return FALSE;
    }

    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.ByteSize = 8;
    dcb.DCBlength = sizeof(dcb);


    DWORD  baudRate;
    switch (baud_rate)
    {
    case 9600:
       baudRate = 9600L;
       break;
    case 19200:
       baudRate = 19200L;
       break;
    case 38400:
       baudRate = 38400L;
       break;
    case 57600:
       baudRate = 57600L;
       break;
    case 115200:
       baudRate = 115200L;
       break;
    default:
       baudRate = 9600L;
       break;
    }



    dcb.BaudRate = baudRate;
    dcb.fNull = FALSE;
    dcb.fBinary = TRUE;
    dcb.fAbortOnError = FALSE;

    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;

    if (!SetCommState(port->portHandle, &dcb))
    {
        errorMessage( "Failed to set communications state");
        CloseHandle(port->portHandle);
        port->portHandle = NULL;
        return FALSE;
    }

    port->rx_bufsize = RX_BUFFER_SIZE;
    port->dwRead = 0;
    port->dwReturned = 0;
    port->fWaitingOnRead = FALSE;
    //port->portNumber = (WORD) comPort;


    return TRUE;
}

/********************** rs232InitCommunications ********************/
int rs232InitCommunicationsOld(COMM_PORT *port, DWORD comPort, DWORD baudRate)
{
    COMMTIMEOUTS timeout;
    DCB dcb;
    const char *openString = "9600,N,8,1";

    /* close port if one is open */
    rs232DeinitCommunications(port);

    FillMemory(&dcb, sizeof(dcb), 0);
    port->portHandle = CreateFile((comPort == 1 ? "COM1" : (comPort == 2 ? "COM2" :
                            (comPort == 3 ? "COM3" : "COM4"))),
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            0,
                            OPEN_EXISTING,
                            FILE_FLAG_OVERLAPPED,
                            0);

    if(port->portHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    if (!GetCommTimeouts(port->portHandle,&timeout))
    {
        errorMessage( "Could not get timeout info" );
        CloseHandle(port->portHandle);
        port->portHandle = NULL;
        return FALSE;
    }

    timeout.ReadIntervalTimeout = MAXDWORD;
    timeout.ReadTotalTimeoutMultiplier = 0;
    timeout.ReadTotalTimeoutConstant = 0;
    timeout.WriteTotalTimeoutMultiplier = 10;
    timeout.WriteTotalTimeoutConstant = 100;

    if(!SetCommTimeouts(port->portHandle, &timeout))
    {
        errorMessage( "Could not set timeout info");
        CloseHandle(port->portHandle);
        port->portHandle = NULL;
        return FALSE;
    }

    FillMemory(&dcb, sizeof(dcb), 0);
    dcb.DCBlength = sizeof(dcb);

    if (!GetCommState(port->portHandle, &dcb))
    {
        errorMessage( "Failed to get communications state");
        CloseHandle(port->portHandle);
        port->portHandle = NULL;
        return FALSE;
    }

    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.ByteSize = 8;
    dcb.DCBlength = sizeof(dcb);

    dcb.BaudRate = baudRate;
    dcb.fNull = FALSE;
    dcb.fBinary = TRUE;
    dcb.fAbortOnError = FALSE;

    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;

    if (!SetCommState(port->portHandle, &dcb))
    {
        errorMessage( "Failed to set communications state");
        CloseHandle(port->portHandle);
        port->portHandle = NULL;
        return FALSE;
    }

    port->rx_bufsize = RX_BUFFER_SIZE;
    port->dwRead = 0;
    port->dwReturned = 0;
    port->fWaitingOnRead = FALSE;
    port->portNumber = (WORD) comPort;


    return TRUE;
}


/******************** rs232DeinitCommunications **********************/
int rs232DeinitCommunications(COMM_PORT *port)
{
    if(port->portHandle != NULL)
    {
        rs232RxFlush(port, 0);
        CloseHandle(port->portHandle);
        port->portHandle = NULL;
    }
    return TRUE;
}


/************************* setCommStateBYTE ***************************/
static BOOL setCommStateBYTE(COMM_PORT *port, DCB *dcb, BYTE *target, BYTE value)
{
    BOOL fSuccess;

    FillMemory(dcb, sizeof(*dcb), 0);

    if (!GetCommState(port->portHandle, dcb))     /* get current DCB */
    {
        errorMessage( "Failed to set control byte");
        return FALSE;
    }
    *target = value;

    fSuccess = SetCommState(port->portHandle, dcb);
    rs232RxFlush(port, 0);

    if (!fSuccess)
    {
         errorMessage( "Failed to set control byte");
         return FALSE;
    }
    return TRUE;
}


/********************** setCommStateDWORD **************************/
static BOOL setCommStateDWORD(COMM_PORT *port, DCB *dcb, DWORD *target, DWORD value)
{
    BOOL fSuccess;

    FillMemory(dcb, sizeof(*dcb), 0);

    if (!GetCommState(port->portHandle, dcb))     /* get current DCB */
    {
        errorMessage( "Failed to set control word");
        return FALSE;
    }
    *target = value;

    fSuccess = SetCommState(port->portHandle, dcb);
    rs232RxFlush(port, 0);

    if (!fSuccess)
    {
         errorMessage( "Failed to set control word");
         return FALSE;
    }
    return TRUE;
}


/************************** rs232SetSpeed ****************************/
int rs232SetSpeed(COMM_PORT *port, DWORD baudRate)
{
    DCB dcb;
    return( setCommStateDWORD(port, &dcb, &(dcb.BaudRate), baudRate));
}


/************************** rs232RxFlush *****************************/
int rs232RxFlush(COMM_PORT *port, WORD numBytes)
{
    port->dwRead = 0;
    port->dwReturned = 0;
    if(port->fWaitingOnRead == TRUE)  /* waiting for read to compete */
    {
        port->fWaitingOnRead = FALSE;
        CloseHandle(port->osReader.hEvent);
    }

    if(PurgeComm(port->portHandle, PURGE_RXCLEAR)) return EOF;
    else return 0;
}


/************************** rs232SendString **************************/
int rs232SendString(COMM_PORT *port, char *commandString)
{
    BOOL fSuccess;
    DWORD numWritten;

    fSuccess = WriteFile(port->portHandle, (LPCVOID)commandString,
                         (DWORD)strlen((const char *)commandString), &numWritten, &(port->osWrite));

    if (!fSuccess || numWritten == 0) return 1;
    else return 0;
}


/************************** rs232PutChar ***************************/
int rs232PutChar(COMM_PORT *port, char c)
{
    BOOL fSuccess;
    DWORD numWritten;

    fSuccess = WriteFile(port->portHandle, (LPCVOID)&c, 1, &numWritten,
                         &(port->osWrite));

    if (!fSuccess || numWritten == 0) return 1;
    return 0;
}


/************************** rs232InChar ****************************/
int rs232InChar(COMM_PORT *port, char *c, int flush)
{
    DWORD dwRes;

    /* flush the input buffer. All characters already in the buffer are
       discarded. Overlaped read process is terminated */

    if(flush || port->dwReturned >= port->rx_bufsize-1)
    {
        port->dwRead = 0;
        port->dwReturned = 0;
        if(port->fWaitingOnRead == TRUE)  /* waiting for read to compete */
        {
            port->fWaitingOnRead = FALSE;
            CloseHandle(port->osReader.hEvent);
        }
    }

    /* return data already in the buffer */
    if(port->dwRead > port->dwReturned)
    {
        *c = port->rxbuf[port->dwReturned++];
        return 1;
    }

    if(port->fWaitingOnRead == FALSE)
    {
        /* Create the overlapped event. Must be closed before exiting
         to avoid a handle leak */

        FillMemory(&(port->osReader), sizeof(port->osReader), 0);
        port->osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        port->dwReturned = 0;
        port->dwRead = 0;

        if (port->osReader.hEvent == NULL)  /* Error creating overlapped event; abort.  */
        {
            return EOF;
        }

        /* Issue read operation.  */
        if (!ReadFile(port->portHandle, port->rxbuf, port->rx_bufsize, (LPDWORD)&(port->dwRead), &(port->osReader)))
        {
            if (GetLastError() != ERROR_IO_PENDING)  /* read not delayed? */
            {
                CloseHandle(port->osReader.hEvent);
                port->dwRead = 0;
            }
            else
            {
                port->fWaitingOnRead = TRUE;
            }
        }
        else  /* read completed */
        {
            CloseHandle(port->osReader.hEvent);
        }
    }

    if(port->fWaitingOnRead == TRUE)  /* waiting for read to compete */
    {
        dwRes = WaitForSingleObject(port->osReader.hEvent, 0);
        switch(dwRes)
        {
            case WAIT_OBJECT_0:
                if (!GetOverlappedResult(port->portHandle, &(port->osReader), (LPDWORD)&(port->dwRead), FALSE))
                {
                    /* function failed */
                    if (GetLastError() != ERROR_IO_INCOMPLETE) /* comm error; abort */
                    {
                        port->fWaitingOnRead = FALSE;
                        CloseHandle(port->osReader.hEvent);
                        port->dwRead = 0;
                    }
                }
                else
                {
                    port->fWaitingOnRead = FALSE;
                    CloseHandle(port->osReader.hEvent);
                }
                break;

            case WAIT_TIMEOUT:
                /* Operation isn't complete yet. fWaitingOnRead flag isn't
                   changed.*/
                break;

            default:
                /* Error in the WaitForSingleObject; abort.
                   This indicates a problem with the OVERLAPPED structure's
                   event handle. */
                port->fWaitingOnRead = FALSE;
                CloseHandle(port->osReader.hEvent);
                port->dwRead = 0;
        }
    }
    if(port->dwRead > port->dwReturned)
    {
        *c = port->rxbuf[port->dwReturned++];
        return 1;
    }
    return EOF;
}


/************************** rs232RxCount ***************************/
WORD rs232RxCount(COMM_PORT *port)
{
    BOOL  fSuccess;
    DWORD numRead;
    DWORD numAvailable;
    int c;

    fSuccess = PeekNamedPipe(port->portHandle, (LPVOID)&c, 1,
                             &numRead, &numAvailable, NULL);

    if (!fSuccess || numRead==0) return 0;
    else return((WORD)numAvailable);
}


/********************** setCommStateRTSState *****************************/
BOOL rs232SetRTSState(COMM_PORT *port, DWORD value)
{
    BOOL fSuccess;
    DCB dcb;

    FillMemory(&dcb, sizeof(dcb), 0);

    if (!GetCommState(port->portHandle, &dcb))     /* get current DCB */
    {
        errorMessage( "Failed to set RTS line state" );
        return FALSE;
    }
    dcb.fRtsControl = value;

    fSuccess = SetCommState(port->portHandle, &dcb);
    rs232RxFlush(port, 0);

    if (!fSuccess)
    {
         errorMessage( "Failed to set RTS line state" );
         return FALSE;
    }
    return TRUE;
}

#endif

#if !defined VPR_OS_Win32

/***********************************************************************
*
*    Description:  UNIX rs232 driver
*
*    Copyright:    InterSense 1999 - All rights Reserved.
*
************************************************************************/

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <fcntl.h>

#ifdef VPR_OS_IRIX
#include <sys/z8530.h> /* SGI only */
#endif

#ifdef VPR_OS_Solaris
#include <termio.h>
#endif

#include "serial.h"
#include "isdriver.h"
#include "timer.h"

/****************************************************************************/
int rs232InitCommunicationsOld(COMM_PORT *port, DWORD comPort, DWORD baudRate)
{
    char *portNames[4] =
#if defined VPR_OS_IRIX
    {"/dev/ttyd11", "/dev/ttyd11", "/dev/ttyd11", "/dev/ttyd11"};
#elif defined VPR_OS_Linux
    {"/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2", "/dev/ttyS3"};
#elif defined VPR_OS_Solaris
    {"/dev/ttya", "/dev/ttyb", "/dev/ttyc", "/dev/ttyd"};
#else
    {"/dev/tty1", "/dev/tty2", "/dev/tty3", "/dev/tty4"};
#endif

    /* close port if one is open */
    rs232DeinitCommunications(port);

    if(comPort < 1) comPort = 1;

    port->rx_bufsize = RX_BUFFER_SIZE;
    port->dwRead = 0;
    port->dwReturned = 0;
    port->portNumber = (WORD) comPort;

    printf("Port %s, baud rate %lu\n", portNames[comPort - 1], baudRate);
    port->desc = open(portNames[comPort - 1], O_RDWR);

    if ( port->desc <= 0)
    {
        perror("Open port:");
        return FALSE;
    }

    rs232SetSpeed(port, baudRate);

#ifdef VPR_OS_IRIX
    /* This removes the 50 Hz limit on the serial port
       access on some SGI computers. */

    if( ioctl(port->desc, SIOC_ITIMER, 0 ) == -1 )
    {
        perror( "Setting up real-time serial port access" );
        return FALSE;
    }
#endif

#if defined VPR_OS_Solaris
    if (fcntl(port->desc, F_SETFL, O_NDELAY) < 0)
    {
        perror( "Tracker-Port Setup");
        return FALSE;
    }
#else
    if (fcntl(port->desc, F_SETFL, FNDELAY) < 0)
    {
        perror( "Tracker-Port Setup");
        return FALSE;
    }
#endif

    if( tcflush(port->desc, TCIOFLUSH ) == -1 )
    {
        perror( "Flushing Port for InterSense Tracker" );
        return FALSE;
    }

    return TRUE;
}

int rs232InitCommunications(COMM_PORT *port, std::string comPort, int baud_rate)
{
#if defined VPR_OS_IRIX
   printf("Driver is compiled for IRIX\n");
#elif defined VPR_OS_Linux
   printf("Driver is compiled for Linux\n");
#elif defined VPR_OS_Solaris
   printf("Driver is compiled for Solaris\n");
#else
   printf("Driver is compiled for UNKNOWN\n");
#endif

    /* close port if one is open */
    rs232DeinitCommunications(port);

    port->rx_bufsize = RX_BUFFER_SIZE;
    port->dwRead = 0;
    port->dwReturned = 0;

    printf("Port %s, baud rate %d\n", comPort.c_str(), baud_rate);
    port->desc = open(comPort.c_str(), O_RDWR);

    if ( port->desc <= 0)
    {
        perror("ERROR, Could not open serial port!\n");
        return FALSE;
    }

    DWORD  baudRate;
    switch (baud_rate)
    {
    case 9600:
       baudRate = 9600L;
       break;
    case 19200:
       baudRate = 19200L;
       break;
    case 38400:
       baudRate = 38400L;
       break;
    case 57600:
       baudRate = 57600L;
       break;
    case 115200:
       baudRate = 115200L;
       break;
    default:
       baudRate = 9600L;
       break;
    }

    rs232SetSpeed(port, baudRate);

#ifdef VPR_OS_IRIX
    /* This removes the 50 Hz limit on the serial port
       access on some SGI computers. */

    if( ioctl(port->desc, SIOC_ITIMER, 0 ) == -1 )
    {
        perror( "Setting up real-time serial port access" );
        return FALSE;
    }
#endif

#if defined VPR_OS_Solaris
    if (fcntl(port->desc, F_SETFL, O_NDELAY) < 0)
    {
        perror( "Tracker-Port Setup");
        return FALSE;
    }
#else
    if (fcntl(port->desc, F_SETFL, FNDELAY) < 0)
    {
        perror( "Tracker-Port Setup");
        return FALSE;
    }
#endif

    if( tcflush(port->desc, TCIOFLUSH ) == -1 )
    {
        perror( "Flushing Port for InterSense Tracker" );
        return FALSE;
    }

    return TRUE;
}

//////////////////////////////////////////////
/****************************************************************************/
int rs232SetSpeed(COMM_PORT *port, DWORD baudRate)
{
    struct termio terminfo;

    if( ioctl(port->desc, TCGETA, &terminfo ) == -1 )
    {
        perror( "Configuring Port for InterSense Tracker" );
        return FALSE;
    }

    terminfo.c_iflag = 0;
    terminfo.c_oflag = 0;
    terminfo.c_lflag = 0;
    terminfo.c_cc[4] = 0;
    terminfo.c_cc[5] = 5;

#if defined VPR_OS_IRIX

   #if defined LEGACY_SGI

       terminfo.c_cflag = (baudRate == 1200  ?  B1200 :
                           baudRate == 9600  ?  B9600 :
                           baudRate == 19200 ?  B19200 :
                           baudRate == 38400 ?  B38400 : B115200) |CS8|CREAD|CLOCAL|CNEW_RTSCTS;

   #else

       terminfo.c_cflag = CS8|CREAD|CLOCAL|CNEW_RTSCTS;
       terminfo.c_ospeed = (baudRate == 1200  ?  B1200 :
                            baudRate == 9600  ?  B9600 :
                            baudRate == 19200 ?  B19200 :
                            baudRate == 38400 ?  B38400 : B115200);
       terminfo.c_ispeed = terminfo.c_ospeed;

   #endif

#else /* other flavors of UNIX, including Linux */

    terminfo.c_cflag  = (baudRate == 1200  ?  B1200 :
                         baudRate == 9600  ?  B9600 :
                         baudRate == 19200 ?  B19200 :
                         baudRate == 38400 ?  B38400 : B115200)|CS8|CREAD|CLOCAL;
#endif

    if( ioctl(port->desc, TCSETA, &terminfo ) == -1 )
    {
        perror( "Configuring Port for InterSense Tracker" );
        return FALSE;
    }

    if( tcflush(port->desc, TCIOFLUSH ) == -1 )
    {
        perror( "Flushing Port for InterSense Tracker" );
        return FALSE;
    }

    rs232SetRTSState(port, TRUE);

    return TRUE;
}


/********************** setCommStateRTSState *****************************/
BOOL rs232SetRTSState(COMM_PORT *port, DWORD value)
{
#if defined USE_RTS_LINE
    int status;

    if(ioctl(port->desc, TIOCMGET, &status) == -1)
    {
        perror( "Get RTS state" );
        return FALSE;
    }

    if(value)
        status |= TIOCM_RTS;
    else
        status &= ~TIOCM_RTS;

    if(ioctl(port->desc, TIOCMSET, &status) == -1)
    {
        perror( "Set RTS state" );
        return FALSE;
    }
    rs232RxFlush(port, 1);
#endif

	return TRUE;
}


/****************************************************************************/
int rs232RxFlush(COMM_PORT *port, WORD numBytes)
{
   boost::ignore_unused_variable_warning(numBytes);
    port->dwRead = 0;
    port->dwReturned = 0;
    if( tcflush(port->desc, TCIFLUSH ) == -1 )
    {
        perror( "Flushing Port for InterSense Tracker" );
        return FALSE;
    }
    return 1;
}


/****************************************************************************/
int rs232DeinitCommunications(COMM_PORT *port)
{
    if(port->desc)
    {
        close(port->desc);
    }
    port->desc = 0;
    return TRUE;
}

/****************************************************************************/
int rs232SendString(COMM_PORT *port, char *commandString)
{
    int len;
    len = strlen(commandString);
    return(write(port->desc, commandString, len) == len);
}


/****************************************************************************/
int rs232PutChar(COMM_PORT *port, char c)
{
    if(write(port->desc, &c, 1) == 1)
    {
        return 1;
    }
    return EOF;
}


/****************************************************************************/
/* return one byte */
int rs232InChar(COMM_PORT *port, char *c, int flush)
{
    /* flush the input buffer. All characters already in the buffer are
       discarded. Also, flush if overflow is detected */

    if(flush || port->dwReturned >= LONG(port->rx_bufsize-1))
    {
        port->dwRead = 0;
        port->dwReturned = 0;
    }

    /* return data already in the buffer */
    if(port->dwRead > port->dwReturned)
    {
        *c = port->rxbuf[port->dwReturned++];
        return 1;
    }

    /* buffer exhausted, need to read again */
    port->dwReturned = 0;
    port->dwRead = 0;

    /* port must be setup for non-blocking operation so system call should return with
       the data already available and not wait for the entire rx_bufsize */

    if((port->dwRead = read(port->desc, &port->rxbuf[0], port->rx_bufsize)) > 0)
    {
        *c = port->rxbuf[port->dwReturned++];
        return 1;
    }

    return EOF;
}


#endif



/*************************** waitForChar ***********************************/
BOOL waitForChar( COMM_PORT *port, char *ch )
{
    float startTime = timeNow();

    while(rs232InChar(port, ch, TRUE) == EOF)
    {
        if( ( timeNow() - startTime ) > 0.4 ) /* time out */
        {
            return FALSE;
        }
    }
    return TRUE;
}


/**************************** waitForShort *********************************/
BOOL waitForShort( COMM_PORT *port, short *num )
{
    union {
       char bytes[2];
       short word;
    } bytesToWord;

#if defined REVERSE_BYTE_ORDER
    if( !waitForChar( port, &bytesToWord.bytes[1] ))  return FALSE;
    if( !waitForChar( port, &bytesToWord.bytes[0] ))  return FALSE;
#else
    if( !waitForChar( port, &bytesToWord.bytes[0] ))  return FALSE;
    if( !waitForChar( port, &bytesToWord.bytes[1] ))  return FALSE;
#endif

    *num = bytesToWord.word;

    return TRUE;
}













