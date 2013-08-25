
#include "../Core/debug.h"
#include "Packet.h"

namespace Rocket {
	namespace Network {

		// --------------------------------------------------------------------------------------------------------------------
		// Packet Constructors/Destructors
		// --------------------------------------------------------------------------------------------------------------------
		void Packet::init() {
			m_data = nullptr;
			m_size = 0;
			m_maxsize = 0;
			m_seek = 0;

			m_nestedElements = 0;

			m_current_element = 0;
		}

		Packet::Packet( PacketTypes type, bool explicitPacketElements ) {
			init();
			m_type = type;
			m_explicitPacketElements = explicitPacketElements;

			// Selects the appropriate element list to make sure only the right elements added
			switch( type ) {
			case PacketTypes::Test :
				c_element_list = (PacketElementTypes*)(&Packet_Test);
				break;

			default:
				m_type = PacketTypes::Typeless;
				c_element_list = nullptr;
			}

			if ( m_type != PacketTypes::Typeless ) {
				// add a placeholder for the packet size
				unsigned int placeholder = 0;
				add( (char*)(&placeholder), PACKET_INT_SIZE );	// use add( c, size ) instead of add( unsigned int ) so that packet element checking is bypassed
				// add the packet type to the data
				add( (char*)(&m_type), 1 );
			}
		}

		Packet::Packet( char * data, unsigned int size, bool explicitPacketElements ) {
			init();
			m_explicitPacketElements = explicitPacketElements;
			add( data, size );

			// Get the packet size
			unsigned int packet_size = getUInt();
			//if ( packet_size != size ) error();
			// Get the packet type
			char byte = getByte();
			// Selects the appropriate element list to make sure only the right elements read
			switch ( byte ) {
				case (int)PacketTypes::Test :
					m_type = PacketTypes::Test;
					c_element_list = (PacketElementTypes*)(&Packet_Test);
					break;

				default:
					m_type = PacketTypes::Typeless;
					c_element_list = nullptr;
			}
		}

		Packet::~Packet() {
			delete [] m_data;
		}


		// --------------------------------------------------------------------------------------------------------------------
		// Add elements to the packet
		// --------------------------------------------------------------------------------------------------------------------

		unsigned int Packet::getPacketSize() {
			return m_size;
		}

		void Packet::setPacketSize( unsigned int size ) {
			if ( size > 0 ) {
				m_maxsize = size;
				char * olddata = m_data;
				m_data = new char[ size ];

				if ( m_size > size ) m_size = size;
				if ( olddata != nullptr ) {
					memcpy( m_data, olddata, m_size );
					delete [] olddata;
				}
			}
		}

		
		
		// add( c, size ) appends the array of bytes to the end of this packet
		// If the packet's memory allocation isn't large enough, more space is allocated to fit the new data
		// (Tip: For increased efficiency, call setPacketSize() prior to adding packet elements
		void Packet::add( char * c, unsigned int size ) {
			// append c to m_data
			if ( size > 0 ) {
				if ( m_size + size > m_maxsize ) setPacketSize( m_size + size );
				memcpy( &(m_data[ m_size ]), c, size );
				m_size += size;
			}
		}

		// All add( * ) functions should call add( c, size ) to add the data to the packet
		// add( * ) adds the specified element only if the element type matches the next expected type in the element list
		//											OR the element is a nested part of a larger type ( add( * ) is called within nestedElement() )

		void Packet::add( int i ) {
			PacketElementTypes type = PacketElementTypes::raw_int;
			if ( nextElementMatches( type ) ) {
				if ( m_explicitPacketElements == true ) add( (char*)(&type), 1 );

				int nbo_i = htonl( i );
				add( (char*)(&nbo_i), PACKET_INT_SIZE );

				( m_nestedElements == 0 ) ? m_current_element++ : m_nestedElements--;
			}
		}

		void Packet::add( unsigned int u ) {
			PacketElementTypes type = PacketElementTypes::raw_uint;
			if ( nextElementMatches( type ) ) {
				if ( m_explicitPacketElements == true ) add( (char*)(&type), 1 );

				int nbo_u = htonl( u );
				add( (char*)(&nbo_u), PACKET_INT_SIZE );

				( m_nestedElements == 0 ) ? m_current_element++ : m_nestedElements--;
			}
		}

