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

#ifndef _GADGET_ASCENSION_MOTION_STAR_EXCEPTIONS_H_
#define _GADGET_ASCENSION_MOTION_STAR_EXCEPTIONS_H_

#include <vpr/Util/Exception.h>


namespace mstar
{

/**
 * Basic exception that may be thrown by the standalone MotionStar driver.
 */
class MotionStarException : public vpr::Exception
{
public:
   MotionStarException(const std::string& msg,
                       const std::string& location = "")
      : vpr::Exception(msg, location)
   {
      /* Do nothing. */ ;
   }

   virtual ~MotionStarException() throw ()
   {
      /* Do nothing. */ ;
   }
};

/**
 * A generic network-related exception.
 */
class NetworkException : public MotionStarException
{
public:
   NetworkException(const std::string& msg = "",
                    const std::string& location = "")
      : MotionStarException(msg, location)
   {
      /* Do nothing. */ ;
   }

   virtual ~NetworkException() throw ()
   {
      /* Do nothing. */ ;
   }
};

/**
 * A generic network exception caused by trying to read from a socket.
 */
class NetworkReadException : public NetworkException
{
public:
   NetworkReadException(const std::string& msg = "",
                        const std::string& location = "")
      : NetworkException(msg, location)
   {
      /* Do nothing. */ ;
   }

   virtual ~NetworkReadException() throw ()
   {
      /* Do nothing. */ ;
   }
};

/**
 * No data was read from the socket.
 */
class NoDataReadException : public NetworkReadException
{
public:
   NoDataReadException(const std::string& msg = "",
                       const std::string& location = "")
      : NetworkReadException(msg, location)
   {
      /* Do nothing. */ ;
   }

   virtual ~NoDataReadException() throw ()
   {
      /* Do nothing. */ ;
   }
};

/**
 * A generic network exception caused by trying to write to a socket.
 */
class NetworkWriteException : public NetworkException
{
public:
   NetworkWriteException(const std::string& msg = "",
                         const std::string& location = "")
      : NetworkException(msg, location)
   {
      /* Do nothing. */ ;
   }

   virtual ~NetworkWriteException() throw ()
   {
      /* Do nothing. */ ;
   }
};

/**
 * No data was written to the socket.
 */
class NoDataWrittenException : public NetworkWriteException
{
public:
   NoDataWrittenException(const std::string& msg = "",
                          const std::string& location = "")
      : NetworkWriteException(msg, location)
   {
      /* Do nothing. */ ;
   }

   virtual ~NoDataWrittenException() throw ()
   {
      /* Do nothing. */ ;
   }
};

/**
 * A socket connection exception.
 */
class ConnectException : public NetworkException
{
public:
   ConnectException(const std::string& msg = "",
                    const std::string& location = "")
      : NetworkException(msg, location)
   {
      /* Do nothing. */ ;
   }

   virtual ~ConnectException() throw ()
   {
      /* Do nothing. */ ;
   }
};

/**
 * An exception uesd when no status information could be read from a device
 * on the Fast Bird Bus (FBB).
 */
class NoDeviceStatusException : public MotionStarException
{
public:
   /**
    * @param deviceNum The device address on the FBB.
    *        msg       An optional message to associate with this exception.
    */
   NoDeviceStatusException(const unsigned int deviceNum,
                           const std::string& msg = "",
                           const std::string& location = "")
      : MotionStarException(msg, location)
      , mDevNum(deviceNum)
   {
      /* Do nothing. */ ;
   }

   virtual ~NoDeviceStatusException() throw ()
   {
      /* Do nothing. */ ;
   }

   unsigned int mDevNum;
};

/**
 * An exception used when something went wrong in trying to send a network
 * command to the MotionStar chassis.
 */
class CommandException : public MotionStarException
{
public:
   CommandException(const std::string& msg = "",
                    const std::string& location = "")
      : MotionStarException(msg, location)
   {
      /* Do nothing. */ ;
   }

   virtual ~CommandException() throw ()
   {
      /* Do nothing. */ ;
   }
};

/**
 * An exception used when data read from the MotionStar is unexcepted or
 * otherwise invalid.
 */
class DataException : public MotionStarException
{
public:
   DataException(const std::string& msg = "", const std::string& location = "")
      : MotionStarException(msg, location)
   {
      /* Do nothing. */ ;
   }

   virtual ~DataException() throw ()
   {
      /* Do nothing. */ ;
   }
};

/**
 * An exception used when the scaling factor being used by the MotionStar is
 * unknown.  A special exception is created for this case because the
 * MotionStar driver depends on the MotionStar hardware to tell it what
 * scaling factor to use when reading data.  Without this information, the
 * data being read from the device is useless.
 */
class ScaleFactorUnknownException : public DataException
{
public:
   ScaleFactorUnknownException(const std::string& msg = "",
                               const std::string& location = "")
      : DataException(msg, location)
   {
      /* Do nothing. */ ;
   }

   virtual ~ScaleFactorUnknownException() throw ()
   {
      /* Do nothing. */ ;
   }
};

} // End of mstar namespace


#endif /* _GADGET_ASCENSION_MOTION_STAR_EXCEPTIONS_H_ */
