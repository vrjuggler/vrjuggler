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

#ifndef CPPUNIT_ESTRING_H
#define CPPUNIT_ESTRING_H


// Create a string from a const char pointer
inline std::string estring (const char *cstring)
{ return std::string (cstring); }

// Create a string from a string (for uniformities' sake)
inline std::string estring (std::string& expandedString)
{ return expandedString; }

// Create a string from an int
inline std::string estring (int number)
{ char buffer [50]; sprintf (buffer, "%d", number); return std::string (buffer); }

// Create a string from a long
inline std::string estring (long number)
{ char buffer [50]; sprintf (buffer, "%ld", number); return std::string (buffer); }

// Create a string from a double
inline std::string estring (double number)
{ char buffer [50]; sprintf (buffer, "%lf", number); return std::string (buffer); }


#endif
