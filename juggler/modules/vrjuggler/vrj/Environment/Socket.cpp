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

#include <Environment/vjSocket.h>

#ifndef VJ_OS_Win32

/***************************** BSD Sockets Version **************************/

//#include <Environment/sockstream.h>
#include <strings.h>    // For bzero()
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <Environment/vjSockStream.h>

const int vjSOCKID_INVALID = -1;


vjSocketPosix::vjSocketPosix() {
    sockid = -1;
    in = 0;
    out = 0;
}



vjSocketPosix::vjSocketPosix(vjSocketIDPosix id) {
    sockid = id;
    out = new std::ostream (new sockstreambuf (id));
    in = new std::istream (new sockstreambuf(id));
}



vjSocketPosix::~vjSocketPosix () {
    close();
}



//  std::string vjSocketPosix::getName () {
//      std::string foo = sockid;
//      return foo;
//  }



void vjSocketPosix::close() {
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



bool vjSocketPosix::listen (int port) {
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
vjSocketPosix* vjSocketPosix::accept () {
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
        return new vjSocketPosix (servsock);
    else
        return NULL;
}



#else


/****************************** Winsock2 Version ***************************/

#include <winsock2.h>
#include <Environment/vjSockStream.h>


const int vjSOCKID_INVALID = -1;


vjSocketWin32::vjSocketWin32() {
    sockid = -1;
    in = 0;
    out = 0;
}



vjSocketWin32::vjSocketWin32(vjSocketIDWin32& id) {
    sockid = id;
//     sockstreambuf* buf = new sockstreambuf (id, new char[512], 512);
//     out = new ofstream (buf);
//     in = new ifstream (buf);
    out = new std::ostream (new sockstreambuf (id));
    in = new std::istream (new sockstreambuf(id));
}



vjSocketWin32::~vjSocketWin32 () {
    close();
}


void vjSocketWin32::close() {
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



bool vjSocketWin32::listen (int port) {
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
vjSocketWin32* vjSocketWin32::accept () {
    sockaddr_in servaddr;
    unsigned int servsock;
    int len = sizeof (struct sockaddr_in);
    servsock = ::accept (sockid,
                         (sockaddr*)&servaddr, &len);
    if (servsock != -1)
        return new vjSocketWin32 (servsock);
    else
        return NULL;
}



#endif //ifndef VJ_OS_Win32

