/*
 *  File:	    $Name$
 *  Date modified:  $Date$
 *  Version:	    $Revision$
 *
 *
 *                                VR Juggler
 *                                    by
 *                              Allen Bierbaum
 *                             Christopher Just
 *                            Carolina Cruz-Neira
 *                               Albert Baker
 *
 *                         Copyright  - 1997,1998,1999
 *                Iowa State University Research Foundation, Inc.
 *                            All Rights Reserved
 */


#include <assert.h>

#include "socketUtil.h"

socketUtil::socketUtil() : _role(CLIENT)
{
    
}

socketUtil::~socketUtil()
{
    
}

void socketUtil::Message::copy(const socketUtil::Message& msg)
{
    messageType = msg.messageType;
    data = msg.data;
    sizeOfData = msg.sizeOfData;
}


const socketUtil::Message& socketUtil::Message::operator==(const socketUtil::Message& message)
{
    this->copy(message);
    return *this;
}

void socketUtil::setRole( const Role& serverOrClient )
{
    _role = serverOrClient;
    switch (_role)
    {
    case CLIENT:
	cout<<"Setting role to client\n"<<flush;
	break;
    case SERVER:
	cout<<"Setting role to server\n"<<flush;
	break;
    }
}

//: set the info for the remote socketUtil you're connecting to
// give - address (string)
// give - port
// optional - a descriptive name
void socketUtil::setRemote( const std::string& address, const int& port, const std::string& name )
{
    this->setRemote( (char*)address.data(), port, (char*)name.data() );   
}

//: set the info for the remote socketUtil you're connecting to
// give - address (char*)
// give - port
// optional - a descriptive name
void socketUtil::setRemote( const char* const address, const int& port, const std::string& name )
{
    cout<<"Using "<<address<<" on port "<<port<<"\n"<<flush;
    
    _connection.address = address;
    _connection.port = port;
    
    if (name == "unnamed")
        _connection.name = address;
    else _connection.name = name;
}

//: connect to remote using the info set by setRemote()
// pre - must use setRemote properly before this will work
void socketUtil::connect()
{
    cout<<"Connecting...\n"<<flush;
    
    //connect to "_connection" as "_role"
    switch( _role )
    {
    case SERVER:
	this->connectToClient( _connection );
	break;
	
    default:
    case CLIENT:
	this->connectToServer( _connection );
	break;
    }
}

//: disconnects current connection
void socketUtil::disconnect()
{
    // redundant...
    switch( _role )
    {
    case SERVER:
	this->closeConnection( _connection );
	break;
	
    default:
    case CLIENT:
	this->closeConnection( _connection );
	break;
    }
}
	
void socketUtil::dequeue()
{
    // if queue is empty, then return.
    if (_messageQueue.size() == 0)
	return;
	
    // delete the memory we're dequeuing.
    delete *_messageQueue.begin();
    
    // pop the first one off the queue.
    _messageQueue.pop_front();
}

void socketUtil::enqueue(const Message& message)
{
    // make a new copy of the Message.
    Message* msg = new Message(message);
    
    // enqueue the copy
    _messageQueue.push_back(msg);
}

void socketUtil::getCurrentMessage(Message& message) const
{
    // if queue is empty, then return.
    if (_messageQueue.size() == 0)
    {
	// check "queueSize()" before "getCurrentMessage()"
	assert(_messageQueue.size() > 0);
	return;
    }
    
    // return the first item in the queue
    Message* msg = *_messageQueue.begin();
    message = *msg;
}

int socketUtil::queueSize() const
{
    return _messageQueue.size();
}

void socketUtil::sendQueue()
{
    // if queue is empty, then return.
    if (_messageQueue.size() == 0)
	return;
    
    // send each item in the queue
    while( this->queueSize() > 0 )
    {
	// send the current message (this dequeues the message)
	this->sendCurrentMessage();
    }
}

