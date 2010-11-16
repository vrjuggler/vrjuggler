/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2010 by Iowa State University
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
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef MACROS_AND_DEFINES_FOR_GLOVE_APP
#define MACROS_AND_DEFINES_FOR_GLOVE_APP

// Macros and defines:
namespace gloveDefined
{
    //: MAX macro
    //  returns - the maximum of a, b, and c.
    template<class T> 
    inline const T& nMax( const T& a, const T& b, const T& c )
	     { return (a>=b) ? ((a>=c)?a:c) : ((b>=c)?b:c); }
    
    //: MIN macro
    //  returns - the minimum of a, b, and c.
    template<class T> 
    inline const T& nMin( const T& a, const T& b, const T& c )
	     { return (a<=b) ? ((a<=c)?a:c) : ((b<=c)?b:c); }
    
    // Converts a number in radians to a number in degrees
    const double    TO_DEG_D         = 57.2957795131;
    
    // Converts a number in radians to a number in degrees
    const float     TO_DEG_F         = 57.2957795131f;
    
    // Converts a number in degrees to a number in radians
    const double    TO_RAD_D         = 0.01745329252;
    
    // Converts a number in degrees to a number in radians
    const float     TO_RAD_F         = 0.01745329252f;
};

using namespace gloveDefined;

#endif
