
#include <string>

#include "../Core/debug.h"
#include "Network.h"

#include <fcntl.h>
#include <errno.h>

namespace Rocket {
	namespace Network {

		// Setup a minimal network
		Network::Network( int networkSettings, unsigned int updateTimeout ) {
#ifdef OS_WINDOWS
			WSADATA wsaData;
			//if (WSAStartup( MAKEWORD(2,0), &wsaData ) != 0) {
			if (WSAStartup( MAKEWORD(2,2), &wsaData ) != 0) {
				Core::Debug_AddToLog( "Error: WSAStartup failed." );
			}
#endif

			m_UDP_port = INVALID_SOCKET;
			m_TCP_listenPort = INVALID_SOCKET;

			m_settings = networkSettings;
			m_updateTimeout = updateTimeout;
		}

		// Cleanup unneeded network information
		Network::~Network() {
			// Close all open TCP connections
			std::unordered_map< SOCKET*, PacketAccumulator* >::iterator iter;
			for ( iter = m_TCP_connections.begin(); iter != m_TCP_connections.end(); ) {
				SOCKET * s = (*iter).first;
				iter++;
				close_TCP( s );
			}

			// todo: delete UDP list of connections

#ifdef OS_WINDOWS
			WSACleanup();
#endif
		}

		// Create a socket to send and recieve UDP packets
		// This socket sends and recieves for all UDP connections, regardless of destination
		unsigned int Network::setupUDP( unsigned int port, unsigned int numberOfPortTries ) {
			if ( m_settings & (int)NetworkSettings::UDP_Enabled ) {
				//bind to the first open port from port to port+numberofPortTries
				m_UDP_port = findOpenPort( port, numberOfPortTries );
				if ( m_UDP_port == 0 ) Core::Debug_AddToLog( "Error: Failed to find an open port." );

				m_UDP_socket = socket( AF_INET, SOCK_DGRAM, 0 );
				if ( m_UDP_socket == INVALID_SOCKET ) Core::Debug_AddToLog( "Error: Failed to create UDP socket." );

				SOCKADDR_IN serverInfo;
				serverInfo.sin_family = AF_INET;
				serverInfo.sin_addr.s_addr = INADDR_ANY;
				serverInfo.sin_port = htons( m_UDP_port );
				memset( &(serverInfo.sin_zero), '\0', 8 );

				if ( bind( m_UDP_socket, (LPSOCKADDR)&serverInfo, sizeof( sockaddr_in ) ) == SOCKET_ERROR ) {
					Core::Debug_AddToLog( "Error: Failed to bind UDP socket." );
					m_UDP_socket = 0;
				}
			}

			return m_UDP_port;
		}