void socketUtil::sendCurrentMessage()
{
    // if queue is empty, then return.
    if (_messageQueue.size() == 0)
	return;
	
    cout<<"Sending current message\n"<<flush;
    
    //: do the network majick to send current message to 
    //      _connection.port IP address.
    
    // Pack the current message for electronic travel. :)
    char* packedMsg = NULL;
    int packedMsgSize = 0;
    Message* msg = *_messageQueue.begin();
    cout<<"Packing data...\n"<<flush;
    socketUtil::pack( _connection.socket, *msg, packedMsg, packedMsgSize );
    
    cout<<"Packed: "<<packedMsgSize<<" bytes: "<<flush;
    for (int tt = 0; tt < packedMsgSize; ++tt)
	cout<<packedMsg[tt]<<flush;
    cout<<"\n"<<flush;

    // try a maximum of 4 (magic number) times to send the packedMsg
    int result = -1;
    for (int trials = 0; trials < 4 && result != packedMsgSize; ++trials)
    {
	cout<<"sending (try "<<trials<<"): "<<msg->data<<" . . . "<<flush;
	// send the packed message using Sockets.
	result = send( _connection.socket, packedMsg, packedMsgSize, 0 );
	cout<<"Result = "<<result<<", Message Size = "<<packedMsgSize<<"\n"<<flush;
	if ( result != packedMsgSize )
	{
	    std::string error;
	    socketUtil::getLastError(error);
	    cout<<"\nsocketUtil: "<<error<<"\n"<<flush;
	}
    }
   
    // if Message arrived safely, then sleep for a bit, 
    // it was a long journey, message is tired..
    if ( result == packedMsgSize )
    {
	cout<<" success\n"<<flush;
	sginap(5);
    }  else {
	cout<<" failed\n"<<flush;
	cout<<"Result = "<<result<<", Message Size = "<<packedMsgSize<<"\n"<<flush;
	
    }
    
    // delete the buffer, we're done with it.
    delete packedMsg;
    
    // dequeue the current message.
    this->dequeue();
}


//: wait for incoming data from the other.
//  add any messages to the message queue.
void socketUtil::collectIncomingData()
{
    cout<<"Waiting for incoming data...\n"<<flush;
    // do the network majick to recv any data from
    //      _connection.port IP address.
    
    const int maxbuflen = 230;
    char packedMsg[maxbuflen];
    int err = recv( _connection.message, packedMsg, sizeof(packedMsg), 0 );
    
    cout<<"Recieved: "<<err<<" bytes: "<<flush;
    for (int tt = 0; tt < err; ++tt)
	cout<<packedMsg[tt]<<flush;
    cout<<"\n"<<flush;
    
    if ( err != SOCKET_ERROR )
    {
	cout<<"Unpacking...\n"<<flush;
	
	// success, unpack the data.
	Message* unpackedMsg = NULL;
	socketUtil::unpack( _connection.socket, packedMsg, maxbuflen, unpackedMsg );

	// TODO: MAYBE: make sure we didn't get bogus data, if the size is less than 1 or bigger than maxbuflen, 
	// then it probably is bogus.
	//if (unpackedWord->size < maxbuflen && unpackedWord->size >= 1)
	//{
	//unpackedWord->string[ unpackedWord->size - 1 ] = '\0';   
	//}
	
	// enqueue the newly recieved message
	this->enqueue( *unpackedMsg );
	cout<<"Queued a new (recieved) message\n"<<flush;

	// delete the message now that we're done with it.
	delete unpackedMsg;
    } else 
    {
	cout<<"socketUtil: Socket error:\n"<<flush;
	// output the error.
	std::string error;
	socketUtil::getLastError( error );
	cout<<"socketUtil: "<<error<<"\n"<<flush;
    }
}


//serve the added connections.
// returns the number of connections failed. 0 == all connected fine.
int socketUtil::connectToClient( Connection& connection )
{
    sockaddr_in			addr;
    int					result = 0;
    int					errors = 0;

    
    cout<<"Connecting to "<<connection.name<<".\n"<<flush;
    
    // Do WIN32 WSA Startup command if compiled on WIN32
    #ifdef WIN32
	    WSADATA wsaData;
	    unsigned short wVersionRequested = (0x1<<8) | 0x1;
	    int err;
	    err = WSAStartup( wVersionRequested, &wsaData ); 
	    if(err < 0)
	    {
		// output the error.
		std::string error;
		socketUtil::getLastError( error );
		cout<<"socketUtil: "<<error<<"\n"<<flush;
	    }
    #endif
    
    // Create a TCP socket connection
    connection.socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // See if socket was created:
    if( connection.socket >= 0 )
    {
	cout<<"TCP socket opened to "<<connection.name<<"\n"<<flush;
	
	// Set up the address and port number for communication
	addr.sin_family		= AF_INET;
	addr.sin_port		= htons( connection.port );
	const char* charAddress = connection.address.data();
	addr.sin_addr.s_addr = inet_addr( charAddress );
	
	// Connect to the newly opened TCP/IP socket
	result = ::connect( connection.socket, (struct sockaddr *)&addr, sizeof(addr) );

	// See if we connected to the socket successfully:
	if( result >= 0)
	{
		cout<<"TCP socket connected to "<<charAddress<<", Turning off NAGLE algorithm.\n"<<flush;

		// Turn off nagel algorithm for less latency.
		const char nagle = 1;
		setsockopt( connection.socket, IPPROTO_TCP, TCP_NODELAY, &nagle, sizeof(int) );
	} else {
		cout<< "TCP socket connection error to "<<charAddress<<".\n"<<flush;

		// output the error.
		std::string error;
		socketUtil::getLastError( error );
		cout<<"socketUtil: "<<error<<"\n"<<flush;

		// Couldn't make connection, so close the socket
		#ifdef WIN32
			int err = closesocket( connection.socket );
		#else
			int err = close( connection.socket );
		#endif
		++errors;
	}
    } else {
	cout<< "Unable to open TCP socket to the client.\n"<<flush;

	// output the error.
	std::string error;
	socketUtil::getLastError( error );
	cout<<"socketUtil: "<<error<<"\n"<<flush;

	// TODO: kill the connection or try again... 
	++errors;
    }

    return errors;
}

