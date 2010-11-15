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

#include "socketUtil.h"

void main(int argc, char** argv)
{
    std::string address = "129.186.232.64";
    int port = 4550;
    
    if (argc == 2)
	address = argv[1];
    
    if (argc == 3)
	port = atoi(argv[2]);
    //-------------------------------------------
    
    std::string words[4] = {"Kevin", "is", "really", "cool"};
    socketUtil sUtil;
    
    sUtil.setRole( socketUtil::SERVER );
    sUtil.setRemote(address, port);
    

	
	sUtil.connect();
	sleep(1);
	for (int i = 0; i < 50000; ++i)
	for (int x = 0; x < 4; ++x)
	{
	    socketUtil::Message message;
	    message.messageType = socketUtil::STRING;
	    message.sizeOfData = 69;
	    message.data = words[x];
	    
	    sUtil.enqueue( message );
	}
	
	
	sUtil.sendQueue();
	
	sleep(5);
	
	sUtil.disconnect();

}
