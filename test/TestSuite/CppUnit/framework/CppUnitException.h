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

#ifndef CPPUNIT_CPPUNITEXCEPTION_H
#define CPPUNIT_CPPUNITEXCEPTION_H

#include <exception>
#include <string>

#define CPPUNIT_UNKNOWNFILENAME        "<unknown>"
#define CPPUNIT_UNKNOWNLINENUMBER      (-1)


class CppUnitException : public std::exception
{
public:
                        CppUnitException (std::string  message    = "", 
                                          long         lineNumber = CPPUNIT_UNKNOWNLINENUMBER, 
                                          std::string  fileName   = CPPUNIT_UNKNOWNFILENAME);
                        CppUnitException (const CppUnitException& other);

    virtual             ~CppUnitException () throw();

    CppUnitException&   operator= (const CppUnitException& other);

    const char          *what() const throw ();

    long                lineNumber ();
    std::string         fileName ();

private:
    std::string         m_message;
    long                m_lineNumber;
    std::string         m_fileName;

};


// Construct the exception
inline CppUnitException::CppUnitException (const CppUnitException& other)
: std::exception (other)
{ 
    m_message       = other.m_message; 
    m_lineNumber    = other.m_lineNumber;
    m_fileName      = other.m_fileName;
} 

inline CppUnitException::CppUnitException (std::string message, long lineNumber, std::string fileName)
: m_message (message), m_lineNumber (lineNumber), m_fileName (fileName)
{}


// Destruct the exception
inline CppUnitException::~CppUnitException () throw()
{}


// Perform an assignment
inline CppUnitException& CppUnitException::operator= (const CppUnitException& other)
{ 
	std::exception::operator= (other);

    if (&other != this) {
        m_message       = other.m_message; 
        m_lineNumber    = other.m_lineNumber;
        m_fileName      = other.m_fileName;
    }

    return *this; 
}


// Return descriptive message
inline const char *CppUnitException::what() const throw ()
{ return m_message.c_str (); }

// The line on which the error occurred
inline long CppUnitException::lineNumber ()
{ return m_lineNumber; }


// The file in which the error occurred
inline std::string CppUnitException::fileName ()
{ return m_fileName; }


#endif
