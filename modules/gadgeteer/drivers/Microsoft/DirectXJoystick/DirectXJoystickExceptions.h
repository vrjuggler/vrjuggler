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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 *************** <auto-copyright.pl END do not edit this line> ***************/

#ifndef _GADGET_DIRECTX_JOYSTICK_EXCEPTIONS_H_
#define _GADGET_DIRECTX_JOYSTICK_EXCEPTIONS_H_

#include <exception>
#include <string>


namespace gadget
{

class DirectXJoystickException : public std::exception
{
public:
   DirectXJoystickException(const std::string& msg)
      : std::exception()
      , mMsg(msg)
   {
      ;
   }

   virtual ~DirectXJoystickException() throw()
   {
      ;
   }

   virtual const char* what() const throw()
   {
      return mMsg.c_str();
   }

protected:
   std::string mMsg;
};

/**
 * Exception type thrown when an attempt to query the Direct Input device
 * object fails.
 */
class DirectXJoystickQueryException : public DirectXJoystickException
{
public:
   DirectXJoystickQueryException(const std::string& msg)
      : DirectXJoystickException(msg)
   {
      ;
   }

   virtual ~DirectXJoystickQueryException() throw()
   {
      ;
   }
};

class DirectXJoystickDataFormatException : public DirectXJoystickException
{
public:
   DirectXJoystickDataFormatException(const std::string& msg)
      : DirectXJoystickException(msg)
   {
      ;
   }

   virtual ~DirectXJoystickDataFormatException() throw()
   {
      ;
   }
};

class DirectXJoystickCooperationException : public DirectXJoystickException
{
public:
   DirectXJoystickCooperationException(const std::string& msg)
      : DirectXJoystickException(msg)
   {
      ;
   }

   virtual ~DirectXJoystickCooperationException() throw()
   {
      ;
   }
};

class DirectXJoystickAccessException : public DirectXJoystickException
{
public:
   DirectXJoystickAccessException(const std::string& msg)
      : DirectXJoystickException(msg)
   {
      ;
   }

   virtual ~DirectXJoystickAccessException() throw()
   {
      ;
   }
};

}


#endif /* _GADGET_DIRECTX_JOYSTICK_EXCEPTIONS_H_ */
