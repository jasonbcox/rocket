
// Removes security warnings for deprecated functions such as fopen()
#define _CRT_SECURE_NO_DEPRECATE

#include "rocket/Core/timer.h"

#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "rocket/Core/debug.h"

#include "rocket/UnitTest.h"
#include "Universe.h"
#include "Input.h"
#include "Object_Newton.h"
#include "ShaderDefaults.h"
#include "Sprite.h"
#include "Object_BitmapText.h"

using namespace Rocket;
using namespace Rocket::Core;
using namespace Rocket::Graphics;

//int main() {
Rocket_UnitTest ( all_Graphics ) {

	// Setup GLFW Window
	GLFWwindow * window;
	if( !glfwInit() ) exit( EXIT_FAILURE );
	// Require OpenGL 3.0 or higher
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow( Graphics::WindowWidth,Graphics::WindowHeight, "ROCKET Engine - Graphics Module", NULL, NULL );
	//if( !glfwOpenWindow( Graphics::WindowWidth,Graphics::WindowHeight, 0,0,0, 0,0,0, GLFW_WINDOW ) ) { //GLFW_WINDOW, GLFW_FULLSCREEN
	if ( !window ) {
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	GL_GET_ERROR();
	glfwMakeContextCurrent( window );

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		//return 1;
		m_result = Rocket::Test::Test_Fail; return;
	}
	GL_GET_ERROR();
	glClearColor( 0.3f, 0.3f, 0.3f, 1.0f );

	// Create an input monitor
	Input * input = new Input( window );

	// Create a new Universe and Scenes to go with it
	Universe * world = new Universe();
	Scene * mainScene = new Scene( 63.0f, (Graphics::WindowWidth * 1.0f)/(Graphics::WindowHeight * 1.0f), 1.0f, 1000.0f );
	world->addRenderPass( mainScene );
	Scene * hudScene = new Scene( Graphics::WindowWidth * 1.0f, Graphics::WindowHeight * 1.0f );
	world->addRenderPass( hudScene );

	// Load shaders
	ShaderDefaults::Shader_Texture * shader_texture = new ShaderDefaults::Shader_Texture( "../../Graphics/Shaders/texture_v.glsl", "../../Graphics/Shaders/texture_f.glsl" );
	world->addShader( "texture", shader_texture );

	// Load meshes
	Mesh * testMesh = generatePrimitive_Quad( world, "testMesh", shader_texture );
	mainScene->addMesh( testMesh );

	//Mesh * testCubeMesh = generatePrimitive_Cube( shader_texture );
	Mesh * testCubeMesh = world->loadMesh( "human", "../../Graphics/Shaders/human.obj", shader_texture ); //new Mesh( "../../Graphics/Shaders/human.obj", shader_texture );
	mainScene->addMesh( testCubeMesh );

	Mesh * testCubeMesh2 = generatePrimitive_Cube( world, "testCubeMesh", shader_texture );
	hudScene->addMesh( testCubeMesh2 );

	Mesh * testPlanetMesh = world->loadMesh( "planet", "../../Graphics/Shaders/StaticPlanet1.obj", shader_texture ); //new Mesh ( "../../Graphics/Shaders/StaticPlanet1.obj", shader_texture );
	mainScene->addMesh( testPlanetMesh );

	// Load textures and objects

	// Yellow smiley-face glass pane
	Object_Newton * testObject = new Object_Newton( testMesh );
	Texture * testTexture = world->loadTexture( "test_happyface", "../../Graphics/Shaders/testTexture1.bmp", true, false );
	ShaderDefaults::setObjectUniforms_texture( testObject,
		Shader_UniformTexture( 0, testTexture, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE ),
		vec2( 1.0f, 1.0f ), vec2( 0.0f, 0.0f ), vec3( 0.99f, 0.98f, 0.97f ), true, 0.0f, 0.5f );
	mainScene->addObject( testObject, NULL );
	testObject->setVelocity( vec3(0,0.1f,0) );
	//testObject->setAngularAcceleration( vec3(0,0.00003f,0) );

	// Test Planet
	Object * testPlanet = new Object( testPlanetMesh );
	Texture * testPlanetTexture = world->loadTexture( "test_planet", "../../Graphics/Shaders/StaticPlanet1.png", true, true );
	ShaderDefaults::setObjectUniforms_texture( testPlanet,
		Shader_UniformTexture( 0, testPlanetTexture, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE ),
		vec2( 1.0f, 1.0f ), vec2( 0.0f, 0.0f ), vec3( 1.0f, 0.8f, 0.5f ), false, 0.0f, 1.0f );
	mainScene->addObject( testPlanet, NULL );
	testPlanet->scale( vec3( 10.0f, 10.0f, 10.0f ) );
	testPlanet->position( vec3( -500.0f, 300.0f, -1000.0f ) );
	testPlanet->rotatePitch( MathConstants::PI / 8.0f );
	testPlanet->rotateYaw( MathConstants::PI / 6.0f );

	// Blue test cube
	Object * testCube = new Object( testCubeMesh );
	Texture * testHumanTexture = world->loadTexture( "test_human", "../../Graphics/Shaders/human1.png", true, true );
	ShaderDefaults::setObjectUniforms_texture( testCube,
		Shader_UniformTexture( 0, testHumanTexture, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE ),
		vec2( 1.0f, 1.0f ), vec2( 0.0f, 0.0f ), vec3( 1.0f, 1.0f, 1.0f ), false, 0.0f, 1.0f );
	mainScene->addObject( testCube, NULL );

	testObject->position( vec3(0,0,-2) );
	testObject->scale( vec3(3,3,3) );
	testCube->position( vec3(0,0,-5) );
	testCube->scale( vec3(2,2,2) );

	// Clone the yellow glass pane and make the clone red
	Object_Newton * testLimb = testObject->cloneNewton();
	//*(testLimb->getUniformVec3( "uniform_color" )) = vec3( 1.0f, 0.0f, 0.0f );
	((ShaderDefaults::ShaderUniforms_Texture*)(testLimb->getShaderUniforms()))->m_color = vec3( 1.0f, 0.0f, 0.0f );
	mainScene->addObject( testLimb, testObject );
	testLimb->position( vec3( 0,0.75,0 ) );
	testLimb->rotateYaw( Rocket::MathConstants::PI / 4.0f );
	testObject->rotateRoll( Rocket::MathConstants::PI / 4.0f );
	//testLimb->hide();
	testLimb->setVelocity( vec3(0,0,0.001f) );

	// make test clones
	for ( int c = 0; c < 1000; c++ ) {
		//Object_Newton * tempClone = testLimb->cloneNewton();
		Object * tempClone = testCube->clone();
		mainScene->addObject( tempClone, testObject );
		tempClone->position( vec3(c*1.0f+1.0f,0,0) );
		tempClone->scale( vec3(0.1f,0.1f,0.1f) );
	}

	// Green "selector" test cube
	Object * pickCube = testCube->clone();
	//*(pickCube->getUniformVec3( "uniform_color" )) = vec3( 0.0f, 1.0f, 0.0f );
	((ShaderDefaults::ShaderUniforms_Texture*)(pickCube->getShaderUniforms()))->m_color = vec3( 0.0f, 1.0f, 0.0f );
	pickCube->scale( vec3( 0.03f,0.03f,0.03f ) );
	pickCube->position( vec3( 0,0,0 ) );
	mainScene->addObject( pickCube, NULL );
	pickCube->hide();

	// Red HUD cube
	Object * hudCube = new Object( testCubeMesh2 );
	ShaderDefaults::setObjectUniforms_texture( hudCube,
		Shader_UniformTexture( 0, testTexture, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE ),
		vec2( 1.0f, 1.0f ), vec2( 0.0f, 0.0f ), vec3( 1.0f, 0.0f, 0.0f ), false, 0.0f, 1.0f );
	hudScene->addObject( hudCube, NULL );
	//hudCube->position( vec3(-430,290,0) );
	hudCube->position( vec3(100,100,0) );
	hudCube->scale( vec3(100,100,100) );
	hudCube->rotateYaw( Rocket::MathConstants::PI / 4.0f );
	hudCube->rotatePitch( Rocket::MathConstants::PI / 4.0f );

	Sprite::enableSpritesInScene( world, hudScene, shader_texture );
	Texture * testTexture2 = world->loadTexture( "test_happyface2", "../../Graphics/Shaders/testTexture1.bmp", false, true );
	Sprite * testSprite = new Sprite( testTexture2, 64, 64 );
	hudScene->addObject( testSprite, NULL );
	testSprite->setAngle( Rocket::MathConstants::PI / 4.0f );
	testSprite->setUV(4,4,40,40);

	Texture * testBitmapTexture = world->loadTexture( "test_font", "../../Graphics/Shaders/testFont1.png", false, true );
	Object_BitmapText * testBitmap = new Object_BitmapText( testBitmapTexture, "The quick, brown fox jumped over the lazy dog.\nTHE QUICK BROWN FOX JUMPED OVER THE LAZY DOG!\n0123456789?" );
	hudScene->addObject( testBitmap, NULL );
	testBitmap->enableTransparency( 0.0f, 1.0f );
	testBitmap->setPosition( vec2i( 0, 200 ) );

	// Main Loop
	input->hideMouse();
	input->lockMouse( Graphics::WindowWidth/2, Graphics::WindowHeight/2 );

	mainScene->setCameraInput( input );
	mainScene->setCameraControl_Move( 'W', 'S' );
	mainScene->setCameraControl_Strafe( 'A', 'D' );
	mainScene->setCameraControl_Elevate( 'R', 'F' );

	mainScene->setCameraControl_Turn( GLFW_KEY_LEFT, GLFW_KEY_RIGHT );
	mainScene->setCameraControl_Pitch( GLFW_KEY_UP, GLFW_KEY_DOWN );

	const float Camera_Move_Speed = 0.01f;	// units/ms
	const float Camera_Turn_Speed = 0.005f;	// rads/ms
	mainScene->setCameraMoveSpeed( vec3( Camera_Move_Speed, Camera_Move_Speed, Camera_Move_Speed ) );
	mainScene->setCameraTurnSpeed( vec3( Camera_Turn_Speed, Camera_Turn_Speed, Camera_Turn_Speed ) );

	unsigned long tick_timer = Core::timer();
	int running = 1;
	while( running ) {
		//Core::Debug_StartTimer( "MAIN" );
		unsigned long elapsed = Core::elapsedtime( tick_timer );
		tick_timer = Core::timer();
		if ( elapsed == 0 ) elapsed = 1;
		//std::cout << "FPS: " << (int)(1000.0 / (elapsed*1.0)) << "\n";
		Core::string showFPS = "FPS: ";
		showFPS << (int)(1000.0 / (elapsed*1.0));
		Core::Debug_AddToLog( showFPS.c_str() );


		mainScene->ControlCamera( (float)elapsed );


		// orbit glass panes around blue cube
		vec3 orbitForce = Rocket::Core::normalize( testCube->positionWorld() - testObject->positionWorld() );
		testObject->applyForce( orbitForce * 0.0001f );

		//std::cout << "cam-R: " << (int)(mainScene->getCameraRotation().x*180/3.1415) << ", " << (int)(mainScene->getCameraRotation().y*180/3.1415) << ", " << (int)(mainScene->getCameraRotation().z*180/3.1415) << "\n";
		//std::cout << "cam-D: " << mainScene->getCameraDirection().x << ", " << mainScene->getCameraDirection().y << ", " << mainScene->getCameraDirection().z << "\n";

		//mainScene->Camera_Position( ( testObject->orientation() * vec4( testLimb->position(), 0.0f ) ).xyz() );
		//mainScene->Camera_Orient( testLimb->orientation() * testObject->orientation() );
		if ( input->getKeySimple( 'E' ) ) {
			mainScene->Camera_Position( ( testObject->orientation() * vec4( testLimb->position(), 1.0f ) ).xyz() );
			//mainScene->Camera_SetRotation( matrixToQuaternion( testObject->orientation() * Rocket::Core::QuaternionRotate( testLimb->rotation_quaternion() ) ) );
			//mainScene->Camera_SetRotation( matrixToQuaternion( testObject->orientation() * testLimb->orientation() ) );
			// todo: get this to work properly!
		}

		// position pickCube
		vec2i mousePos = input->getMousePosition();
		vec4 pickVec = mainScene->pickScreen( mousePos.x, mousePos.y );
		pickVec.z = abs( pickVec.z );
		vec4 pickCameraVec = mainScene->getCameraOrientationInverse() * pickVec;
		vec3 pickPos = -(pickCameraVec.xyz()) + mainScene->getCameraPosition();
		pickCube->position( pickPos );

		// rotate test sprite
		testSprite->Raster::rotate( Rocket::MathConstants::PI / 400.0f );

		//Core::Debug_StopTimer( "MAIN" );

		// OpenGL rendering goes here...
		//glClear( GL_COLOR_BUFFER_BIT );
		world->display( (float)elapsed );

		Core::string polyCount = "Polys(tris): ";
		polyCount << world->m_frame_renderedPolygons;
		Core::Debug_AddToLog( polyCount.c_str() );
		Core::string objectCount = "Objects: ";
		objectCount << world->m_frame_renderedObjects;
		Core::Debug_AddToLog( objectCount.c_str() );

		
		glfwSwapBuffers( window );
		glfwPollEvents();
		
		
		std::string debug = Core::Debug_PrintLogView();
		if ( debug.size() > 0 ) testBitmap->setText( debug + "\nThe quick, brown fox jumped over the lazy dog.\nTHE QUICK BROWN FOX JUMPED OVER THE LAZY DOG!\n0123456789?\n" );
	
		//Core::Debug_ResetTimer( "Object::draw" );

		// Check if ESC key was pressed or window was closed
		running = !input->getKeySimple( GLFW_KEY_ESCAPE ) && !glfwWindowShouldClose( window );
	}

	// Cleanup before closing the context
	delete world;

	glfwTerminate();

	//return 0;
	return;
}
