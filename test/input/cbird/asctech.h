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
#define FTW (float)32768.0                          /* float to word integer */
#define WTF (float)(1.0/32768.0)                    /* float to word integer */
#define ANGK (float)(180.0/32768.0)                 /* integer to degrees */
#define POSK36 (float)(36.0/32768.0)                /* integer to inches */
#define POSK72 (float)(72.0/32768.0)                /* integer to inches */
#define POSK144 (float)(144.0/32768.0)              /* integer to inches ER Controller */

/*
    Output Mode defintions
*/
#define POINT 0
#define CONTINUOUS 1
#define STREAM  2

/*
    Data Mode Definitions
*/
#define POS         1
#define ANGLE       2
#define MATRIX      3
#define QUATER      4
#define POSANGLE    5
#define POSMATRIX   6
#define POSQUATER   7

/*
    Bird Timing definitions
*/
#define DEFAULTCRYSTALFREQ (float) 20.00        /* CPU Clock Frequency */
#define DEFAULTCLOCKVALUE (float) 0.40          /* 8/DEFAULTCRYSTALFREQ */


#endif /* ASCTECH */


