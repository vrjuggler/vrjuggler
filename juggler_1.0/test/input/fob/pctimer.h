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
#define PCTIMER_CTLREG 0x43     /* Programmable Timer Control Reg */
#define PCTIMER_TIMER0 0x40     /* Programmable Timer 0 Reg */

/*
    Interrupt Controller (8259) Definitions
*/
#define INT_CNTRL_OCW1 0x21       /* operational control addresses */
#define INT_CNTRL_OCW2 0x20
#define INT_CNTRL_OCW3 0x20
#define INT_MSK_REG INT_CNTRL_OCW1  /* mask register address */
#define INT_EOI 0x20               /* end of interrupt command */
#define INT_RD_IRR 0x0a             /* read Interrupt request register */
#define INT_RD_ISR 0x0b             /* read Interrupt service register */

extern unsigned long numirqticks;
extern unsigned char irq0_flg;
extern unsigned char pctimerstored_flg;

#define GETTICKS numirqticks
#define TICK_MSECS 50

/*
    Prototypes
*/
short pctimer_init(short millisecs);
void pctimer_restore(void);

#endif /* PCTIMER */

#endif /* DOS */



