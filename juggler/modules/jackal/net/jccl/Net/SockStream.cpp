
#include <Environment/vjSockStream.h>

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

const size_t sockstreambuf::BUFSIZE = 128;


sockstreambuf::sockstreambuf (int _sock/*, char* c, int l*/):streambuf() {
    sock = _sock;
    buf = 0;
    allocate();
}


sockstreambuf::sockstreambuf ():streambuf() {
    sock = -1;
}


int sockstreambuf::doallocate (){
    // return 1 on allocation and 0 if there is no need
    
    //          if (!base ()) {
    buf = new char[2*BUFSIZE];  //allocate a buffer
    setb (buf, buf+BUFSIZE, 0);       //set the reserve area
    setg (buf, buf, buf);            //set the get area
    //              setg (buf, buf+BUFSIZE, buf+BUFSIZE);
    
    //buf += BUFSIZE;
    setp (buf+BUFSIZE, buf+2*BUFSIZE);          //set the put area
    return 1;
    //         }
    //          return 0;
}


int sockstreambuf::overflow (int c) {
    //cout << "overflow: pbase is " << (int)pbase() << " and msg size is " << (int)(pptr()-pbase()) << endl;
    
    send (sock, pbase(), pptr()-pbase(), 0);
    //setp (pbase(), epptr());
    setp (buf+BUFSIZE, buf+2*BUFSIZE);          //set the put area
    if (c != EOF)
        sputc(c);
    return 1;
    
}


int sockstreambuf::underflow () {
    char c;
    //cout << "underflow: gptr is " << (int)gptr() << " and egptr is " << (int)egptr() << endl;
    if (gptr() < egptr()) {
        // does underflow ever actually get called under this circumstance?
        // there's already a character to read...
        c = *gptr();
        return c;
    }
    else {
        int nread = recv (sock, base(), BUFSIZE, 0);
        //cout << "nread = " << nread << endl;
        if (nread > 0) {
            setg (base(), base(), base()+nread);
            return *base();
        }
        else {
            setg (buf, buf, buf);
            return EOF;
        }
    }
    return EOF;
}


int sockstreambuf::sync() {
    //cout << "sync was called!" << endl;
    overflow();
    return 0;
    //return streambuf::sync();
}

