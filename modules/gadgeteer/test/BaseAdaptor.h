/*************** <auto-copyright.pl BEGIN do not edit this line> **************
 *
 * VR Juggler is (C) Copyright 1998-2003 by Iowa State University
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

#ifndef _GADGET_BASE_ADAPTOR_H_
#define _GADGET_BASE_ADAPTOR_H_

//#include <gadget/gadgetConfig.h>
#include <string>
//#include <vpr/IO/Port/SerialPort.h>

/**
 * Software interface to Fakespace Pinchglove hardware.
 *
 *
 * @note The long function names greatly decrease the
 *       ambiguity of what the functions do ... sorry. :)
 *
 * @date 1-23-99
 */

namespace gadget_test
{
class BaseAdaptor
{
public:
    // Default constructor.
	BaseAdaptor(const std::string& devName = "Device")
	{
	}
    virtual ~BaseAdaptor();
    
	virtual bool Start(const std::string& ttyPort, int mBaudRate)=0;
	virtual bool Start()=0;
	virtual bool Stop()=0;
	virtual bool Sample( int numSamples )=0;
	virtual bool Sample()=0;
	virtual bool Dump()=0;
	virtual void printCommands()=0;
	virtual void Command(char ch)=0;		
	char spiner();
	std::string mName;

private:
};
}
#endif
