
#ifndef Rocket_Network_Packet_H
#define Rocket_Network_Packet_H

#include "system.h"
#include "string_.h"
#include "floap.h"

#ifdef OS_WINDOWS
#include <WinSock2.h>
#pragma comment (lib, "Ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#endif

#define PACKET_INT_SIZE 4

namespace Rocket {
	namespace Network {

		// fswap is used to attain byte access to a float
		union fswap {
			float f;
			int i;
		};

		namespace PacketElementTypes {
		// Base types denote how the data is stored:
			// char - readable ascii characters
			// raw - data type is its exact byte representation
			// comp - compressed data type
		// empty is NOT to be used as a packet element type and is there
		// only for processing purposes
		enum PacketElementTypes {
			empty = 0,
			char_string,
			raw_int,
			raw_uint,
			raw_floap
		};
		}

		namespace PacketTypes {
		// Typeless packets can only be created, not received (for special outbound circumstances)
		enum PacketTypes {
			Typeless = 0,
			Test
		};
		}

		// --------------------------------------------------------------------------------------------------------------------
		// Packet Types (warning: all arrays must end with empty!)
		// --------------------------------------------------------------------------------------------------------------------
		const PacketElementTypes::PacketElementTypes Packet_Test[] = {
			PacketElementTypes::char_string,
			PacketElementTypes::raw_floap,
			PacketElementTypes::raw_floap,
			PacketElementTypes::raw_floap,
			PacketElementTypes::raw_int,
			PacketElementTypes::empty
		};

		// --------------------------------------------------------------------------------------------------------------------
		// Packet
		// ------
		// Packet data bytes:
		// 0-3	:	unsigned int	packet size
		// 4	:	byte			packet type
		// 5+	:	...				packet data
		// --------------------------------------------------------------------------------------------------------------------
		class Packet {
		public:
			void init();
			// Create a new packet; it's type must be set (the type will be added to the packet automatically)
			Packet( PacketTypes::PacketTypes type, bool explicitPacketElements = false );
			// Receive a packet; the first byte is the packet type and the rest is data
			Packet( char * data, unsigned int size, bool explicitPacketElements = false );
			// explicitPacketElements is false by default.  This means that packet element types do not precede
			// each packet element within the packet.  If set to true, the byte preceding each packet element will
			// denote that element's type.
			~Packet();

			unsigned int getPacketSize();
			// The the maximum memory allocated for this packet.  Newly allocated space appended to the end of the packet is not zeroed out.
			// If the maximum is less that what is currently written, the data will be truncated.  
			void setPacketSize( unsigned int size );


			// Add raw data to the packet
			void add( char * c, unsigned int size );
			// Add elements to the packet
			void add( int i );
			void add( unsigned int u );
			void add( Core::floap f );			// idea: add bool flag for optional compression
			void add( Core::string s );


			// Read a single byte from the packet
			char getByte();
			// Read elements from the packet
			int getInt();
			unsigned int getUInt();
			Core::floap getFloap();
			Core::string getString();


			// Returns the packet data blob for sending
			void out( char *& data, unsigned int & size );


		private:
			PacketTypes::PacketTypes m_type;
			char * m_data;
			unsigned int m_size;		// amount of memory used for this packet
			unsigned int m_maxsize;		// maximum memory allocated for this packet

			// The index of the next packet element to be added or read
			unsigned int m_seek;
			// The elements expected to be in this packet
			// These values are set based on the packet type, and elements being added that
			// don't correspond to the expected types will not succeed
			unsigned int m_current_element;
			PacketElementTypes::PacketElementTypes * c_element_list;
			bool m_explicitPacketElements;

			// used to determine if a type is being used within a type
			// for example: char_string uses raw_uint to store its length, but that uint should
			// not be checked against the c_element_list, because it will not be there
			unsigned int m_nestedElements;

			// call nestedElement() on get*() when reading or add( * ) when writing a packet element that is embedded within another packet element
#define nestedElement( func ) do { m_nestedElements++; (func); } while(0)
			
			bool nextElementMatches( PacketElementTypes::PacketElementTypes type );

			void setPacketSizeInData();
		};

	}
}

#endif