		// Create a socket to listen for new, incoming TCP stream connections
		unsigned int Network::setupTCP_listen( unsigned int listenPort, unsigned int numberOfPortTries ) {
			if ( m_settings & (int)NetworkSettings::TCP_ListeningEnabled ) {
				//bind to the first open port from listenPort to listenPort+numberofPortTries
				m_TCP_listenPort = findOpenPort( listenPort, numberOfPortTries );
				if ( m_TCP_listenPort == 0 ) Core::Debug_AddToLog( "Error: Failed to find an open port." );

				m_TCP_listenSocket = socket( AF_INET, SOCK_STREAM, 0 );
				if ( m_TCP_listenSocket == INVALID_SOCKET ) Core::Debug_AddToLog( "Error: Failed to create listenSocket." );

				SOCKADDR_IN serverInfo;
				serverInfo.sin_family = AF_INET;
				serverInfo.sin_addr.s_addr = INADDR_ANY;
				serverInfo.sin_port = htons( m_TCP_listenPort );
				memset( &(serverInfo.sin_zero), '\0', 8 );

				if ( bind( m_TCP_listenSocket, (LPSOCKADDR)&serverInfo, sizeof(sockaddr_in) ) == SOCKET_ERROR ) {
					Core::Debug_AddToLog( "Error: Failed to bind listenSocket." );
					m_TCP_listenSocket = 0;
				}
	
				if ( listen( m_TCP_listenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
					Core::Debug_AddToLog( "Error: Listen error." );
					m_TCP_listenSocket = 0;
				}

				// Unblock m_listenSocket
#ifdef OS_WINDOWS
				u_long * unblocked = new u_long;
				*unblocked = 1;	//1 to unblock socket
				ioctlsocket( m_TCP_listenSocket, FIONBIO, unblocked );
#else
				int flags = fcntl( m_TCP_listenSocket, F_GETFL, 0 );
				fcntl( m_TCP_listenSocket, F_SETFL, flags | O_NONBLOCK );
#endif
			}

			return m_TCP_listenPort;
		}

		// Lookup the IP address for a given host
		Core::string Network::hostLookup( Core::string host, unsigned int port ) {
			struct addrinfo hints;
			struct addrinfo * servinfo;
			memset( &hints, 0, sizeof(hints) );
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			Core::Debug_AddToLog( "Looking up address:" );
			Core::Debug_AddToLog( host.c_str() );

			Core::string port_s = "";
			port_s << port;
			if ( getaddrinfo( host.c_str() , port_s.c_str() ,&hints ,&servinfo ) != 0 ) {
				Core::Debug_AddToLog( "Address Lookup Failed" );
				return "";
			}

			//if ( servinfo->ai_addrlen == 0 ) return "127.0.0.1";
			return inet_ntoa( ((struct sockaddr_in *)servinfo->ai_addr)->sin_addr );
		}

		// Create a new UDP connection to the specified destination
		Rocket::Network::PacketAccumulator * Network::connect_UDP_IP4( Core::string host, unsigned int port ) {
			Core::string IP = hostLookup( host, port );
			if ( IP == "" ) return NULL;
			Core::Debug_AddToLog( "Connecting (UDP) to:" );
			Core::Debug_AddToLog( IP.c_str() );

			Rocket::Network::PacketAccumulator * conn = new PacketAccumulator( ConnectionTypes::Connection_UDP, IP, port );
			Core::string IPandPort = (IP << ":" << port);
			m_UDP_connections[ IPandPort.std_str() ] = conn;
			return conn;
		}

		// Create a new TCP connection to the specified destination
		Rocket::Network::PacketAccumulator * Network::connect_TCP_IP4( Core::string host, unsigned int port ) {
			Core::string IP = hostLookup( host, port );
			if ( IP == "" ) return NULL;
			Core::Debug_AddToLog( "Connecting (TCP) to:" );
			Core::Debug_AddToLog( IP.c_str() );

			Rocket::Network::PacketAccumulator * conn = new PacketAccumulator( ConnectionTypes::Connection_TCP, IP, port );
			// Create new socket
			SOCKET * newSocket = new SOCKET();
			sockaddr_in target;
			target.sin_family = AF_INET;
			target.sin_port = htons( port );
			target.sin_addr.s_addr = inet_addr( IP.c_str() );

			*newSocket = socket( AF_INET, SOCK_STREAM, 0 );
			if ( *newSocket == INVALID_SOCKET ) {
				Core::Debug_AddToLog( "Error: Invalid socket when creating new TCP connection." );
				delete conn;
				delete newSocket;
				return NULL;
			}

			// Connect to IP:port with the new socket
			int result = connect( *newSocket, (const sockaddr *)&target, sizeof(sockaddr_in) );
			if ( result == SOCKET_ERROR ) {
				Core::Debug_AddToLog( "Error: Connect failed when creating new TCP connection." );
				delete conn;
				delete newSocket;
				return NULL;
			}

			Core::Debug_AddToLog( "TCP Connect Successful." );

			m_TCP_connections[ newSocket ] = conn;
			return conn;
		}

		// receive all data in the network buffers, and append it to the PacketAccumulators
		// send all packets in the queue of the PacketAccumulators
		// If this network is a server and a new TCP connection is accepted, update() will return a PacketAccumulator for that connection; otherwise, update() returns NULL
		PacketAccumulator * Network::update() {
			PacketAccumulator * newConnection = NULL;

			// Receive all data
			timeval timeout;
			timeout.tv_sec = m_updateTimeout / 1000;
			timeout.tv_usec = ( m_updateTimeout % 1000 ) * 1000;

			int maxFD = setFDs();

			int r = 0;
			if ( maxFD > 0 ) r = select( maxFD+1, &ReadFDs, NULL, NULL, &timeout );
			if ( r > 0 ) {
				// Receive on the UDP socket
				if ( m_settings & (int)NetworkSettings::UDP_Enabled ) {
					if ( FD_ISSET( m_UDP_socket, &ReadFDs ) ) {
						receive_UDP( &m_UDP_socket );
					}
				}

				// Receive everything on all TCP sockets
				std::unordered_map< SOCKET*, PacketAccumulator* >::iterator iter;
				for ( iter = m_TCP_connections.begin(); iter != m_TCP_connections.end(); ) {
					SOCKET * s = (*iter).first;
					iter++;
					if ( FD_ISSET( *s, &ReadFDs ) ) {
						receive_TCP( s );
					}
				}

				// Accept incoming connections
				if ( m_settings & (int)NetworkSettings::TCP_ListeningEnabled ) {
					if ( FD_ISSET( m_TCP_listenSocket, &ReadFDs ) ) {
						// Accept the new connection
						SOCKET * acceptSocket = new SOCKET();
						sockaddr_in addr;
#ifdef OS_WINDOWS
						int addrlen = sizeof( sockaddr_in );
#else
						socklen_t addrlen = sizeof( sockaddr_in );
#endif
						*acceptSocket = accept( m_TCP_listenSocket, (struct sockaddr*)&addr, &addrlen );

						//unblock listenSocket
						//u_long unblocked = 1;
						//ioctlsocket( *acceptSocket, FIONBIO, &unblocked );

						if ( *acceptSocket != SOCKET_ERROR ) {
							Core::string addr_IP = "";
#ifdef OS_WINDOWS
							addr_IP << addr.sin_addr.S_un.S_un_b.s_b1 << "." << addr.sin_addr.S_un.S_un_b.s_b2 << "." << addr.sin_addr.S_un.S_un_b.s_b3 << "." << addr.sin_addr.S_un.S_un_b.s_b4;
#else
							addr_IP << ((addr.sin_addr.s_addr >> 24) & 0xff) << "." << ((addr.sin_addr.s_addr >> 16) & 0xff) << "." << ((addr.sin_addr.s_addr >> 8) & 0xff) << "." << (addr.sin_addr.s_addr & 0xff);
#endif
							newConnection = new PacketAccumulator( ConnectionTypes::Connection_TCP, addr_IP, ntohs( addr.sin_port ) );
							m_TCP_connections[ acceptSocket ] = newConnection;
							Core::Debug_AddToLog( "TCP Peer Connected." );
						}
					}
				}
			} else if ( r == 0 ) {
				// timeout
				Rocket::Core::Debug_AddToLog( "select() timed out" );
			} else {
#ifdef OS_WINDOWS
				int err = WSAGetLastError();
#else
				int err = errno;
#endif
				Rocket::Core::string err_s = "";
				err_s << err ;
				Rocket::Core::Debug_AddToLog( "Error: select() failed" );
				Rocket::Core::Debug_AddToLog( err_s.c_str() );
			}

			// Send all packets
			// Send on the UDP socket
			std::unordered_map< std::string, PacketAccumulator* >::iterator iter_UDP;
			for ( iter_UDP = m_UDP_connections.begin(); iter_UDP != m_UDP_connections.end(); iter_UDP++ ) {
				Packet * p = NULL;
				char * data;
				unsigned int size;
				while ( ( p = iter_UDP->second->toSocket() ) != NULL ) {
					sockaddr_in destination = iter_UDP->second->getDestination();
					p->out( data, size );
					sendto( m_UDP_socket, data, size, 0, (struct sockaddr*)&destination, sizeof(sockaddr_in) );
					delete p;
				}
			}

			// Send everything on all TCP sockets
			std::unordered_map< SOCKET*, PacketAccumulator* >::iterator iter_TCP;
			for ( iter_TCP = m_TCP_connections.begin(); iter_TCP != m_TCP_connections.end(); iter_TCP++ ) {
				Packet * p = NULL;
				char * data;
				unsigned int size;
				while ( ( p = iter_TCP->second->toSocket() ) != NULL ) {
					p->out( data, size );
					send( *(iter_TCP->first), data, size, 0 );
					delete p;
				}
			}

			return newConnection;
		}

		void Network::receive_UDP( SOCKET * s ) {
			char buffer[ NETWORK_PACKET_BUFFER_SIZE ];
			char ipstr[ INET6_ADDRSTRLEN ];
			memset( ipstr, 0, INET6_ADDRSTRLEN );
			int r;
			socklen_t addrlen;
			struct sockaddr_storage addr;

			// Receive data on this socket
			addrlen = sizeof( sockaddr );
			r = recvfrom( m_UDP_socket, buffer, NETWORK_PACKET_BUFFER_SIZE, 0, (sockaddr*)&addr, &addrlen );
			if ( addr.ss_family == AF_INET ) {
				inet_ntop( addr.ss_family, &(((struct sockaddr_in *)&addr)->sin_addr), ipstr, INET6_ADDRSTRLEN );
			} else {
				inet_ntop( addr.ss_family, &(((struct sockaddr_in6 *)&addr)->sin6_addr), ipstr, INET6_ADDRSTRLEN );
			}

			// Append port to ip to make search string
			Core::string fromip = ipstr;
			if ( fromip == "" ) fromip = "127.0.0.1";
			if ( addr.ss_family == AF_INET ) {
				fromip = fromip << ":" << ntohs( ((struct sockaddr_in *)&addr)->sin_port );
			} else {
				fromip = fromip << ":" << ntohs( ((struct sockaddr_in6 *)&addr)->sin6_port );
			}
			std::unordered_map< std::string, PacketAccumulator* >::iterator from = m_UDP_connections.find( fromip.std_str() );
			if ( from != m_UDP_connections.end() ) {
				// Add buffer to PacketAccumulator
				from->second->fromSocket( buffer, r );
			} else {
				// The packet is from an unknown source, so discard it
				Core::Debug_AddToLog( "Received packet from unknown UDP source: " );
				Core::Debug_AddToLog( fromip.c_str() );
				Core::Debug_AddToLog( "Discarding." );
			}
		}
		
		void Network::receive_TCP( SOCKET * s ) {
			char buffer[ NETWORK_PACKET_BUFFER_SIZE ];
			int r;
			r = recv( *s, buffer, NETWORK_PACKET_BUFFER_SIZE, 0 );

			if ( r > 0 ) {
				// Pass data to PacketAccumulator
				std::unordered_map< SOCKET*, PacketAccumulator* >::iterator iter = m_TCP_connections.find( s );
				if ( iter != m_TCP_connections.end() ) {
					iter->second->fromSocket( buffer, r );
				} else {
					// The packet is from an unregistered socket, so discard it
					Core::Debug_AddToLog( "Received packet on unregistered TCP socket. Discarding." );
				}
			} else {
				if ( r == 0 ) {
					// Connection closed, so remove it from the list of TCP connections
					close_TCP( s );
				} else {
					Core::Debug_AddToLog( "Error: TCP recv() failed." );
				}
			}
		}

		void Network::close_TCP( SOCKET * socket ) {
			std::unordered_map< SOCKET*, PacketAccumulator* >::iterator iter = m_TCP_connections.find( socket );
			if ( iter != m_TCP_connections.end() ) {
				closeSocket( iter->first );
				delete iter->first;
				//delete iter->second;
				m_TCP_connections.erase( iter );
			} else {
				// socket is unregistered
				Core::Debug_AddToLog( "Error: Could not close unregistered TCP socket." );
			}
		}

		void Network::closeSocket( SOCKET * socket ) {
#ifdef OS_WINDOWS
				closesocket( *socket );
#else
				close( *socket );
#endif
		}

		// Open and close ports starting with startingPort until an open port is found, but only test up to PORT_TRY_COUNT ports.
		unsigned int Network::findOpenPort( unsigned int startingPort, unsigned int numberOfTries ) {
			int result, result2;
			unsigned int returnPort = startingPort;

			for ( unsigned int port = returnPort; port < returnPort + numberOfTries; port++ ) {
				Core::string returnPortStr = Core::string::tostring( port );

				// Test both TCP and UDP bindings
				SOCKET testSocket_TCP = socket( AF_INET, SOCK_STREAM, 0 );
				if(testSocket_TCP == INVALID_SOCKET) Core::Debug_AddToLog( "Error: Invalid testSocket_TCP on creation." );
				SOCKET testSocket_UDP = socket( AF_INET, SOCK_DGRAM, 0 );
				if(testSocket_UDP == INVALID_SOCKET) Core::Debug_AddToLog( "Error: Invalid testSocket_UDP on creation." );

				SOCKADDR_IN serverInfo;
				serverInfo.sin_family = AF_INET;
				serverInfo.sin_addr.s_addr = INADDR_ANY;
				serverInfo.sin_port = htons( port );

				result = bind( testSocket_TCP, (LPSOCKADDR)&serverInfo, sizeof(sockaddr_in) );
				closeSocket( &testSocket_TCP );
				result2 = bind( testSocket_UDP, (LPSOCKADDR)&serverInfo, sizeof(sockaddr_in) );
				closeSocket( &testSocket_UDP );
				if ( ( result != SOCKET_ERROR ) && ( result2 != SOCKET_ERROR ) ) {
					return port;
				}
			}

			// No open ports were found within the given range
			return 0;
		}

		int Network::setFDs() {
			FD_ZERO( &ReadFDs );
			//FD_ZERO( &WriteFDs );
			//FD_ZERO( &ExceptFDs );

			// Find the maxFD
			int maxFD = 0;
			// Check the listen socket (and FD_SET it) if this network has TCP enabled
			if ( m_settings & (int)NetworkSettings::TCP_ListeningEnabled ) {
				if ( m_TCP_listenSocket != INVALID_SOCKET ) {
					FD_SET( m_TCP_listenSocket, &ReadFDs );
					//FD_SET( m_TCP_listenSocket, &ExceptFDs );
					if ( m_TCP_listenSocket > (SOCKET)maxFD ) maxFD = m_TCP_listenSocket;
				}
			}
			// Check the UDP socket (and FD_SET it) if this network has UDP enabled
			if ( m_settings & (int)NetworkSettings::UDP_Enabled ) {
				if ( m_UDP_socket != INVALID_SOCKET ) {
					FD_SET( m_UDP_socket, &ReadFDs );
					if ( m_UDP_socket > (SOCKET)maxFD ) maxFD = m_UDP_socket;
				}
			}

			// Check all connections and FD_SET them
			if ( m_settings & (int)NetworkSettings::TCP_Enabled ) {
				// Add TCP client connections
				std::unordered_map< SOCKET*, PacketAccumulator* >::iterator iter;
				for ( iter = m_TCP_connections.begin(); iter != m_TCP_connections.end(); iter++ ) {
					int fd = *((*iter).first);
					FD_SET( fd, &ReadFDs );
					if ( fd > maxFD ) maxFD = fd;
				}
			}

			return maxFD;
		}


	}
}