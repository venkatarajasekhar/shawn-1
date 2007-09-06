/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifdef SHAWN
	#include <apps/tcpip/socket.h>
	#include <sys/simulation/simulation_controller.h>
#else
	#include "socket.h"
#endif

#ifdef BUILD_TCPIP


#ifndef WIN32
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/tcp.h>
	#include <arpa/inet.h>
	#include <netdb.h>
	#include <errno.h>
	#include <fcntl.h>
#else
	#ifdef ERROR
		#undef ERROR
	#endif

	#include <winsock2.h>

	#ifndef vsnprintf
		#define vsnprintf _vsnprintf
	#endif

#endif

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <assert.h>
#include <string>
#include <vector>
#include <string>
#include <algorithm>
#include <string.h>

using namespace std;


#ifdef SHAWN
    extern "C" void init_tcpip( shawn::SimulationController& sc )
    {
            // std::cout << "tcpip init" << std::endl;
    }
#endif

namespace tcpip
{
#ifdef WIN32
	bool Socket::init_windows_sockets_ = true;
	bool Socket::windows_sockets_initialized_ = false;
	int Socket::instance_count_ = 0;
#endif

	// ----------------------------------------------------------------------
	Socket::
		Socket(std::string host, int port) 
		: host_( host ),
		port_( port ),
		socket_(-1),
		server_socket_(-1),
		blocking_(true)
	{
		init();
	}

	// ----------------------------------------------------------------------
	Socket::
		Socket(int port) 
		: host_(""),
		port_( port ),
		socket_(-1),
		server_socket_(-1),
		blocking_(true)
	{
		init();
	}

	// ----------------------------------------------------------------------
	void
		Socket::
		init()
	{
#ifdef WIN32
		instance_count_++;

		if( init_windows_sockets_ && !windows_sockets_initialized_ )
		{
			WSAData wsaData;
			if( WSAStartup(MAKEWORD(1, 1), &wsaData) != 0 )
				BailOnSocketError("Unable to init WSA Sockets");
			windows_sockets_initialized_ = true;
		}
#endif
	}

	// ----------------------------------------------------------------------
	Socket::
		~Socket()
	{
		// Close first an existing client connection ...
		close();
#ifdef WIN32
		instance_count_--;
#endif

		// ... then the server socket
		if( server_socket_ >= 0 )
		{
#ifdef WIN32
			::closesocket( server_socket_ );
#else
			::close( server_socket_ );
#endif
			server_socket_ = -1;
		}

#ifdef WIN32
		if( server_socket_ == -1 && socket_ == -1 
		    && init_windows_sockets_ && instance_count_ == 0 )
				WSACleanup();
#endif
	}

	// ----------------------------------------------------------------------
	void 
		Socket::
		BailOnSocketError( std::string context) 
		const throw( SocketException )
	{
#ifdef WIN32
		int e = WSAGetLastError();
		std::string msg = GetWinsockErrorString( e );
#else
		std::string msg = strerror( errno );
#endif
		throw SocketException( context + ": " + msg );
	}

	// ----------------------------------------------------------------------
	int  
		Socket::
		port()
	{
		return port_;
	}


	// ----------------------------------------------------------------------
	bool 
		Socket::
		datawaiting(int sock) 
		const throw()
	{
		fd_set fds;
		FD_ZERO( &fds );
		FD_SET( sock, &fds );

		struct timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		int r = select( sock+1, &fds, NULL, NULL, &tv);

		if (r < 0)
			BailOnSocketError("tcpip::Socket::datawaiting @ select");

		if( FD_ISSET( sock, &fds ) )
			return true;
		else
			return false;
	}

	// ----------------------------------------------------------------------
	bool
		Socket::
		atoaddr( std::string address, struct in_addr& addr)
	{
		struct hostent* host;
		struct in_addr saddr;

		// First try nnn.nnn.nnn.nnn form
		saddr.s_addr = inet_addr(address.c_str());
		if (saddr.s_addr != static_cast<unsigned int>(-1)) 
		{
			addr = saddr;
			return true;
		}

		host = gethostbyname(address.c_str());
		if( host ) {
			addr = *((struct in_addr*)host->h_addr_list[0]);
			return true;
		}

		return false;
	}


