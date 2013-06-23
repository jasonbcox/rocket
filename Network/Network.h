
#ifndef Rocket_Network_Network_H
#define Rocket_Network_Network_H

#include <unordered_map>
#include <string>
#include <deque>
#include <string.h>

#include "rocket/Core/system.h"
#include "rocket/Core/string_.h"

#ifdef OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef int SOCKET;
const int INVALID_SOCKET = -1;
const int SOCKET_ERROR = -1;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr* LPSOCKADDR;
#endif

#include "Packet.h"

namespace Rocket {
	namespace Network {

		/*static const Core::string	NETWORK_LISTEN_PORT				= "9001";
		static const int			NETWORK_NUMBER_OF_PORT_TRIES	= 99;

		static const int			NETWORK_RECEIVE_ALL_BUFFER_SIZE	= 1048576;

		static const int			NETWORK_LISTEN_TIMEOUT = 300;	//ms*/

		static const unsigned int	NETWORK_BUFFER_SIZE = 1048576;
		static const unsigned int	NETWORK_PACKET_BUFFER_SIZE = 4096;

		namespace NetworkSettings {
		enum NetworkSettings {
			UDP_Enabled = 1,
			TCP_Enabled = 2,
			TCP_ListeningEnabled = 4
		};

		enum ConnectionTypes {
			Connection_UDP = 1,
			Connection_TCP
		};
		}

		class PacketAccumulator;
		class Network {
		public:
			Network( int networkSettings, unsigned int updateTimeout );
			~Network();

			// UDP uses port for all communications
			unsigned int setupUDP( unsigned int port, unsigned int numberOfPortTries );
			// setupTCP_listen() is only necessary if listening for incoming connections
			// returns the port number being listened on
			unsigned int setupTCP_listen( unsigned int listenPort, unsigned int numberOfPortTries );

			Core::string hostLookup( Core::string host, unsigned int port );
			PacketAccumulator * connect_UDP_IP4( Core::string host, unsigned int port );
			PacketAccumulator * connect_TCP_IP4( Core::string host, unsigned int port );

			// todo: add close_UDP() after X time (because UDP connections don't "close")
			// close_TCP() does not delete PacketAccumulators, so they must be cleaned up by you
			void close_TCP( SOCKET * socket );

			// for receiving and sending packets and accepting incoming connections
			PacketAccumulator * update();

			static void closeSocket( SOCKET * socket );

			static unsigned int findOpenPort( unsigned int startingPort, unsigned int numberOfTries );

		private:
			int m_settings;

			unsigned int m_UDP_port;
			SOCKET m_UDP_socket;
			// the std::string is the IP and port of a connection with this format: 'IP.IP.IP.IP:PORT'
			std::unordered_map< std::string, PacketAccumulator* > m_UDP_connections;

			unsigned int m_TCP_listenPort;
			SOCKET m_TCP_listenSocket;
			std::unordered_map< SOCKET*, PacketAccumulator* > m_TCP_connections;

			void receive_UDP( SOCKET * s );
			void receive_TCP( SOCKET * s );

			unsigned int m_updateTimeout;

			fd_set ReadFDs, WriteFDs, ExceptFDs;
			int setFDs();
		};


		// All connections use a PacketAccumulator to administer inbound and outbound packets on that connection
		// Inbound packets are added as they come in
		// Outbound packets are sent at the next cycle
		class PacketAccumulator {
		public:
			PacketAccumulator( NetworkSettings::ConnectionTypes protocol, Core::string IP, unsigned int port );
			~PacketAccumulator();

			void send( Packet * p );	// queue a packet for sending
			Packet * receive();			// get the next packet that was queued on receive

			void getBuffer( char *& buffer, unsigned int & size );	// To read straight from the buffer
			void shiftBuffer( unsigned int shift );					// Erase shift bytes from the buffer

			sockaddr_in getDestination();

		private:
			friend Network;

			std::deque< Packet* > m_packets_outbound;
			std::deque< Packet* > m_packets_inbound;
			char * m_packets_buffer;
			unsigned int m_packets_buffer_index;

			NetworkSettings::ConnectionTypes m_protocol;
			Core::string m_destination_IP;
			unsigned int m_destination_port;

			// For sending and receiving bytes on the socket directly
			void fromSocket( char * inbound, unsigned int size );
			Packet * toSocket();
		};

	}
}

#endif