//closes the socket connections.
void socketUtil::closeConnection( Connection& connection )
{
    #ifdef WIN32
	    int	err = closesocket( connection.socket );
    #else
	    int	err = close( connection.socket );
    #endif
    cout<<"Closed socket "<<connection.name<<"... Error:"<<err<<"\n"<<flush;
    
    if (err < 0) 
    {
	// output the error.
	std::string error;
	socketUtil::getLastError( error );
	cout<<"socketUtil: "<<error<<"\n"<<flush;
    }
}

//connect to a server, use Connection #1 as the server's connection.
//you must use addConnection once before calling this function.
// specify the parameters of the server in this, the ip address and name is not important.
bool socketUtil::connectToServer( Connection& connection )
{
    int		err;

    #ifdef WIN32
	WSADATA wsaData;
	unsigned short wVersionRequested = (0x1<<8) | 0x1;
	err = WSAStartup( wVersionRequested, &wsaData ); 
	if(err < 0)
	{
	    // output the error.
	    std::string error;
	    socketUtil::getLastError( error );
	    cout<<"socketUtil: "<<error<<"\n"<<flush;
	}
    #endif
	    
    // create a socket to the server.
    connection.socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // if success, then connect to the socket
    if( connection.socket >= 0 )
    {
	cout<< "Created a socket to server.\n"<<flush;

	// Set up the address and port number for communication
	sockaddr_in             addr_in;
	addr_in.sin_family      = AF_INET;
	addr_in.sin_addr.s_addr = INADDR_ANY;
	addr_in.sin_port        = htons( connection.port );
	
	//create an alias of type sockaddr
	sockaddr &addr = *(reinterpret_cast<sockaddr *>( &addr_in ));

	// Bind to the server's socket
	err = bind( connection.socket, &addr, sizeof(addr) );
	if(err < 0)
	{
		cout<<"Could not bind to server's socket\n"
		    <<"  Closing server's socket.\n"<<flush;
		
		// output the error.
		std::string error;
		socketUtil::getLastError( error );
		cout<<"socketUtil: "<<error<<"\n"<<flush;
		
		#ifdef WIN32
			int	err = closesocket( connection.socket );
		#else
			int	err = close( connection.socket );
		#endif
		return false;
	}

	// Listen to the server's socket
	// We'll wait until it talks (this is called blocking)
	cout<< "Listening...\n"<<flush;
	listen( connection.socket, 5 );

	// Server talked, accept its connection
	int addrlen = sizeof(addr);
	connection.message = accept( connection.socket, &addr, &addrlen );

	// Did we accept it without errors?
	if( connection.message >= 0 )
	{
		cout<< "Accepted connection from server.\n"<<flush;
	} else {
		cout<< "Could not accept connection to server.\nClosing server's socket.\n"<<flush;
		
		// output the error.
		std::string error;
		socketUtil::getLastError( error );
		cout<<"socketUtil: "<<error<<"\n"<<flush;
		
		#ifdef WIN32
			int	err = closesocket( connection.socket );
		#else
			int	err = close( connection.socket );
		#endif
		return false;
	}

	cout<< "Disabling Nagle algorithm.\n"<<flush;
	const char nagle = 1;
	setsockopt( connection.socket, IPPROTO_TCP, TCP_NODELAY, &nagle, sizeof(int) );
	return true;
    } else {
	cout<< "Unable to open socket to server.\n"<<flush;
	
	// output the error.
	std::string error;
	socketUtil::getLastError( error );
	cout<<"socketUtil: "<<error<<"\n"<<flush;
    }

    return false;
}

