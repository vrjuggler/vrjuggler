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
#define INT_CNTRL_OCW2 0x20
#define INT_CNTRL_OCW3 0x20
#define INT_MSK_REG INT_CNTRL_OCW1  /* mask register address */
#define INT_EOI 0x20               /* end of interrupt command */
#define INT_RD_IRR 0x0a             /* read Interrupt request register */
#define INT_RD_ISR 0x0b             /* read Interrupt service register */

/*
    Interrupt Register (8259) definitions
*/
#define IRQ0            0x01    /* bit pos for IRQ 0 */
#define IRQ1            0x02    /* bit pos for IRQ 1 */
#define IRQ2            0x04    /* bit pos for IRQ 2 */
#define IRQ3            0x08    /* bit pos for IRQ 3 */
#define IRQ4            0x10    /* bit pos for IRQ 4 */
#define IRQ5            0x20    /* bit pos for IRQ 5 */
#define IRQ6            0x40    /* bit pos for IRQ 6 */
#define IRQ7            0x80    /* bit pos for IRQ 7 */

/*
    Interrupt Vector Numbers
*/
#define IRQ0_VEC        0x08    /* Master PIC starting Vector # */
#define IRQ1_VEC        IRQ0_VEC+1
#define IRQ2_VEC        IRQ0_VEC+2
#define IRQ3_VEC        IRQ0_VEC+3
#define IRQ4_VEC        IRQ0_VEC+4
#define IRQ5_VEC        IRQ0_VEC+5
#define IRQ6_VEC        IRQ0_VEC+6
#define IRQ7_VEC        IRQ0_VEC+7


extern unsigned char oldirqmsk;

#define PCPIC_MASK(maskval) OUTPORTB(INT_MSK_REG, INPORTB(INT_MSK_REG) | maskval)
#define PCPIC_UNMASK(maskval) OUTPORTB(INT_MSK_REG, INPORTB(INT_MSK_REG) & ~maskval)
#define PCPIC_SAVEMASK INPORTB(INT_MSK_REG)
#define PCPIC_RESTOREMASK(maskval) OUTPORTB(INT_MSK_REG, maskval)
#define PCPIC_OUTEOI OUTPORTB(INT_CNTRL_OCW2,INT_EOI)


#endif /* PCPIC */

#endif /* DOS */



