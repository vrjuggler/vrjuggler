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

#include <vjConfig.h>

#ifdef VJ_OS_Win32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

#include <Environment/vjSockStream.h>


const size_t sockstreambuf::BUFSIZE = 128;


sockstreambuf::sockstreambuf (int _sock) {
    sock = _sock;
    buf = new char[2*BUFSIZE +2];  //allocate a buffer
    setg (buf, buf, buf);            //set the get area
    setp (buf+BUFSIZE, buf+2*BUFSIZE);          //set the put area
}


sockstreambuf::sockstreambuf () {
    sock = -1;
    buf = new char[2*BUFSIZE +2];  //allocate a buffer
    setg (buf, buf, buf);            //set the get area
    setp (buf+BUFSIZE, buf+2*BUFSIZE);          //set the put area
}



int sockstreambuf::overflow (int c) {
    //std::cout << "overflow: pbase is " << (int)pbase() << " and msg size is " << (int)(pptr()-pbase()) << std::endl;
    
    send (sock, pbase(), pptr()-pbase(), 0);
    setp (buf+BUFSIZE, buf+2*BUFSIZE);          //set the put area
    if (c != EOF)
        sputc(c);
    return 1;
    
}


sockstreambuf::int_type sockstreambuf::underflow () {
    char c;
    //std::cout << "underflow: gptr is " << (int)gptr() << " and egptr is " << (int)egptr() << std::endl;
    if (gptr() < egptr()) {
        // does underflow ever actually get called under this circumstance?
        // there's already a character to read...
        c = *gptr();
        return c;
    }
    else {
        int nread = recv (sock, /*base()*/eback(), BUFSIZE, 0);
        //cout << "nread = " << nread << endl;
        if (nread > 0) {
            setg (eback(), eback(), eback()+nread/*base(), base(), base()+nread*/);
            return *eback()/**base()*/;
        }
        else {
            setg (buf, buf, buf);
            return EOF;
        }
    }
    return EOF;
}


int sockstreambuf::sync() {
    //std::cout << "sync was called!" << std::endl;
    overflow();
    return 0;
    //return streambuf::sync();
}

