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

#if !defined UNIX 

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
int rs232InitCommunications(COMM_PORT *port, const char* comPort, DWORD baudRate)
{
    COMMTIMEOUTS timeout;
    DCB dcb;
    const char *openString = "9600,N,8,1";

    /* close port if one is open */
    rs232DeinitCommunications(port);

    FillMemory(&dcb, sizeof(dcb), 0);
    port->portHandle = CreateFile( comPort,
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
        return FALSE;
    }

    FillMemory(&dcb, sizeof(dcb), 0);
    dcb.DCBlength = sizeof(dcb);

    if (!GetCommState(port->portHandle, &dcb))
    {
        errorMessage( "Failed to get communications state");
        return FALSE;
    }

    BuildCommDCB((const char *) openString, &dcb);
    dcb.DCBlength = sizeof(dcb);
    dcb.BaudRate = baudRate;
    dcb.fNull = FALSE;
    dcb.fBinary = TRUE;
    dcb.fAbortOnError = FALSE;   

    if (!SetCommState(port->portHandle, &dcb))
    {
        errorMessage( "Failed to set communications state");
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
        if (!ReadFile(port->portHandle, port->rxbuf, port->rx_bufsize, &(port->dwRead), &(port->osReader)))
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
                if (!GetOverlappedResult(port->portHandle, &(port->osReader), &(port->dwRead), FALSE))
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

#ifdef UNIX

/***********************************************************************
*
*    Description:  UNIX rs232 driver
*    Author:       Yury Altshuler
*
*    Copyright:    InterSense 1999 - All rights Reserved.
*
************************************************************************/

#include <sys/types.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/termios.h>
#include <fcntl.h>

#ifdef IRIX
#include <sys/z8530.h> /* SGI only */
#endif

#ifdef SOLARIS
#include <termio.h> 
#endif

#include "serial.h"
#include "isdriver.h"
#include "timer.h"

/****************************************************************************/
int rs232InitCommunications(COMM_PORT *port, const char* comPort, DWORD baudRate)
{
    struct termio terminfo;

    /* close port if one is open */
    rs232DeinitCommunications(port);

    port->rx_bufsize = RX_BUFFER_SIZE;
    port->dwRead = 0;
    port->dwReturned = 0;
    port->portNumber = 0;

    port->desc = open(comPort, O_RDWR);

    if ( port->desc > 0)
    {
        printf("Port %s open. Baud rate %lu\n", comPort, baudRate);
    }
    else
    {
        perror("Open port:");
        return FALSE;
    }

    if( ioctl(port->desc, TCGETA, &terminfo ) == -1 )
    {
        perror( "Configuring Port for InterSense Tracker" );
        return FALSE;
    }

#if defined IRIX
      
    terminfo.c_iflag = 0;
    terminfo.c_oflag = 0;
    terminfo.c_lflag = 0;
    terminfo.c_cc[4] = 0;
    terminfo.c_cc[5] = 5;

#if defined LEGACY_SGI
    terminfo.c_cflag = 
        (baudRate == 9600 ? B9600 : baudRate == 19200 ?  B19200 : B38400)|CS8|CREAD|CLOCAL|CNEW_RTSCTS;
#else  
    terminfo.c_cflag = CS8|CREAD|CLOCAL|CNEW_RTSCTS;
    terminfo.c_ospeed = (baudRate == 9600 ? B9600 :
                        (baudRate == 19200 ?  B19200 :
                        (baudRate == 38400 ?  B38400 : B115200)));                        
    terminfo.c_ispeed = terminfo.c_ospeed;
#endif


#else /* other flavors of UNIX, including Linux */

    terminfo.c_iflag = 0;
    terminfo.c_oflag = 0;
    terminfo.c_lflag = 0;

    terminfo.c_cflag  = (baudRate == 9600L  ? B9600  :
                        (baudRate == 19200L ? B19200 :
                        (baudRate == 38400L ? B38400 : B115200)))|CS8|CREAD|CLOCAL;                       

    terminfo.c_cc[4] = 0;
    terminfo.c_cc[5] = 5;

#endif

    if( ioctl(port->desc, TCSETA, &terminfo ) == -1 )
    {
        perror( "Configuring Port for InterSense Tracker" );
        return FALSE;
    }

#ifdef IRIX
    /* This removes the 50 Hz limit on the serial port
       access on some SGI computers. */

    if( ioctl(port->desc, SIOC_ITIMER, 0 ) == -1 )
    {
        perror( "Setting up real-time serial port access" );
        return FALSE;
    }
#endif

#if defined SOLARIS
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


/****************************************************************************/
int rs232SetSpeed(COMM_PORT *port, DWORD baudRate)
{
    struct termio terminfo;

    if( ioctl(port->desc, TCGETA, &terminfo ) == -1 )
    {
        perror( "Configuring Port for InterSense Tracker" );
        return FALSE;
    }

#if defined IRIX
      
    terminfo.c_iflag = 0;
    terminfo.c_oflag = 0;
    terminfo.c_lflag = 0;
    terminfo.c_cc[4] = 0;
    terminfo.c_cc[5] = 5;

#if defined LEGACY_SGI
    terminfo.c_cflag = 
        (baudRate == 9600 ? B9600 : baudRate == 19200 ?  B19200 : B38400)|CS8|CREAD|CLOCAL|CNEW_RTSCTS; 
#else
    terminfo.c_cflag = CS8|CREAD|CLOCAL|CNEW_RTSCTS;
    terminfo.c_ospeed = (baudRate == 9600 ? B9600 :
                        (baudRate == 19200 ?  B19200 :
                        (baudRate == 38400 ?  B38400 : B115200)));                        
    terminfo.c_ispeed = terminfo.c_ospeed;
#endif


#else /* other flavors of UNIX, including Linux */

    terminfo.c_iflag = 0;
    terminfo.c_oflag = 0;
    terminfo.c_lflag = 0;

    terminfo.c_cflag  = (baudRate == 9600L  ? B9600  :
                        (baudRate == 19200L ? B19200 :
                        (baudRate == 38400L ? B38400 : B115200)))|CS8|CREAD|CLOCAL;                       

    terminfo.c_cc[4] = 0;
    terminfo.c_cc[5] = 5;

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

    return TRUE;
}


/****************************************************************************/
int rs232RxFlush(COMM_PORT *port, WORD numBytes)
{
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

    if(flush || port->dwReturned >= port->rx_bufsize-1)
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