	// ----------------------------------------------------------------------
	void 
		Socket::
		accept()
		throw( SocketException )
	{
		if( socket_ > 1 )
			return;

		struct sockaddr_in client_addr;
#ifdef WIN32
		int addrlen = sizeof(client_addr);
#else
		socklen_t addrlen = sizeof(client_addr);
#endif

		if( server_socket_ < 0 )
		{
			struct sockaddr_in self;

			//Create the server socket
			server_socket_ = static_cast<int>(socket( AF_INET, SOCK_STREAM, 0 ));
			if( server_socket_ < 0 )
				BailOnSocketError("tcpip::Socket::accept() @ socket");
			
			//"Address already in use" error protection
			{
				int reuseaddr = 1;

				#ifdef WIN32
					//setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseaddr, sizeof(reuseaddr));
					// No address reuse in Windows!!!
				#else
					setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
				#endif
			}

			// Initialize address/port structure
			memset(&self, 0, sizeof(self));
			self.sin_family = AF_INET;
			self.sin_port = htons(port_);
			self.sin_addr.s_addr = htonl(INADDR_ANY);

			// Assign a port number to the socket
			if ( bind(server_socket_, (struct sockaddr*)&self, sizeof(self)) != 0 )
				BailOnSocketError("tcpip::Socket::accept() Unable to create listening socket");


			// Make it a "listening socket"
			if ( listen(server_socket_, 10) == -1 )
				BailOnSocketError("tcpip::Socket::accept() Unable to listen on server socket");

			// Make the newly created socket blocking or not
			set_blocking(blocking_);
		}

		socket_ = static_cast<int>(::accept(server_socket_, (struct sockaddr*)&client_addr, &addrlen));

		if( socket_ > 0 )
		{
			int x = 1;
			setsockopt(socket_, IPPROTO_TCP, TCP_NODELAY, (const char*)&x, sizeof(x));
		}
	}

	// ----------------------------------------------------------------------
	void 
		Socket::
		set_blocking(bool blocking) 
		throw(SocketException )
	{
		blocking_ = blocking;

		if( server_socket_ > 0 )
		{
#ifdef WIN32
			ULONG NonBlock = blocking_ ? 0 : 1;
		    if (ioctlsocket(server_socket_, FIONBIO, &NonBlock) == SOCKET_ERROR)
				BailOnSocketError("tcpip::Socket::set_blocking() Unable to initialize non blocking I/O");
#else
			long arg = fcntl(server_socket_, F_GETFL, NULL);
			if (blocking_)
			{
				arg &= ~O_NONBLOCK;
			} else {
				arg |= O_NONBLOCK;
			}
			fcntl(server_socket_, F_SETFL, arg);
#endif
		}
	
	}

	// ----------------------------------------------------------------------
	void 
		Socket::
		connect()
		throw( SocketException )
	{
		in_addr addr;
		if( !atoaddr( host_.c_str(), addr) )
			BailOnSocketError("tcpip::Socket::connect() @ Invalid network address");

		sockaddr_in address;
		memset( (char*)&address, 0, sizeof(address) );
		address.sin_family = AF_INET;
		address.sin_port = htons( port_ );
		address.sin_addr.s_addr = addr.s_addr;

		socket_ = static_cast<int>(socket( AF_INET, SOCK_STREAM, 0 ));
		if( socket_ < 0 )
			BailOnSocketError("tcpip::Socket::connect() @ socket");

		if( ::connect( socket_, (sockaddr const*)&address, sizeof(address) ) < 0 )
			BailOnSocketError("tcpip::Socket::connect() @ connect");
	}

	// ----------------------------------------------------------------------
	void 
		Socket::
		close()
	{
		// Close client-connection 
		if( socket_ >= 0 )
		{
#ifdef WIN32
			::closesocket( socket_ );
#else
			::close( socket_ );
#endif

			socket_ = -1;
		}
	}

	// ----------------------------------------------------------------------
	void 
		Socket::
		send( std::vector<unsigned char> b) 
		throw( SocketException )
	{
		if( socket_ < 0 )
			return;

		size_t numbytes = b.size();
		unsigned char *buf = new unsigned char[numbytes];
		unsigned char *buf_ = buf;
		for(size_t i = 0; i < numbytes; ++i)
			buf[i] = b[i];

		while( numbytes > 0 )
		{
#ifdef WIN32
			int n = ::send( socket_, (const char*)buf, static_cast<int>(numbytes), 0 );
#else
			int n = ::send( socket_, buf, numbytes, 0 );
#endif
			if( n<0 )
				BailOnSocketError( "send failed" );

			numbytes -= n;
			buf += n;
		}

		delete[] buf_;
	}
	

	
        // ----------------------------------------------------------------------

	void
		Socket::
		sendExact( const Storage &b)
		throw( SocketException )
	{
		int length = static_cast<int>(b.size());
		Storage length_storage;
		length_storage.writeInt(length);
		vector<unsigned char> msg;
		msg.insert(msg.end(), length_storage.begin(), length_storage.end());
		msg.insert(msg.end(), b.begin(), b.end());
		send(msg);
	}


	
	// ----------------------------------------------------------------------
	vector<unsigned char> 
		Socket::
		receive(int bufSize)
		throw( SocketException )
	{
		vector<unsigned char> b;

		if( socket_ < 0 )
			connect();

		if( !datawaiting( socket_) )
			return b;

		unsigned char* buf = new unsigned char[bufSize];
		int a = recv( socket_, (char*)buf, bufSize, 0 );

		if( a < 0 )
			BailOnSocketError( "tcpip::Socket::receive() @ recv" );

		b.resize(a);
		for(int i = 0; i < a; ++i)
			b[i] = buf[i];

		delete[] buf;
		return b;
	}

