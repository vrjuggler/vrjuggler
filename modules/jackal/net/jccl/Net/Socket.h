
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