const int MESSAGETYPE_POS = 0;
const int DATASIZE_POS = 1;
const int DATA_POS = 2;

void socketUtil::pack( const SOCKET& socket, const Message& message, char*& data, int& packedMsgSize )
{
	int numberOfLongsInData = (message.data.size()+sizeof(long)-1) / sizeof(long);
	packedMsgSize = numberOfLongsInData * sizeof(long);
	data = (char*)new char[ 2 * sizeof(long) + numberOfLongsInData ];
	assert( data!=NULL );

	unsigned long tempLong = 0;
	
	// pack the MessageType into a long-sized section of the buffer
	tempLong = message.messageType;
	#ifndef WIN32
	    ((unsigned long*)data)[MESSAGETYPE_POS] = htonl( tempLong );
	#else
	    WSAHtonl( socket, &tempLong, &((unsigned long*)data)[MESSAGETYPE_POS] ); 
	#endif
	
	// pack the data-size into a long-sized section of the buffer
	tempLong = message.data.size();
	cout<<"Packing "<<message.data<<"("<<message.data.size()<<"): "<<tempLong<<" bytes of data\n"<<flush;
	#ifndef WIN32
	    ((unsigned long*)data)[DATASIZE_POS] = htonl( tempLong );
	#else
	    WSAHtonl( socket, &tempLong, &((unsigned long*)data)[MESSAGETYPE_POS] ); 
	#endif
	
	
	cout<<"Packing: "<<numberOfLongsInData<<" number of longs\n"<<flush;
	
	// pack the data into longs
	//  (for each long in the data, pack it into the buffer)
	for (int x=0; x < numberOfLongsInData; ++x)
	{
	    // extract the next long from the data.
	    for (int t = 0; t < sizeof(long); ++t)
		((char*)&tempLong)[t] = message.data[x * sizeof(long) + t];
	    
	    cout<<"Packing: (long) "<<((char*)&tempLong)<<"\n"<<flush;
	    
	    // convert the long from hardware format to network format, 
	    // store it into the data section of the buffer:
	    unsigned long networkLong = 0;
	    #ifndef WIN32
		networkLong = htonl( tempLong );
	    #else
			// don't translate the character string in win32...
			//if (x <= 1)
		
		WSAHtonl( socket, &tempLong, &networkLong ); 
		((unsigned long*)data)[DATA_POS + x] = networkLong;
			//else
			//	((unsigned long*)data)[DATA_POS + x] = ((unsigned long*)&message)[DATA_POS + x];
	    #endif
	    ((unsigned long*)data)[DATA_POS + x] = networkLong;
		
	}
	
	cout<<"Packed: (raw)"<<&(data[DATA_POS])<<"\n"<<flush;
}

void socketUtil::unpack( const SOCKET& socket, const char* const data, const int& dataSize, Message*& message )
{
	message = new Message;
	assert(message!=NULL);
	unsigned long tempLong = 0;
	
	cout<<"Unpacking: (raw)"<<&(data[DATA_POS])<<"\n"<<flush;
	
	// unpack the MessageType from the data buffer
	#ifndef WIN32
	    tempLong = ntohl( ((unsigned long*)data)[MESSAGETYPE_POS] );
	#else    
	    WSANtohl( socket, ((unsigned long*)data)[MESSAGETYPE_POS], &tempLong ); 
	#endif
	message->messageType = (MessageType) tempLong;
	
	// unpack the data-size from the data buffer
	#ifndef WIN32
	    message->sizeOfData = ntohl( ((unsigned long*)data)[DATASIZE_POS] );
	#else
	    WSANtohl( socket, ((unsigned long*)data)[DATASIZE_POS], &message->sizeOfData ); 
	#endif
	cout<<"Unpacking: "<<message->sizeOfData/sizeof(long)<<" longs of data\n"<<flush;
	
	message->data.reserve( message->sizeOfData );
	
	// unpack the data from the data buffer into a std::string
	for (int x = 0; x < message->sizeOfData/sizeof(long); ++x)
	{
	    // get the next long of "data" from the buffer
	    #ifndef WIN32
		tempLong = ntohl( ((unsigned long*)data)[DATA_POS + x] );
	    #else
		WSANtohl( socket, ((unsigned long*)data)[DATA_POS + x], &tempLong ); 
	    #endif
	    
	    cout<<tempLong<<"\n"<<flush;
	    
	    // copy the data from the long into the std::string.
	    for (int t = 0; t < sizeof(long); ++t)
	    {
		message->data[x*sizeof(long) + t] = ((char*)&tempLong)[t];
	    }
	}
	cout<<"Unpacked: "<<message->data<<"\n"<<flush;
}