	// ----------------------------------------------------------------------
	

	bool
		Socket::
		receiveExact( Storage &msg )
		throw( SocketException )
	{
		/* receive length of vector */
		Storage length_storage;
		while (length_storage.size()<4)
		{
			vector<unsigned char> rcv = receive(4 - length_storage.size());
			for (vector<unsigned char>::iterator iter=rcv.begin(); 
				iter != rcv.end(); 
				length_storage.writeChar(*iter++)); 
		}
		int NN = length_storage.readInt();
		
		/* receive vector */
		int n = NN;
		msg.reset();
		while (n>0)
		{
			vector<unsigned char> rcv = receive(n);
			vector<unsigned char>::iterator iter = rcv.begin();
			while (iter != rcv.end())
			{
				msg.writeChar(*iter);
				++iter;
				--n;
			}
		}
		return true;
	}
	
	
	// ----------------------------------------------------------------------
	bool 
		Socket::
		has_client_connection() 
		const
	{
		return socket_ > 0;
	}

	// ----------------------------------------------------------------------
	bool 
		Socket::
		is_blocking() 
		throw()
	{
		return blocking_;
	}


#ifdef WIN32
	// ----------------------------------------------------------------------
	std::string 
		Socket::
		GetWinsockErrorString(int err) 
		const
	{

		switch( err)
		{
		case 0:					return "No error";
		case WSAEINTR:			return "Interrupted system call";
		case WSAEBADF:			return "Bad file number";
		case WSAEACCES:			return "Permission denied";
		case WSAEFAULT:			return "Bad address";
		case WSAEINVAL:			return "Invalid argument";
		case WSAEMFILE:			return "Too many open sockets";
		case WSAEWOULDBLOCK:	return "Operation would block";
		case WSAEINPROGRESS:	return "Operation now in progress";
		case WSAEALREADY:		return "Operation already in progress";
		case WSAENOTSOCK:		return "Socket operation on non-socket";
		case WSAEDESTADDRREQ:	return "Destination address required";
		case WSAEMSGSIZE:		return "Message too long";
		case WSAEPROTOTYPE:		return "Protocol wrong type for socket";
		case WSAENOPROTOOPT:	return "Bad protocol option";
		case WSAEPROTONOSUPPORT:	return "Protocol not supported";
		case WSAESOCKTNOSUPPORT:	return "Socket type not supported";
		case WSAEOPNOTSUPP:		return "Operation not supported on socket";
		case WSAEPFNOSUPPORT:	return "Protocol family not supported";
		case WSAEAFNOSUPPORT:	return "Address family not supported";
		case WSAEADDRINUSE:		return "Address already in use";
		case WSAEADDRNOTAVAIL:	return "Can't assign requested address";
		case WSAENETDOWN:		return "Network is down";
		case WSAENETUNREACH:	return "Network is unreachable";
		case WSAENETRESET:		return "Net Socket reset";
		case WSAECONNABORTED:	return "Software caused tcpip::Socket abort";
		case WSAECONNRESET:		return "Socket reset by peer";
		case WSAENOBUFS:		return "No buffer space available";
		case WSAEISCONN:		return "Socket is already connected";
		case WSAENOTCONN:		return "Socket is not connected";
		case WSAESHUTDOWN:		return "Can't send after socket shutdown";
		case WSAETOOMANYREFS:	return "Too many references, can't splice";
		case WSAETIMEDOUT:		return "Socket timed out";
		case WSAECONNREFUSED:	return "Socket refused";
		case WSAELOOP:			return "Too many levels of symbolic links";
		case WSAENAMETOOLONG:	return "File name too long";
		case WSAEHOSTDOWN:		return "Host is down";
		case WSAEHOSTUNREACH:	return "No route to host";
		case WSAENOTEMPTY:		return "Directory not empty";
		case WSAEPROCLIM:		return "Too many processes";
		case WSAEUSERS:			return "Too many users";
		case WSAEDQUOT:			return "Disc quota exceeded";
		case WSAESTALE:			return "Stale NFS file handle";
		case WSAEREMOTE:		return "Too many levels of remote in path";
		case WSASYSNOTREADY:	return "Network system is unavailable";
		case WSAVERNOTSUPPORTED:	return "Winsock version out of range";
		case WSANOTINITIALISED:	return "WSAStartup not yet called";
		case WSAEDISCON:		return "Graceful shutdown in progress";
		case WSAHOST_NOT_FOUND:	return "Host not found";
		case WSANO_DATA:		return "No host data of that type was found";
		}

		return "unknown";
	}

#endif // WIN32

} // namespace tcpip

#endif // BUILD_TCPIP

/*-----------------------------------------------------------------------
* Source  $Source: $
* Version $Revision$
* Date    $Date$
*-----------------------------------------------------------------------
* $Log: $
*-----------------------------------------------------------------------*/
