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

#ifndef _GADGET_DATA_GLOVE_STANDALONE_H_
#define _GADGET_DATA_GLOVE_STANDALONE_H_

#include <gadget/Devices/DriverConfig.h>
#include <string>
#include <vpr/IO/Port/SerialPort.h>
#include <ftd2xx.h>

/**
 * Software interface to wireless noDNA X-IST hardware.
 */
class X_ISTStandalone
{
public:
   /** Default constructor. */
   X_ISTStandalone();

   /** Default destructor. */
   ~X_ISTStandalone();

   /** Connects to the data glove hardware */
   bool connectToHardware(const int gloveNumber);

   unsigned int getNumberSensors() const
   {
      return (mFirmwareVersion > 26 ? 24 : 20);
   }

   /** Reads data from the X-IST hardware  */
   bool readRecordsFromHardware(WORD values[24]);

private:
   FT_HANDLE mComm;
   FTTIMEOUTS mCtmoOld;
   unsigned int mFirmwareVersion;

   void closeGlove();

   bool read(char *buffer, const unsigned long int size);
   bool write(char *buffer, const unsigned long int size);
};


#endif
