/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998, 1999, 2000 by Iowa State University
 *
 * Original Authors:
 *   Allen Bierbaum, Christopher Just,
 *   Patrick Hartling, Kevin Meinert,
 *   Carolina Cruz-Neira, Albert Baker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile$
 * Date modified: $Date$
 * Version:       $Revision$
 * -----------------------------------------------------------------
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#define DOS     /* Dos OS */

#include <io.h>                    /* for read,write,open */
#include <fcntl.h>
#include <graph.h>                 /* for CLEARSCREEN macro */
#include <dos.h>

#define ENABLE() _enable()
#define DISABLE() _disable()
#define CLEARSCREEN _clearscreen(_GCLEARSCREEN)
#define OUTPORTB outp
#define INPORTB inp
#define GETVECT(intnum) _dos_getvect(intnum)
#define SETVECT(intnum, intserviceroutine) _dos_setvect(intnum, intserviceroutine)
#define SETTEXTPOS(row,col) _settextposition(row,col)
#define INTTYPE void interrupt far

#endif

/*************************************************************************/
#ifdef TC       /* Boland Turbo C/C++ */

#define DOS    /* Dos OS */

#include <conio.h>                 /* for getch,putch,kbhit */
#include <io.h>                    /* for read,write,open */
#include <fcntl.h>
#include <dos.h>

#define ENABLE() enable()
#define DISABLE() disable()
#define CLEARSCREEN clrscr()
#define OUTPORTB outportb           /* should compile to inline assembly */
#define INPORTB inportb             /* should compile to inline assembly */
#define GETVECT(intnum) getvect(intnum)
#define SETVECT(intnum, intserviceroutine) setvect(intnum, intserviceroutine)
#define SETTEXTPOS(row,col) gotoxy(row,col)
#define INTTYPE void interrupt far

#endif

#ifdef HIGHC

#define DOS    /* Dos OS */

#define DPMC   /* Dos Protected Mode Compiler */

#include <conio.h>                 /* for getch,putch,kbhit */
#include <io.h>                    /* for read,write,open */
#include <fcntl.h>
#include <graph.h>                 /* for CLEARSCREEN macro */

#include <dos.h>

#define OUTPORTB _outp
#define INPORTB _inp
#define CLEARSCREEN _clearscreen(_GCLEARSCREEN)
#define SETTEXTPOS(row,col) _settextposition(row,col)
#define GETVECT(intnum) _getrvect(intnum)
#define GETPVECT(intnum) _getpvect(intnum)
#define SETVECT(intnum, intserviceroutine) _setrvect(intnum, intserviceroutine)
#define SETPVECT(intnum, intserviceroutine) _setpvect(intnum, intserviceroutine)
#define SETRPVECT(intnum, intserviceroutine) _setrpvectp(intnum, intserviceroutine)
#define ENABLE() _enable()
#define DISABLE() _disable()
#define INTTYPE _Far _INTERRPT void
#define RINTTYPE _real_int_handler_t

#endif


/*************************************************************************/
#ifdef KNR      /* Standard C Compilers */

#define CLEARSCREEN clearconsole()

#define SETTEXTPOS(row,col) printf("\n\n\r")

 /*
	IBM PC/Compatible - Mark Williams COHERENT version 4.0
 */
#ifdef COHERENT
#define UNIX                      /* UNIX type OS */
#define UNIX_SGTTY                /* TTY IOCTL Calls use SGTTY.h */
#include <v7sgtty.h>              /* needed for TIOCQUERY and TIOCFLUSH */
#include <fcntl.h>
#define TIONREAD TIOCQUERY        /* re map the IOCTL function */
#define BAUDRATE_115200 -1
#define BAUDRATE_57600  -1
#define BAUDRATE_38400  B38400
#endif

 /*
	IBM RISC 6000 - AIX Release 3.2 
 */
#ifdef AIX
#define UNIX                      /* UNIX type OS */
#define UNIX_TERMIO               /* TTY IOCTL Calls use TERMIO.h */
#include <fcntl.h>
#define TIONREAD FIONREAD         /* re map the IOCTL function */
#define BAUDRATE_115200 -1
#define BAUDRATE_57600  -1
#define BAUDRATE_38400  B38400
#endif

 /*
	DEC ULTRIX
 */
#ifdef ULTRIX
#define UNIX                      /* UNIX type OS */
#define UNIX_TERMIO               /* TTY IOCTL Calls use TERMIO.h */
#define TIONREAD FIONREAD         /* re map the IOCTL function */
#define B19200 EXTA               /* EXTA = 19200 baud */
#define BAUDRATE_115200 -1
#define BAUDRATE_57600  -1
#define BAUDRATE_38400  -1
#endif

 /*
	SGI - IRIX Release 4.0 System V.3
 */
#ifdef IRIX
#define UNIX                      /* UNIX type OS */
#define UNIX_TERMIO               /* UNIX-TERMIO type OS */
#include<sys/ioctl.h>
#include<sys/fcntl.h>
#define TIONREAD FIONREAD         /* re map the IOCTL function */
#define BAUDRATE_115200 -1
#define BAUDRATE_57600  -1
#define BAUDRATE_38400  B38400
#endif
 
 /*
	SUN - SUNOS Release 4.1 
 */
#ifdef SUNOS
#define UNIX                      /* UNIX type OS */
#define UNIX_TERMIO               /* UNIX-TERMIO type OS */
#include<sys/filio.h>             /* for FIONREAD */
#include<sys/ttold.h>             /* for TIOCFLUSH */
#include<sys/fcntlcom.h>
#include<fcntl.h>                  
#define TIONREAD FIONREAD         /* re map the IOCTL function */
#define BAUDRATE_115200 -1
#define BAUDRATE_57600  -1
#define BAUDRATE_38400  B38400
#endif


 /*
	UNIX OS's
 */
#ifdef UNIX_SGTTY
#include <sgtty.h>                /* for SGTTY ioctl calls */
#endif

#ifdef UNIX_TERMIO
#include <termio.h>               /* for TERMIO ioctl calls */
#endif

#endif

#endif  /* COMPILER */