void socketUtil::getLastError(std::string& error)
{
	int code=0;
	
	#ifdef WIN32
	code = WSAGetLastError();
	#else
	code = oserror();
	#endif
	
	cout<<code<<": ";
	
	switch(code)
	{
	#ifdef WIN32
	case SOCKET_ERROR:
	    error = " a socket error\n"<<flush;
	    break;
	case WSANOTINITIALISED:
		error = "A successful WSAStartup must occur before using this function.\n";
		break;
	case WSAENETDOWN:
		error = "The network subsystem has failed. \n";
		break;
	case WSAEFAULT: 
		error = "The buf parameter is not completely contained in a valid part of the user address space. \n";
		break;
	case WSAENOTCONN: 
		error = "The socket is not connected. \n";
		break;
	case WSAEINTR: 
		error = "The (blocking) call was canceled through WSACancelBlockingCall. \n";
		error = "A blocking Windows Socket 1.1 call was canceled through WSACancelBlockingCall.\n"; 
		break;
	case WSAEINPROGRESS: 
		error = "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. \n";
		break;
	case WSAENETRESET: 
		error = "The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress. \n";
		break;
	case WSAENOTSOCK: 
		error = "The descriptor is not a socket. \n";
		break;
	case WSAEOPNOTSUPP: 
		error = "MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, out-of-band data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations. \n";
		break;
	case WSAESHUTDOWN: 
		error = "The socket has been shut down; it is not possible to recv on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH. \n";
		break;
	case WSAEWOULDBLOCK: 
		error = "The socket is marked as nonblocking and the receive operation would block. \n";
		break;
	case WSAEMSGSIZE: 
		error = "The message was too large to fit into the specified buffer and was truncated. \n";
		break;
	case WSAEINVAL: 
		error = "The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative. \n";
		error = "The timeout value is not valid, or all three descriptor parameters were NULL.\n"; 
		break;
	case WSAECONNABORTED: 
		error = "The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable. \n";
		break;
	case WSAETIMEDOUT: 
		error = "The connection has been dropped because of a network failure or because the peer system failed to respond. \n";
		break;
	case WSAECONNRESET: 
		error = "The virtual circuit was reset by the remote side executing a \"hard\" or \"abortive\" close. The application should close the socket as it is no longer usable. On a UDP datagram socket this error would indicate that a previous send operation resulted in an ICMP \"Port Unreachable\" message. \n";
		break;
	#else
	case ENETDOWN:
		error = "The network subsystem has failed. \n";
		break;
	case EFAULT: 
		error = "The buf parameter is not completely contained in a valid part of the user address space. \n";
		break;
	case ENOTCONN: 
		error = "The socket is not connected. \n";
		break;
	case EINTR: 
		error = "The (blocking) call was canceled through CancelBlockingCall. \n";
		error = "A blocking Socket call was canceled through CancelBlockingCall.\n"; 
		break;
	case EINPROGRESS: 
		error = "A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. \n";
		break;
	case ENETRESET: 
		error = "The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress. \n";
		break;
	case ENOTSOCK: 
		error = "The descriptor is not a socket. \n";
		break;
	case EOPNOTSUPP: 
		error = "MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, out-of-band data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations. \n";
		break;
	case ESHUTDOWN: 
		error = "The socket has been shut down; it is not possible to recv on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH. \n";
		break;
	case EWOULDBLOCK: 
		error = "The socket is marked as nonblocking and the receive operation would block. \n";
		break;
	case EMSGSIZE: 
		error = "The message was too large to fit into the specified buffer and was truncated. \n";
		break;
	case EINVAL: 
		error = "The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative. \n";
		error = "The timeout value is not valid, or all three descriptor parameters were NULL.\n"; 
		break;
	case ECONNABORTED: 
		error = "The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable. \n";
		break;
	case ETIMEDOUT: 
		error = "The connection has been dropped because of a network failure or because the peer system failed to respond. \n";
		break;
	case ECONNRESET: 
		error = "The virtual circuit was reset by the remote side executing a \"hard\" or \"abortive\" close. The application should close the socket as it is no longer usable. On a UDP datagram socket this error would indicate that a previous send operation resulted in an ICMP \"Port Unreachable\" message. \n";
		break;
	#endif
	default:
		error = "Unknown error\n";
		break;
	}
}