		void Packet::add( fixedpoint f ) {
			PacketElementTypes type = PacketElementTypes::raw_fixedpoint;
			if ( nextElementMatches( type ) ) {
				if ( m_explicitPacketElements == true ) add( (char*)(&type), 1 );
				
				fswap cf;
				cf.f = f.toValue();
				int nbo_i = htonl( cf.i );
				add( (char*)(&nbo_i), PACKET_INT_SIZE );

				( m_nestedElements == 0 ) ? m_current_element++ : m_nestedElements--;
			}
		}

		void Packet::add( rstring s ) {
			PacketElementTypes type = PacketElementTypes::char_string;
			if ( nextElementMatches( type ) ) {
				if ( m_explicitPacketElements == true ) add( (char*)(&type), 1 );

				unsigned int s_length = s.length();

				// add string length
				nestedElement( add( s_length ) );

				// add string data
				char * c = (char*)s.c_str();
				add( c, s_length );

				( m_nestedElements == 0 ) ? m_current_element++ : m_nestedElements--;
			}
		}

		// --------------------------------------------------------------------------------------------------------------------
		// Read elements from the packet
		// --------------------------------------------------------------------------------------------------------------------
		char Packet::getByte() {
			m_seek++;
			if ( m_seek < m_size ) {
				return m_data[ m_seek - 1 ];
			} else {
				return 0;
			}
		}

		int Packet::getInt() {
			char type = 0;
			if ( ( m_explicitPacketElements == false ) || (nextElementMatches( (PacketElementTypes)(type = getByte()) )) ) {
				unsigned int size = PACKET_INT_SIZE;
				int i = 0;
				memcpy( &i, &(m_data[m_seek]), size );
				i = ntohl( i );

				m_seek += size;
				( m_nestedElements == 0 ) ? m_current_element++ : m_nestedElements--;
				return i;
			} else {
				return 0;
			}
		}

		unsigned int Packet::getUInt() {
			char type = 0;
			if ( ( m_explicitPacketElements == false ) || (nextElementMatches( (PacketElementTypes)(type = getByte()) )) ) {
				unsigned int size = PACKET_INT_SIZE;
				unsigned int u = 0;
				memcpy( &u, &(m_data[m_seek]), size );
				u = ntohl( u );

				m_seek += size;
				( m_nestedElements == 0 ) ? m_current_element++ : m_nestedElements--;
				return u;
			} else {
				return 0;
			}
		}

		fixedpoint Packet::getfixedpoint() {
			char type = 0;
			if ( ( m_explicitPacketElements == false ) || (nextElementMatches( (PacketElementTypes)(type = getByte()) )) ) {
				fswap cf;
				int nbo_i;
				memcpy( &nbo_i, &(m_data[m_seek]), PACKET_INT_SIZE );
				cf.i =  ntohl( nbo_i );
				FixedPoint_OutputType f = cf.f;

				m_seek += PACKET_INT_SIZE;
				( m_nestedElements == 0 ) ? m_current_element++ : m_nestedElements--;
				return f;
			} else {
				return 0;
			}
		}

		rstring Packet::getString() {
			char type = 0;
			if ( ( m_explicitPacketElements == false ) || (nextElementMatches( (PacketElementTypes)(type = getByte()) )) ) {
				// get string length
				unsigned int s_length;
				nestedElement( s_length = getUInt() );

				// get string data
				char * c = new char[ s_length + 1 ];
				memcpy( c, &(m_data[m_seek]), s_length );
				c[ s_length ] = '\0';

				m_seek += s_length;
				rstring r = c;
				delete [] c;

				( m_nestedElements == 0 ) ? m_current_element++ : m_nestedElements--;
				return r;
			} else {
				return "";
			}
		}

		// --------------------------------------------------------------------------------------------------------------------
		// Other functions
		// --------------------------------------------------------------------------------------------------------------------

		// Returns the packet data blob for sending
		void Packet::out( char *& data, unsigned int & size ) {
			if ( m_type != PacketTypes::Typeless ) setPacketSizeInData();
			data = m_data;
			size = m_size;
		}

		// Returns true if type matches the next expected packet element type
		bool Packet::nextElementMatches( PacketElementTypes type ) {
			if ( c_element_list == nullptr ) return true;
			if ( m_nestedElements > 0 ) return true;
			if ( c_element_list[ m_current_element ] == PacketElementTypes::empty ) return false;
			if ( c_element_list[ m_current_element ] == type ) return true;
			return false;
		}

		// Overwrite the placeholder for the packet's size
		void Packet::setPacketSizeInData() {
			unsigned int nbo_size = htonl( m_size );
			memcpy( &(m_data[ 0 ]), &nbo_size, PACKET_INT_SIZE );
		}

	}
}