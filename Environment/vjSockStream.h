
#ifndef _VJ_SOCKSTREAM_H_
#define _VJ_SOCKSTREAM_H_

#include <vjConfig.h>
#include <iostream.h>

class sockstreambuf:public streambuf {
public:

    int sock;
    static const size_t BUFSIZE;
    char* buf;


    sockstreambuf (int _sock/*, char* c, int l*/);


    sockstreambuf ();


    virtual int doallocate ();


    virtual int overflow (int c=EOF);


    virtual int underflow ();


    virtual int sync();
};


#endif
