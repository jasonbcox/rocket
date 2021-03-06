
#include "Network.h"

namespace Rocket {
	namespace Network {

		PacketAccumulator::PacketAccumulator( ConnectionTypes protocol, rstring IP, unsigned int port ) {
			m_protocol = protocol;
			m_destination_IP = IP;
			m_destination_port = port;

			m_packets_buffer = new char[ NETWORK_BUFFER_SIZE ];
			m_packets_buffer_index = 0;
		}

		PacketAccumulator::~PacketAccumulator() {
			for ( auto inboundPacket : m_packets_inbound ) {
				delete inboundPacket;
			}
			for ( auto outboundPacket : m_packets_outbound ) {
				delete outboundPacket;
			}
			delete [] m_packets_buffer;
		}

		// Queue a packet for sending
		void PacketAccumulator::send( Packet * p ) {
			m_packets_outbound.push_back( p );
		}

		// Return the next available packet on the receiving queue, or nullptr if there is none
		Packet * PacketAccumulator::receive() {
			if ( m_packets_inbound.size() > 0 ) {
				Packet * r = m_packets_inbound[0];
				m_packets_inbound.pop_front();
				return r;
			} else {
				return nullptr;
			}
		}

		// Read directly from the buffer (not recommended)
		void PacketAccumulator::getBuffer( char *& buffer, unsigned int & size ) {
			buffer = m_packets_buffer;
			size = m_packets_buffer_index;
		}

		// Erase shift number of bytes from the buffer and shift the remaining bytes to be in their place
		void PacketAccumulator::shiftBuffer( unsigned int shift ) {
			if ( shift > 0 ) {
				m_packets_buffer_index -= shift;
				memmove( m_packets_buffer, &(m_packets_buffer[ shift ]), m_packets_buffer_index );
			}
		}

		// Takes in data from a socket recv() and appends it to the temporary buffer
		// When a packet is complete, that data is popped off, formed into a Packet, and added to packets_inbound
		void PacketAccumulator::fromSocket( char * inbound, unsigned int size ) {
			// append inbound data to the buffer
			if ( size > 0 ) {
				memcpy( &(m_packets_buffer[ m_packets_buffer_index ]), inbound, size );
				m_packets_buffer_index += size;
			}
			
			// check to see if the packet is complete by checking the packet size and the amount of data in the buffer
			unsigned int packet_size = 0;
			memcpy( &packet_size, m_packets_buffer, PACKET_INT_SIZE );
			packet_size = ntohl( packet_size );
			if ( packet_size <= m_packets_buffer_index ) {
				// create new Packet and queue it up for reading
				Packet * newPacket = new Packet( m_packets_buffer, packet_size );
				m_packets_inbound.push_back( newPacket );

				shiftBuffer( packet_size );
			}
		}

		// toSocket() returns the next packet ready for sending across the socket, or nullptr if there are no packets to send
		Packet * PacketAccumulator::toSocket() {
			if ( m_packets_outbound.size() > 0 ) {
				Packet * r = m_packets_outbound[0];
				m_packets_outbound.pop_front();
				return r;
			} else {
				return nullptr;
			}
		}

		sockaddr_in PacketAccumulator::getDestination() {
			sockaddr_in r;
			r.sin_family = AF_INET;
			r.sin_port = htons( m_destination_port );
			r.sin_addr.s_addr = inet_addr( m_destination_IP.c_str() );
			memset( &(r.sin_zero), 0, 8 );
			return r;
		}

	}
}