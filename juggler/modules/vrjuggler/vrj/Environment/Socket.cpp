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

#include <vrj/vjConfig.h>

#include <vrj/Environment/Socket.h>

#ifndef VJ_OS_Win32

/***************************** BSD Sockets Version **************************/

//#include <Environment/sockstream.h>
#include <strings.h>    // For bzero()
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <vrj/Environment/SockStream.h>

namespace vrj
{
   
const int SOCKID_INVALID = -1;


SocketPosix::SocketPosix() {
    sockid = -1;
    in = 0;
    out = 0;
}



SocketPosix::SocketPosix(SocketIDPosix id) {
    sockid = id;
    out = new std::ostream (new sockstreambuf (id));
    in = new std::istream (new sockstreambuf(id));
}



SocketPosix::~SocketPosix () {
    close();
}



//  std::string SocketPosix::getName () {
//      std::string foo = sockid;
//      return foo;
//  }



void SocketPosix::close() {
    if (in) {
        delete in;
        in = 0;
    }
    if (out) {
        delete out;
        out = 0;
    }
    if (sockid != -1) {
        ::close (sockid);
        sockid = -1;
    }
}



bool SocketPosix::listen (int port) {
    /* here, we open a socket & get ready to read connections */
    struct sockaddr_in sockaddress;
    bzero(&sockaddress, sizeof (struct sockaddr_in));
    sockaddress.sin_family = PF_INET;
    sockaddress.sin_port = htons(port);

    sockid = socket (AF_INET, SOCK_STREAM, 0);
    if (sockid == -1)
        return false;

    int err = bind ( sockid, (sockaddr*)&sockaddress,
                     sizeof (struct sockaddr_in));
    if (!err)
        err = ::listen (sockid, 0);
    
    if (err) {
        close();
        return false;
    }
    else
        return true;
}


// must be called after listen
SocketPosix* SocketPosix::accept () {
    sockaddr_in servaddr;
    int servsock;


#ifdef VJ_OS_HPUX
    int len;
#else
    socklen_t len;
#endif

    len = sizeof (struct sockaddr_in);
    servsock = ::accept (sockid, (sockaddr*)&servaddr, &len);

    if (servsock != -1)
        return new SocketPosix (servsock);
    else
        return NULL;
}


} // end namespace

#else


/****************************** Winsock2 Version ***************************/

#include <winsock2.h>
#include <vrj/Environment/SockStream.h>

namespace vrj
{
         
const int SOCKID_INVALID = -1;


SocketWin32::SocketWin32() {
    sockid = -1;
    in = 0;
    out = 0;
}



SocketWin32::SocketWin32(SocketIDWin32& id) {
    sockid = id;
//     sockstreambuf* buf = new sockstreambuf (id, new char[512], 512);
//     out = new ofstream (buf);
//     in = new ifstream (buf);
    out = new std::ostream (new sockstreambuf (id));
    in = new std::istream (new sockstreambuf(id));
}



SocketWin32::~SocketWin32 () {
    close();
}


void SocketWin32::close() {
    if (in) {
        delete in;
        in = 0;
    }
    if (out) {
        delete out;
        out = 0;
    }
    if (sockid != -1) {
        ::closesocket (sockid);
        sockid = -1;
    }
}



bool SocketWin32::listen (int port) {
    /* here, we open a socket & get ready to read connections */
    WSADATA wsaData;
    unsigned short wVersionRequested = (0x1<<8) | 0x1;
    int err;

    err = WSAStartup( wVersionRequested, &wsaData ); 
    if(err < 0) {
        std::cout << "-------- socket - WSAstartup failed-------------"
                  << std::endl;
        //return false;
        // output the error.
        //std::string error;
        //socketUtil::getLastError( error );
        //std::cout<<"socketUtil: "<<error<<"\n"<<std::flush;
    }

    u_long nInterfaceAddr = inet_addr("0.0.0.0");
    struct sockaddr_in sockaddress;
//      for (int j = 0; j < sizeof (struct sockaddr_in); j++)
//          *((char*)&sockaddress) = 0;
    sockaddress.sin_addr.s_addr = nInterfaceAddr;
    sockaddress.sin_family = PF_INET;
    sockaddress.sin_port = htons(port);

    sockid = socket (AF_INET, SOCK_STREAM, 0);
    if (sockid == -1) {
        std::cout << "---------- socket - sockID bad----------" << std::endl;
        return false;
    }

    err = bind ( sockid, (sockaddr*)&sockaddress,
                 sizeof (struct sockaddr_in));
    if (err)
        std::cout << "--------- socket - bind error " << err << " --------"
                  << std::endl;
    if (!err)
        err = ::listen (sockid, 0);

    if (err) {
        std::cout << "--------------- socket - listen fail ------------"
                  << std::endl;
        close();
        return false;
    }
    else
        return true;
}



// must be called after listen
SocketWin32* SocketWin32::accept () {
    sockaddr_in servaddr;
    unsigned int servsock;
    int len = sizeof (struct sockaddr_in);
    servsock = ::accept (sockid,
                         (sockaddr*)&servaddr, &len);
    if (servsock != -1)
        return new SocketWin32 (servsock);
    else
        return NULL;
}



} // end namespace

#endif //ifndef VJ_OS_Win32

