
#include "rocket/UnitTest.h"

#include "rocket/Core/debug.h"

#include "Network.h"

Rocket_UnitTest ( Network_TCP ) {
	// Setup local server
	Rocket::Network::Network * server = new Rocket::Network::Network( (int)Rocket::Network::NetworkSettings::TCP_ListeningEnabled | (int)Rocket::Network::NetworkSettings::TCP_Enabled, 1000 );
	unsigned int server_port = server->setupTCP_listen( 1234, 100 );

	// Connect client to server
	Rocket::Network::Network * client = new Rocket::Network::Network( (int)Rocket::Network::NetworkSettings::TCP_Enabled, 1000 );
	Rocket::Network::PacketAccumulator * client_acc = client->connect_TCP_IP4( "127.0.0.1", server_port );
	Rocket_UnitTest_Check_Expression( client_acc != NULL );

	// Server: accept client connection
	Rocket::Network::PacketAccumulator * server_acc = server->update();
	Rocket_UnitTest_Check_Expression( server_acc != NULL );

	// Client: send data
	Rocket::Network::Packet * p = new Rocket::Network::Packet( Rocket::Network::PacketTypes::Test );
	p->add( "Hello server!" );
	p->add( (Rocket::Core::fixedpoint)0.5f );
	p->add( (Rocket::Core::fixedpoint)0.0f );
	p->add( (Rocket::Core::fixedpoint)-1.1f );
	p->add( (int)-5 );
	client_acc->send( p );
	client->update();

	// Server: receive data
	server->update();
	Rocket::Network::Packet * p2 = server_acc->receive();
	Rocket_UnitTest_Check_Expression( p2 != NULL );

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
	Rocket::Network::Network * sender = new Rocket::Network::Network( (int)Rocket::Network::NetworkSettings::UDP_Enabled, 100 );
	unsigned int sender_port = sender->setupUDP( 1234, 100 );

	// Setup receiver
	Rocket::Network::Network * receiver = new Rocket::Network::Network( (int)Rocket::Network::NetworkSettings::UDP_Enabled, 100 );
	unsigned int receiver_port = receiver->setupUDP( 1234, 100 );

	// Sender: get ready to send to destination
	Rocket::Network::PacketAccumulator * sender_acc = sender->connect_UDP_IP4( "127.0.0.1", receiver_port );
	Rocket_UnitTest_Check_Expression( sender_acc != NULL );

	// Receiver: get ready to receive from the sender
	Rocket::Network::PacketAccumulator * receiver_acc = receiver->connect_UDP_IP4( "127.0.0.1", sender_port );
	Rocket_UnitTest_Check_Expression( receiver_acc != NULL );

	// Sender: send UDP packet
	Rocket::Network::Packet * p = new Rocket::Network::Packet( Rocket::Network::PacketTypes::Test );
	p->add( "Hello Receiver!" );
	p->add( (Rocket::Core::fixedpoint)12345.0f );
	p->add( (Rocket::Core::fixedpoint)-987654321.0f );
	p->add( (Rocket::Core::fixedpoint)0.0f );
	p->add( (int)0 );
	sender_acc->send( p );
	sender->update();

	// Receiver: get incoming packet
	receiver->update();
	Rocket::Network::Packet * p2 = receiver_acc->receive();
	Rocket_UnitTest_Check_Expression( p2 != NULL );

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
	Rocket::Network::Network * web = new Rocket::Network::Network( (int)Rocket::Network::NetworkSettings::TCP_Enabled, 1000 );
	Rocket::Core::string hostname = "www.jasonbcox.com";
	Rocket::Network::PacketAccumulator * acc = web->connect_TCP_IP4( hostname, 80 );
	Rocket_UnitTest_Check_Expression( acc != NULL );

	// Formulate the HTTP request and send it
	Rocket::Network::Packet * httpRequest = new Rocket::Network::Packet( Rocket::Network::PacketTypes::Typeless );
	Rocket::Core::string request = "GET /Portfolio/projects/programming/moo/moo.h HTTP/1.0\r\n";
	request << "Host: " << hostname << "\r\n";
	request << "Connection: close\r\n";
	request << "\r\n\r\n";
	httpRequest->add( (char*)request.c_str(), request.length() );
	Rocket::Core::Debug_AddToLog( "HTTP Request:" );
	Rocket::Core::Debug_AddToLog( request.c_str() );
	acc->send( httpRequest );
	web->update();

	// Get response (wait 5 seconds)
	for ( int x = 0; x < 5; x++ ) { web->update(); }
	//Rocket::Network::Packet * response = acc->receive();
	char * resp;
	unsigned int size;
	acc->getBuffer( resp, size );
	Rocket_UnitTest_Check_Expression( size > 0 );

	if ( size > 2048 ) size = 2048;
	char * response = new char[size+2];
	memcpy( response, resp, size );
	response[size] = '\n';
	response[size+1] = '\0';

	Rocket::Core::Debug_AddToLog( "HTTP Response:" );
	Rocket::Core::Debug_AddToLog( response );

	acc->shiftBuffer( size );

	Rocket::Core::Debug_PrintLogView();
}