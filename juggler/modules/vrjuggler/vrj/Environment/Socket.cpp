/*
 * VRJuggler
 *   Copyright (C) 1997,1998,1999,2000
 *   Iowa State University Research Foundation, Inc.
 *   All Rights Reserved
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
 */


#include <Environment/vjSocket.h>

#ifndef WIN32

/***************************** BSD Sockets Version **************************/

//#include <Environment/sockstream.h>
#include <strings.h>    // For bzero()
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>


const int vjSOCKID_INVALID = -1;


vjSocketPosix::vjSocketPosix() {
    sockid = -1;
    in = 0;
    out = 0;
}



vjSocketPosix::vjSocketPosix(vjSocketIDPosix id) {
    sockid = id;
    out = new ofstream (id);
    in = new ifstream (id);
}



vjSocketPosix::~vjSocketPosix () {
    close();
}



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
#if defined(__FreeBSD__) || defined(__linux__)
    socklen_t len = sizeof (struct sockaddr_in);
#else
    int len = sizeof (struct sockaddr_in);
#endif
    servsock = ::accept (sockid,
                         (sockaddr*)&servaddr, &len);
    if (servsock != -1)
        return new vjSocketPosix (servsock);
    else
        return NULL;
}



#else


/****************************** Winsock2 Version ***************************/

#include <winsock2.h>


const int vjSOCKID_INVALID = -1;


vjSocketWin32::vjSocketWin32() {
    sockid = -1;
    in = 0;
    out = 0;
}



vjSocketWin32::vjSocketWin32(vjSocketIDWin32& id) {
    sockid = id;
    out = new ofstream (id);
    in = new ifstream (id);
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
        cout << "-------- socket - WSAstartup failed-------------" << endl;
        //return false;
        // output the error.
        //std::string error;
        //socketUtil::getLastError( error );
        //cout<<"socketUtil: "<<error<<"\n"<<flush;
    }

    struct sockaddr_in sockaddress;
    //bzero(&sockaddress, sizeof (struct sockaddr_in));
    for (int j = 0; j < sizeof (struct sockaddr_in); j++)
        *((char*)&sockaddress) = 0;
    sockaddress.sin_family = PF_INET;
    sockaddress.sin_port = htons(port);

    sockid = socket (AF_INET, SOCK_STREAM, 0);
    if (sockid == -1) {
        cout << "---------- socket - sockID bad----------" <<endl;
        return false;
    }

    err = bind ( sockid, (sockaddr*)&sockaddress,
                 sizeof (struct sockaddr_in));
    if (err) cout << "--------- socket - bind error " << err << " --------" << endl;
    if (!err)
        err = ::listen (sockid, 0);

    if (err) {
        cout << "--------------- socket - listen fail ------------" << endl;
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



#endif //ifndef WIN32

