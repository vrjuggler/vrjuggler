/*
 *  File:          $RCSfile$
 *  Date modified: $Date$
 *  Version:       $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                             Patrick Hartling
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                  Copyright (C) - 1997, 1998, 1999, 2000
 *              Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#ifndef VJ_WORD_BROKER_INCLUDED
#define VJ_WORD_BROKER_INCLUDED



#ifdef WIN32
    //#include <winsock.h>		//for sockets
    #include <winsock2.h>		//for sockets + WSAHtonl and WSANtohl
    #include <ostream.h>		//for output stream
    inline void sginap(int x) { Sleep(x); }
#else
    
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <errno.h>       // for oserror, goserror, seterror
    #include <sys/errno.h>   // for socket error codes
    #include <netinet/in.h>  // for sockaddr_in
    #include <netinet/tcp.h> // for TCP_NODELAY
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <sys/un.h>     // for socketaddr_un
    #include <iostream.h>		//for output stream    
    #define TRACE printf
    #define SOCKET int
    #define SOCKET_ERROR -1
#endif

#include <string> // for string
#include <list>			//for list


class socketUtil
{
public:
	//: Constructor
	socketUtil();
	
	//: Destructor
	virtual ~socketUtil();

	//: the role that this instance of socketUtil will perform
	enum Role
	{
	    //: this socketUtil will be a server
	    SERVER= 0, 
	    
	    //: this socketUtil will be a client
	    CLIENT = 1 
	};
	
	//: set the role that this instance of socketUtil will perform
	// give - CLIENT or SERVER
	void setRole( const Role& serverOrClient );
	
	//: set the info for the remote socketUtil you're connecting to
	// give - address (string)
	// give - port
	// optional - a descriptive name
	void setRemote( const std::string& address, const int& port, const std::string& name = "unnamed" );
	
	//: set the info for the remote socketUtil you're connecting to
	// give - address (char*)
	// give - port
	// optional - a descriptive name
	void setRemote( const char* const address, const int& port, const std::string& name = "unnamed" );
	
	//: connect to remote using the info set by setRemote()
	// pre - must use setRemote properly before this will work
	void connect();
	
	//: disconnects current connection
	void disconnect();
	
	//: the type of message you're sending
	enum MessageType
	{
	    //: data is a text string
	    STRING = 0, 
	    
	    //: 1st 4 bytes of data is an integer
	    INT32 = 1, 
	    
	    //: 1st 4 bytes of data is a float
	    FLOAT32 = 2, 
	    
	    //: 1st 8 bytes of data is a double
	    FLOAT64 = 3, 
	    
	    //: 1st 4 bytes of data is an integer specifying a command
	    COMMAND = 4
	};
	
	//: a Message
	//  this is what you can send with this socketUtil class.
	class Message
	{
	public:
	    //: Constructor
	    Message() : messageType(STRING), data(), sizeOfData(0) {;}
	    
	    //: Copy Constructor
	    Message(const Message& msg) { this->copy(msg); }
	    
	    //: Destructor
	    virtual ~Message() {;}
	    
	    //: Equality operator
	    const Message& operator==(const Message& message);
	    
	    //: copy msg to self
	    void copy(const Message& msg);
	    
	    //: the message type
	    MessageType messageType;
	    
	    //: message data
	    std::string data;
	    
	    //: just for a sanity check, the size of "data"
	    long sizeOfData;
	};
	
	//: dequeue the current message.
	//  if queue is empty, then function does nothing.
	void	    dequeue();
	
	//: enqueue a new message.
	void	    enqueue(const Message& message);
	
	//: get the current message from the queue.
	//  function does not dequeue the message. <BR>
	//  NOTE: check queueSize() before getCurrentMessage()
	//        if queue is empty, then function asserts.
	void	    getCurrentMessage(Message& message) const;
	
	//: get the queue size
	int	    queueSize() const;
	
	//: wait for incoming data from the other.
	//  add any messages to the message queue.
	void	    collectIncomingData();
	
	//: send the entire message queue
	//  dequeues each message. <BR>
	//  if queue is empty, then function does nothing.
	void	    sendQueue();
	
	//: send the current message
	//  dequeues the message after send. <BR>
	//  if queue is empty, then function does nothing.
	void	    sendCurrentMessage();
	
	//: get the last error created by sockets.
	//  returns - a text description of the error.
	static void getLastError(std::string& error);
protected:

	//: a socket connection
	class Connection
	{
	public:
	    //: Constructor
	    Connection( const int& p = 6200, 
			const char* const add = "000.000.000.000", 
			const char* const n = "unnamed" )	
			: address( add ), port( p ), name( n ) 
	    {;}
	    
	    //: Number of the winsock port
	    int			port;    
	    
	    //: descriptive name of this connection.
	    std::string		name;    
	    
	    //: address in form "xxx.xxx.xxx.xxx"
	    std::string		address; 
	    
	    //: Sockets things. . . 
	    SOCKET		socket;
	    SOCKET		message;
	};
	
	//serve the added connections.
	// returns the number of connections failed. 0 == all connected fine.
	static int  connectToClient( Connection& connection );
	
	//closes the socket connections.
	static void closeConnection( Connection& connection );

	//connect to a server, use Connection #1 as the server's connection.
	//you must use addConnection once before calling this function.
	// specify the parameters of the server in this, the ip address and name is not important.
	static bool connectToServer( Connection& connection );

	//: pack the Message into a data buffer
	static void pack( const SOCKET& socket, const Message& message, char*& data, int& packedMsgSize  );

	//: unpack a data buffer into a Message
	static void unpack( const SOCKET& socket, const char* const data, const int& dataSize, Message*& message );

	
	//: the connection to the other socketUtil entity.
	Connection _connection;
	
	//: the message queue.
	std::list<Message*> _messageQueue;
	
	//: the role that this socketUtil will take (SERVER or CLIENT)
	Role _role;
};


#endif
