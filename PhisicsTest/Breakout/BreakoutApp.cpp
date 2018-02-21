#include "BreakoutApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <Gizmos.h>

BreakoutApp::BreakoutApp() {

}

BreakoutApp::~BreakoutApp() {

}

bool BreakoutApp::startup() 
{
	//increases the 2d line count to maxamise the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_gameLoop = new GameLoop();

	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_gameLoop->Init();

	return true;
}

void BreakoutApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void BreakoutApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	m_gameLoop->Loop(deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void BreakoutApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!	
	m_gameLoop->Render(m_2dRenderer);
	
	// done drawing sprites
	m_2dRenderer->end();
}