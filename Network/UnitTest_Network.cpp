
#include "rocket/UnitTest.h"

#include "rocket/Core/debug.h"

#include "Network.h"

using namespace Rocket::Core;
using namespace Rocket::Network;

Rocket_UnitTest ( Network_TCP ) {
	// Setup local server
	Network * server = new Network( (int)NetworkSettings::TCP_ListeningEnabled | (int)NetworkSettings::TCP_Enabled, 1000 );
	unsigned int server_port = server->setupTCP_listen( 1234, 100 );

	// Connect client to server
	Network * client = new Network( (int)NetworkSettings::TCP_Enabled, 1000 );
	PacketAccumulator * client_acc = client->connect_TCP_IP4( "127.0.0.1", server_port );
	Rocket_UnitTest_Check_Expression( client_acc != nullptr );

	// Server: accept client connection
	PacketAccumulator * server_acc = server->update();
	Rocket_UnitTest_Check_Expression( server_acc != nullptr );

	// Client: send data
	Packet * p = new Packet( PacketTypes::Test );
	p->add( "Hello server!" );
	p->add( (fixedpoint)0.5f );
	p->add( (fixedpoint)0.0f );
	p->add( (fixedpoint)-1.1f );
	p->add( (int)-5 );
	client_acc->send( p );
	client->update();

	// Server: receive data
	server->update();
	Packet * p2 = server_acc->receive();
	Rocket_UnitTest_Check_Expression( p2 != nullptr );

	Rocket_UnitTest_Check_CharStringEqual( p2->getString().c_str(), "Hello server!" );
	Rocket_UnitTest_Check_FloatEqual( p2->getfixedpoint().toValue(), 0.5f, 0.001f );
	Rocket_UnitTest_Check_FloatEqual( p2->getfixedpoint().toValue(), 0.0f, 0.001f );
	Rocket_UnitTest_Check_FloatEqual( p2->getfixedpoint().toValue(), -1.1f, 0.001f );
	Rocket_UnitTest_Check_Equal( p2->getInt(), -5 );

	delete server;
	delete client;
}

Rocket_UnitTest ( Network_UDP ) {
	// Setup sender
	Network * sender = new Network( (int)NetworkSettings::UDP_Enabled, 100 );
	unsigned int sender_port = sender->setupUDP( 1234, 100 );

	// Setup receiver
	Network * receiver = new Network( (int)NetworkSettings::UDP_Enabled, 100 );
	unsigned int receiver_port = receiver->setupUDP( 1234, 100 );

	// Sender: get ready to send to destination
	PacketAccumulator * sender_acc = sender->connect_UDP_IP4( "127.0.0.1", receiver_port );
	Rocket_UnitTest_Check_Expression( sender_acc != nullptr );

	// Receiver: get ready to receive from the sender
	PacketAccumulator * receiver_acc = receiver->connect_UDP_IP4( "127.0.0.1", sender_port );
	Rocket_UnitTest_Check_Expression( receiver_acc != nullptr );

	// Sender: send UDP packet
	Packet * p = new Packet( PacketTypes::Test );
	p->add( "Hello Receiver!" );
	p->add( (fixedpoint)12345.0f );
	p->add( (fixedpoint)-987654321.0f );
	p->add( (fixedpoint)0.0f );
	p->add( (int)0 );
	sender_acc->send( p );
	sender->update();

	// Receiver: get incoming packet
	receiver->update();
	Packet * p2 = receiver_acc->receive();
	Rocket_UnitTest_Check_Expression( p2 != nullptr );

	Rocket_UnitTest_Check_CharStringEqual( p2->getString().c_str(), "Hello Receiver!" );
	Rocket_UnitTest_Check_FloatEqual( p2->getfixedpoint().toValue(), 12345.0f, 0.001f );
	Rocket_UnitTest_Check_FloatEqual( p2->getfixedpoint().toValue(), -987654321.0f, 0.001f );
	Rocket_UnitTest_Check_FloatEqual( p2->getfixedpoint().toValue(), -0.0f, 0.001f );
	Rocket_UnitTest_Check_Equal( p2->getInt(), 0 );

	delete sender;
	delete receiver;
}

Rocket_UnitTest ( Network_HTTP ) {
	// Connect to a test web server
	Network * web = new Network( (int)NetworkSettings::TCP_Enabled, 1000 );
	rstring hostname = "www.jasonbcox.com";
	PacketAccumulator * acc = web->connect_TCP_IP4( hostname, 80 );
	Rocket_UnitTest_Check_Expression( acc != nullptr );

	// Formulate the HTTP request and send it
	Packet * httpRequest = new Packet( PacketTypes::Typeless );
	rstring request = "GET /Portfolio/projects/programming/moo/moo.h HTTP/1.0\r\n";
	request << "Host: " << hostname << "\r\n";
	request << "Connection: close\r\n";
	request << "\r\n\r\n";
	httpRequest->add( (char*)request.c_str(), request.length() );
	//Debug_AddToLog( "HTTP Request:" );
	//Debug_AddToLog( request.c_str() );
	acc->send( httpRequest );
	web->update();

	// Get response (wait 5 seconds)
	for ( int x = 0; x < 5; x++ ) { web->update(); }
	//Packet * response = acc->receive();
	char * resp;
	unsigned int size;
	acc->getBuffer( resp, size );
	Rocket_UnitTest_Check_Expression( size > 0 );

	if ( size > 2048 ) size = 2048;
	char * response = new char[size+2];
	memcpy( response, resp, size );
	response[size] = '\n';
	response[size+1] = '\0';

	//Debug_AddToLog( "HTTP Response:" );
	//Debug_AddToLog( response );

	acc->shiftBuffer( size );

	//Debug_PrintLogView();
}
