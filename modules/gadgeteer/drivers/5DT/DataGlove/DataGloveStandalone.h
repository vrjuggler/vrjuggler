/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2008 by Iowa State University
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

#ifndef _GADGET_DATA_GLOVE_STANDALONE_H_
#define _GADGET_DATA_GLOVE_STANDALONE_H_

#include <gadget/Devices/DriverConfig.h>
#include <string>
#include <vpr/IO/Port/SerialPort.h>


/**
 * Software interface to wireless 5DT Dataglove hardware.
 *
 * Submitted by: Chang Kim
 *
 * @note The 5DT wireless DataGlove is a transmit-only device.
 * @note The glove continuously emits a glove data packet.
 *
 * @date 4-25-02
 */
class DataGloveStandalone
{
public:
   /** Default constructor. */
   DataGloveStandalone();

   /** Default destructor. */
   ~DataGloveStandalone();

   /** Connects to the data glove hardware */
   bool connectToHardware( const std::string& tty_port, int baud_rate );

   /** Reads data from the DataGlove hardware  */
   int ReadRecordsFromHardware(float *ch0, float *ch1, float *ch2, float *ch3,
                               float *ch4, float *ch5, float *ch6, int *ch7,
                               int *ch8 );

protected:

   // Port pointer
   vpr::SerialPort     *mPort;

   int mConnectToHardware( const std::string& tty_port , int baud);

};

#endif
