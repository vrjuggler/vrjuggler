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


#ifndef _JCCL_SOCKET_H_
#define _JCCL_SOCKET_H_

#include <jccl/jcclConfig.h>
#include <vpr/IO/Socket/Socket.h>


#ifndef VJ_OS_Win32

namespace jccl {

typedef int SocketIDPosix;
extern const int SOCKID_INVALID;

class SocketPosix {

private:

    std::ostream* out;
    std::istream* in;
    SocketIDPosix sockid;

public:

    SocketPosix();
    
    SocketPosix(SocketIDPosix id);

    ~SocketPosix ();

//    std::string getName ();
    int getID () {
        return (int)sockid;
    }

    void close();


    bool listen (int port);


    // must be called after listen
    SocketPosix* accept ();

    inline std::ostream* getOutputStream () {
        return out;
    }

    inline std::istream* getInputStream () {
        return in;
    }

};


typedef SocketPosix Socket;
typedef SocketIDPosix SocketID;

};

#else

/******************************* Winsock2 version ************************/

#include <winsock2.h>

namespace jccl {

typedef SOCKET SocketIDWin32;
extern const int vjSOCKID_INVALID;

class VJ_CLASS_API SocketWin32 {

private:

    std::ostream* out;
    std::istream* in;
    SocketIDWin32 sockid;

public:

    SocketWin32();

    SocketWin32(SocketIDWin32& id);

    ~SocketWin32 ();

	int getID () {
		return (int)sockid;
	}

    void close();

    bool listen (int port);

    // must be called after listen
    SocketWin32* accept ();


    inline std::ostream* getOutputStream () {
        return out;
    }

    inline std::istream* getInputStream () {
        return in;
    }

};


typedef SocketWin32 Socket;
typedef SocketIDWin32 SocketID;

};

#endif //ifndef VJ_OS_Win32

#endif

