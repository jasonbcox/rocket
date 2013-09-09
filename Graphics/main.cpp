
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
#include "Scissor.h"
#include "Button.h"

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
	auto mainScene = make_shared< Scene >( 63.0f, (Graphics::WindowWidth * 1.0f)/(Graphics::WindowHeight * 1.0f), 1.0f, 1000.0f );
	world->addRenderPass( mainScene.get() );
	//auto hudScene = make_shared< Scene >( Graphics::WindowWidth * 1.0f, Graphics::WindowHeight * 1.0f );
	auto hudScene = make_shared< Scissor >( Graphics::WindowWidth * 1.0f, Graphics::WindowHeight * 1.0f, 0, 20, 400, 400 );
	world->addRenderPass( hudScene.get() );

	// Load shaders
	auto shader_texture = make_shared< ShaderDefaults::Shader_Texture >( "../../Graphics/Shaders/texture_v.glsl", "../../Graphics/Shaders/texture_f.glsl" );
	world->addShader( "texture", shader_texture.get() );

	// Load meshes
	auto testMesh = generatePrimitive_Quad( world, "testMesh", shader_texture.get() );
	mainScene->addMesh( testMesh );

	//Mesh * testCubeMesh = generatePrimitive_Cube( shader_texture );
	auto testCubeMesh = world->loadMesh( "human", "../../Graphics/Shaders/human.obj", shader_texture.get() ).get(); //new Mesh( "../../Graphics/Shaders/human.obj", shader_texture );
	mainScene->addMesh( testCubeMesh );

	auto testCubeMesh2 = generatePrimitive_Cube( world, "testCubeMesh", shader_texture.get() );
	hudScene->addMesh( testCubeMesh2 );

	auto testPlanetMesh = world->loadMesh( "planet", "../../Graphics/Shaders/StaticPlanet1.obj", shader_texture.get() ).get(); //new Mesh ( "../../Graphics/Shaders/StaticPlanet1.obj", shader_texture );
	mainScene->addMesh( testPlanetMesh );

	// Load textures and objects

	// Yellow smiley-face glass pane
	auto testObject = make_shared< Object_Newton >( testMesh );
	auto testTexture = world->loadTexture( "test_happyface", "../../Graphics/Shaders/testTexture1.bmp", true, false );
	ShaderDefaults::setObjectUniforms_texture( testObject.get(),
		Shader_UniformTexture( 0, testTexture.get(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE ),
		vec2( 1.0f, 1.0f ), vec2( 0.0f, 0.0f ), vec3( 0.99f, 0.98f, 0.97f ), true, 0.0f, 0.5f );
	mainScene->addObject( testObject.get(), nullptr );
	testObject->setVelocity( vec3(0,0.1f,0) );
	//testObject->setAngularAcceleration( vec3(0,0.00003f,0) );

	// Test Planet
	auto testPlanet = make_shared< Object >( testPlanetMesh );
	auto testPlanetTexture = world->loadTexture( "test_planet", "../../Graphics/Shaders/StaticPlanet1.png", true, true );
	ShaderDefaults::setObjectUniforms_texture( testPlanet.get(),
		Shader_UniformTexture( 0, testPlanetTexture.get(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE ),
		vec2( 1.0f, 1.0f ), vec2( 0.0f, 0.0f ), vec3( 1.0f, 0.8f, 0.5f ), false, 0.0f, 1.0f );
	mainScene->addObject( testPlanet.get(), nullptr );
	testPlanet->scale( vec3( 10.0f, 10.0f, 10.0f ) );
	testPlanet->position( vec3( -500.0f, 300.0f, -1000.0f ) );
	testPlanet->rotatePitch( MathConstants::PI / 8.0f );
	testPlanet->rotateYaw( MathConstants::PI / 6.0f );

	// Blue test cube
	auto testCube = make_shared< Object >( testCubeMesh );
	auto testHumanTexture = world->loadTexture( "test_human", "../../Graphics/Shaders/human1.png", true, true );
	ShaderDefaults::setObjectUniforms_texture( testCube.get(),
		Shader_UniformTexture( 0, testHumanTexture.get(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE ),
		vec2( 1.0f, 1.0f ), vec2( 0.0f, 0.0f ), vec3( 1.0f, 1.0f, 1.0f ), false, 0.0f, 1.0f );
	mainScene->addObject( testCube.get(), nullptr );

	testObject->position( vec3(0,0,-2) );
	testObject->scale( vec3(3,3,3) );
	testCube->position( vec3(0,0,-5) );
	testCube->scale( vec3(2,2,2) );

	// Clone the yellow glass pane and make the clone red
	auto testLimb = testObject->cloneNewton();
	//*(testLimb->getUniformVec3( "uniform_color" )) = vec3( 1.0f, 0.0f, 0.0f );
	((ShaderDefaults::ShaderUniforms_Texture*)(testLimb->getShaderUniforms()))->m_color = vec3( 1.0f, 0.0f, 0.0f );
	mainScene->addObject( testLimb.get(), testObject.get() );
	testLimb->position( vec3( 0,0.75,0 ) );
	testLimb->rotateYaw( Rocket::MathConstants::PI / 4.0f );
	testObject->rotateRoll( Rocket::MathConstants::PI / 4.0f );
	//testLimb->hide();
	testLimb->setVelocity( vec3(0,0,0.001f) );

	// make test clones
	for ( int c = 0; c < 1000; c++ ) {
		//auto tempClone = testLimb->cloneNewton();
		auto tempClone = testCube->clone();
		mainScene->addObject( tempClone.get(), testObject.get() );
		tempClone->position( vec3(c*1.0f+1.0f,0,0) );
		tempClone->scale( vec3(0.1f,0.1f,0.1f) );
	}

	// Green "selector" test cube
	auto pickCube = testCube->clone();
	//*(pickCube->getUniformVec3( "uniform_color" )) = vec3( 0.0f, 1.0f, 0.0f );
	((ShaderDefaults::ShaderUniforms_Texture*)(pickCube->getShaderUniforms()))->m_color = vec3( 0.0f, 1.0f, 0.0f );
	pickCube->scale( vec3( 0.03f,0.03f,0.03f ) );
	pickCube->position( vec3( 0,0,0 ) );
	mainScene->addObject( pickCube.get(), nullptr );
	pickCube->hide();

	// Red HUD cube
	auto hudCube = make_shared< Object >( testCubeMesh2 );
	ShaderDefaults::setObjectUniforms_texture( hudCube.get(),
		Shader_UniformTexture( 0, testTexture.get(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE ),
		vec2( 1.0f, 1.0f ), vec2( 0.0f, 0.0f ), vec3( 1.0f, 0.0f, 0.0f ), false, 0.0f, 1.0f );
	hudScene->addObject( hudCube.get(), nullptr );
	//hudCube->position( vec3(-430,290,0) );
	hudCube->position( vec3(100,100,0) );
	hudCube->scale( vec3(100,100,100) );
	hudCube->rotateYaw( Rocket::MathConstants::PI / 4.0f );
	hudCube->rotatePitch( Rocket::MathConstants::PI / 4.0f );

	Sprite::enableSpritesInScene( world, hudScene.get(), shader_texture.get() );
	auto testTexture2 = world->loadTexture( "test_happyface2", "../../Graphics/Shaders/testTexture1.bmp", false, true );
	auto testSprite = make_shared< Sprite >( testTexture2.get(), 64, 64 );
	hudScene->addObject( testSprite.get(), nullptr );
	testSprite->setAngle( Rocket::MathConstants::PI / 4.0f );
	testSprite->setUVPixels(4,4,40,40);

	testSprite->addAnimation( "testAnimation", {vec4i(0,0,24,24), vec4i(4,0,28,24), vec4i(8,0,32,24), vec4i(12,0,36,24)} );
	testSprite->playAnimation( "testAnimation", 2000 );

	auto testBitmapTexture = world->loadTexture( "test_font", "../../Graphics/Shaders/testFont1.png", false, true );
	auto testBitmap = make_shared< Object_BitmapText >( testBitmapTexture.get() );
	testBitmap->setText( "The quick, brown fox jumped over the lazy dog.\nTHE QUICK BROWN FOX JUMPED OVER THE LAZY DOG!\n0123456789?" );
	hudScene->addObject( testBitmap.get(), nullptr );
	testBitmap->enableTransparency( 0.0f, 1.0f );
	testBitmap->setPosition( vec2i( 0, 200 ) );

	auto testButton = make_shared< ToggleButton >( testTexture2.get(), 100, 18, input );
	testButton->setPosition( vec2i( 60, 30 ) );
	testButton->addAnimation( "none", {vec4i(0,0,64,16)} );
	testButton->addAnimation( "hover", {vec4i(0,16,64,32)} );
	testButton->addAnimation( "pressed", {vec4i(0,32,64,48), vec4i(0,36,64,52), vec4i(0,40,64,56), vec4i(0,44,64,56), vec4i(0,48,64,60), vec4i(0,52,64,64)} );
	testButton->setStateAnimation( Button_ButtonState::NotPressed, "none" );
	testButton->setStateAnimation( Button_ButtonState::Hover, "hover" );
	testButton->setStateAnimation( Button_ButtonState::Pressed, "pressed" );
	testButton->setStateAnimation( Button_ButtonState::HoverPressed, "pressed" );
	hudScene->addObject( testButton.get(), nullptr );

	// Main Loop
	//input->hideMouse();
	//input->lockMouse( Graphics::WindowWidth/2, Graphics::WindowHeight/2 );

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

		rstring debug;
		debug << "FPS: ";
		debug << (int)(1000.0 / (elapsed*1.0));
		debug << "\n";

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
		pickVec.z = fabs( pickVec.z );
		vec4 pickCameraVec = mainScene->getCameraOrientationInverse() * pickVec;
		vec3 pickPos = -(pickCameraVec.xyz()) + mainScene->getCameraPosition();
		pickCube->position( pickPos );

		// rotate test sprite
		testSprite->Raster::rotate( Rocket::MathConstants::PI / 400.0f );

		//Core::Debug_StopTimer( "MAIN" );

		// OpenGL rendering goes here...
		world->display( (float)elapsed );

		debug << "Polys(tris): " << world->m_cache_renderedPolygons << "\n";
		debug << "Objects: ";
		debug << world->m_cache_renderedObjects;
		debug << "\n";

		glfwSwapBuffers( window );
		glfwPollEvents();

		if ( debug.length() > 0 ) testBitmap->setText( debug.std_str() + "\nThe quick, brown fox jumped over the lazy dog.\nTHE QUICK BROWN FOX JUMPED OVER THE LAZY DOG!\n0123456789?\n" );
	
		//Core::Debug_ResetTimer( "Object::draw" );

		// Check if ESC key was pressed or window was closed
		running = !input->getKeySimple( GLFW_KEY_ESCAPE ) && !glfwWindowShouldClose( window );
	}

	// Cleanup before closing the context
	delete world;

	glfwTerminate();

	return;
}
