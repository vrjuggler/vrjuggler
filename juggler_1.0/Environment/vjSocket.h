
#ifndef _VJ_SOCKET_H_
#define _VJ_SOCKET_H_

#ifndef WIN32

//#include <Environment/sockstream.h>
#include <strings.h>    // For bzero()
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>


typedef int vjSocketIDPosix;
const int vjSOCKID_INVALID = -1;

class vjSocketPosix {

public:

    ostream* out;
    istream* in;
    vjSocketIDPosix sockid;

    vjSocketPosix() {
	sockid = -1;
	in = 0;
	out = 0;
    }

    vjSocketPosix(vjSocketIDPosix id) {
	sockid = id;
	out = new ofstream (id);
	in = new ifstream (id);
    }

    ~vjSocketPosix () {
	close();
    }

    void close() {
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


    bool listen (int port) {
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
    vjSocketPosix* accept () {
	sockaddr_in servaddr;
	int servsock;
 	int len = sizeof (struct sockaddr_in);
 	servsock = ::accept (sockid,
 			   (sockaddr*)&servaddr, &len);
	if (servsock != -1)
	    return new vjSocketPosix (servsock);
	else
	    return NULL;
    }


    ostream* getOutputStream () {
	return out;
    }

    istream* getInputStream () {
	return in;
    }

};


typedef vjSocketPosix vjSocket;
typedef vjSocketIDPosix vjSocketID;


#else

#include <strings.h>    // For bzero()
#include <winsock2.h>


typedef SOCKET vjSocketIDWin32;
const int vjSOCKID_INVALID = -1;

class vjSocketWin32 {

public:

    ostream* out;
    istream* in;
    vjSocketIDWin32 sockid;

    vjSocketWin32() {
	sockid = -1;
	in = 0;
	out = 0;
    }

    vjSocketWin32(vjSocketIDWin32& id) {
	sockid = id;
	out = new ofstream (id);
	in = new ifstream (id);
    }

    ~vjSocketWin32 () {
	close();
    }

    void close() {
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


    bool listen (int port) {
	/* here, we open a socket & get ready to read connections */


	WSADATA wsaData;
	unsigned short wVersionRequested = (0x1<<8) | 0x1;
	int err;
	err = WSAStartup( wVersionRequested, &wsaData ); 
	if(err < 0) {
	    return false;
	    // output the error.
	    //std::string error;
	    //socketUtil::getLastError( error );
	    //cout<<"socketUtil: "<<error<<"\n"<<flush;
	}


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
    vjSocketWin32* accept () {
	sockaddr_in servaddr;
	int servsock;
 	int len = sizeof (struct sockaddr_in);
 	servsock = ::accept (sockid,
 			   (sockaddr*)&servaddr, &len);
	if (servsock != -1)
	    return new vjSocketWin32 (servsock);
	else
	    return NULL;
    }


    ostream* getOutputStream () {
	return out;
    }

    istream* getInputStream () {
	return in;
    }

};


typedef vjSocketWin32 vjSocket;
typedef vjSocketIDWin32 vjSocketID;


#endif //ifndef WIN32

#endif

