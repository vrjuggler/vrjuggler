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


#ifndef _VJ_SOCKET_H_
#define _VJ_SOCKET_H_

#include <vjConfig.h>

#ifndef WIN32


typedef int vjSocketIDPosix;
extern const int vjSOCKID_INVALID;

class vjSocketPosix {

private:

    ostream* out;
    istream* in;
    vjSocketIDPosix sockid;

public:

    vjSocketPosix();
    
    vjSocketPosix(vjSocketIDPosix id);

    ~vjSocketPosix ();

//    std::string getName ();
    int getID () {
        return (int)sockid;
    }

    void close();


    bool listen (int port);


    // must be called after listen
    vjSocketPosix* accept ();

    inline ostream* getOutputStream () {
        return out;
    }

    inline istream* getInputStream () {
        return in;
    }

};


typedef vjSocketPosix vjSocket;
typedef vjSocketIDPosix vjSocketID;


#else

/******************************* Winsock2 version ************************/

#include <winsock2.h>
//#include <Environment/sockstream.h>

typedef SOCKET vjSocketIDWin32;
extern const int vjSOCKID_INVALID;

class vjSocketWin32 {

private:

    ostream* out;
    istream* in;
    vjSocketIDWin32 sockid;

public:

    vjSocketWin32();

    vjSocketWin32(vjSocketIDWin32& id);

    ~vjSocketWin32 ();

	int getID () {
		return (int)sockid;
	}

    void close();

    bool listen (int port);

    // must be called after listen
    vjSocketWin32* accept ();


    inline ostream* getOutputStream () {
        return out;
    }

    inline istream* getInputStream () {
        return in;
    }

};


typedef vjSocketWin32 vjSocket;
typedef vjSocketIDWin32 vjSocketID;


#endif //ifndef WIN32

#endif

