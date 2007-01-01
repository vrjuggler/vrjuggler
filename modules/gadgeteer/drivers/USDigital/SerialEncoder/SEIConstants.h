/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2007 by Iowa State University
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
 *************** <auto-copyright.pl END do not edit this line> ***************/

/* Model numbers */
#define M1MODEL		1			/* SEI-M1 model number 		*/
#define A2MODEL		2			/* A2 model number			*/
#define ED2MODEL    3           /* ED2 model number         */
#define AD5MODEL    5           /* AD5 model number         */

/* SEI command numbers */
#define RDSN		0x03		/* read serial number 		*/
#define CHKSN		0x04		/* check serial number 		*/
#define FAILSN		0x05		/* fail serial number 		*/
#define RADDR		0x06		/* read address 			*/
#define AADDR		0x07		/* assign address 			*/
#define FINFO		0x08		/* factory info request 	*/
#define RSET		0x0E		/* software reset 			*/
#define BAUDR		0x0F		/* change baud rate 		*/
#define LOOPB		0x10		/* enter loopback mode 		*/
#define OFFLIN		0x11		/* enter offline mode 		*/
#define WAKEUP		0x6F		/* wakeup command 			*/
#define SLEEP		0x5F		/* sleep command 			*/

/* A2 encoder command numbers */
#define STROBE		0x40		/* strobe command 			*/
#define ORIG		0x01		/* set origin command 		*/
#define SETPOS		0x02		/* set abs position  		*/
#define RDRES		0x09		/* read resolution 			*/
#define CHRES		0x0A		/* change resolution 		*/
#define RDMOD		0x0B		/* read mode 				*/
#define CHMOD		0x0C		/* change mode 				*/
#define CHDMOD		0x0D		/* change default mode 		*/
#define	STOPCLKS	0x12		/* stop clocks, hibernate	*/

/* SEI-M1 command numbers */
#define WROUT		0x09		/* write outputs 			*/
#define WRDOUT		0x0A		/* write default outputs	*/
#define WRMOT		0x0B		/* write motor 				*/
#define WRDMOT		0x0C		/* write default motor 		*/          

/* ED2 command numbers */
#define ED2ORIG		0x01		/* set origin command 		*/
#define ED2SETPOS1	0x02		/* set abs position  		*/
#define ED2SETPOS2	0x12		/* set abs position  		*/
#define ED2RDRES	0x09		/* read resolution 			*/
#define ED2CHRES	0x0A		/* change resolution 		*/
#define ED2RDMOD	0x0B		/* read mode 				*/
#define ED2CHMOD	0x0C		/* change mode 				*/
#define ED2CHDMOD	0x0D		/* change default mode 		*/

#define ED2RDPOS1   0x10        /* single byte read position        */
#define ED2RDPOS2   0x20        /* single byte read position        */
#define ED2RDDIG    0x70        /* single byte read digital inputs  */
#define ED2WROUT	0x19		/* write outputs 			*/
#define ED2WRDOUT	0x0A		/* write default outputs	*/

/* AD5 command numbers */
#define AD5RDMOD	0x0B		/* read mode 				*/
#define AD5CHMOD	0x0C		/* change mode 				*/
#define AD5CHDMOD	0x0D		/* change default mode 		*/

#define AD5RESET0	0xB0		/* single byte Reset Port 1	*/
#define AD5RESET1	0xC0		/* single byte Reset Port 2	*/
#define AD5RESET2	0xD0		/* single byte Reset Port 3	*/
#define AD5RESET3	0xE0		/* single byte Reset Port 4	*/

#define AD5SETPOS0	0x02		/* set position Port 1 		*/
#define AD5SETPOS1	0x12		/* set position Port 2 		*/
#define AD5SETPOS2	0x13		/* set position Port 3 		*/
#define AD5SETPOS3	0x14		/* set position Port 4 		*/

#define AD5RDRES0	0x21		/* read resolution Port 1	*/
#define AD5RDRES1	0x22		/* read resolution Port 2	*/
#define AD5RDRES2	0x23		/* read resolution Port 3	*/
#define AD5RDRES3	0x24		/* read resolution Port 4	*/

#define AD5CHRES0	0x31		/* change resolution Port 1	*/
#define AD5CHRES1	0x32		/* change resolution Port 2	*/
#define AD5CHRES2	0x33		/* change resolution Port 3	*/
#define AD5CHRES3	0x34		/* change resolution Port 4	*/

#define AD5RDPOS0   0x10        /* single byte read position Port 1 */
#define AD5RDPOS1   0x20        /* single byte read position Port 2 */
#define AD5RDPOS2   0x30        /* single byte read position Port 3 */
#define AD5RDPOS3   0x40        /* single byte read position Port 4 */
#define AD5RDALL    0x50        /* single byte read position ALL PORTS */

#define AD5CHCMR0   0x41        /* set CMR Port 1           */
#define AD5CHCMR1   0x42        /* set CMR Port 2           */
#define AD5CHCMR2   0x43        /* set CMR Port 3           */
#define AD5CHCMR3   0x44        /* set CMR Port 4           */

#define AD5RDCMR0	0x45		/* read CMR Port 1			*/
#define AD5RDCMR1	0x46		/* read CMR Port 2			*/
#define AD5RDCMR2	0x47		/* read CMR Port 3			*/
#define AD5RDCMR3	0x48		/* read CMR Port 4			*/
