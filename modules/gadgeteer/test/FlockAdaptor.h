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

#ifndef _GADGET_FLOCK_ADAPTOR_H_
#define _GADGET_FLOCK_ADAPTOR_H_

//#include <gadget/gadgetConfig.h>
#include <string>
//#include <vpr/IO/Port/SerialPort.h>
#include <gadget/Devices/Ascension/FlockStandalone.h>
#include <BaseAdaptor.h>

namespace gadget_test
{
class FlockAdaptor : public BaseAdaptor
{
public:
   /// Default constructor.
   FlockAdaptor(const std::string& devName = "Flock of Birds");
   ~FlockAdaptor();

   /** Connects to the pinch glove hardware. */
	bool Start( const std::string& ttyPort, int mBaudRate );
	bool Start();
	bool Stop();
	bool Sample( int numSamples );
	bool Sample();
	bool Dump();
	void printCommands();
	void Command(char ch);
private:
   FlockStandalone* mFlock;
   std::vector<int> button;    // size of 4
   std::vector<float>  analog;    // size of 4
};
}
#endif
