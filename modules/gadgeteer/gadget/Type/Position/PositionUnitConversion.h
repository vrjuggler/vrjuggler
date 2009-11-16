/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2009 by Iowa State University
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

#ifndef _GADGET_POSITION_UNITS_H_
#define _GADGET_POSITION_UNITS_H_

namespace gadget
{
   /**
    * Constant values for converting to/from meters to the given units.
    * These constants can be used as parameters in the methods that require
    * scale factor values.
    */
   namespace PositionUnitConversion
   {
   const float ConvertToFeet = 3.28f;  /**< Conversion from meters to feet. */
   const float ConvertToInches = 39.37f; /**< Conversion from meters to inches. */
   const float ConvertToMeters = 1.0f;
   const float ConvertToCentimeters = 100.0f;
   }
}

#endif
