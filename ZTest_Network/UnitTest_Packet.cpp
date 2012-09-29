
#include "UnitTest.h"

#include "Packet.h"

Rocket_UnitTest ( Packet_Simple ) {
	Rocket::Network::Packet * p1 = new Rocket::Network::Packet( Rocket::Network::PacketTypes::Test );

	p1->add( "Hello\nPacket Test" );
	p1->add( (Rocket::Core::floap)0.1f );
	p1->add( (Rocket::Core::floap)99.9f );
	p1->add( (Rocket::Core::floap)-1337.0f );
	p1->add( -1 );

	char * data;
	unsigned int size;
	p1->out( data, size );

	Rocket::Network::Packet * p2 = new Rocket::Network::Packet( data, size );
	Rocket_UnitTest_Check_CharStringEqual( p2->getString().c_str(), "Hello\nPacket Test" );
	Rocket_UnitTest_Check_FloatEqual( p2->getFloap().toValue(), 0.1f, 0.001f );
	Rocket_UnitTest_Check_FloatEqual( p2->getFloap().toValue(), 99.9f, 0.001f );
	Rocket_UnitTest_Check_FloatEqual( p2->getFloap().toValue(), -1337.0f, 0.001f );
	Rocket_UnitTest_Check_Equal( p2->getInt(), -1 );
}