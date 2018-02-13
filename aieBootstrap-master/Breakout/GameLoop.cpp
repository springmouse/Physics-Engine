#include "GameLoop.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

#include <glm\ext.hpp>
#include <Gizmos.h>
#include <cmath>



GameLoop::GameLoop()
{
}


GameLoop::~GameLoop()
{
}

void GameLoop::Init()
{
	m_physicsScene = new PhysicsScene();
	m_physicsScene->constructor( 0.01f, glm::vec2(0.0f, -9.8f));

	Sphere * ball1 = new Sphere(glm::vec2(-50,0),glm::vec2(11.11f, 0), 80, 3, 1, glm::vec4(1,0,0,1));
	Sphere * ball2 = new Sphere(glm::vec2(50, 1.5f), glm::vec2(-15, 0), 80, 3, 1, glm::vec4(0, 1, 0, 1));
	Sphere * ball3 = new Sphere(glm::vec2(25, 30), glm::vec2(5, 15), 80, 3, 1, glm::vec4(1, 1, 0, 1));
	Sphere * ball4 = new Sphere(glm::vec2(55, -20), glm::vec2(-2, -20), 80, 3, 1, glm::vec4(0, 1, 1, 1));
	Sphere * ball5 = new Sphere(glm::vec2(-30, 1.5f), glm::vec2(4, 8), 80, 3, 1, glm::vec4(1, 0, 1, 1));
	Sphere * ball6 = new Sphere(glm::vec2(-45, 20), glm::vec2(11, -7), 80, 3, 1, glm::vec4(0.5f, 1, 0.5f, 1));

	m_physicsScene->addActor(ball1);
	m_physicsScene->addActor(ball2);
	m_physicsScene->addActor(ball3);
	m_physicsScene->addActor(ball4);
	m_physicsScene->addActor(ball5);
	m_physicsScene->addActor(ball6);

	Box * box1 = new Box(glm::vec2(-10, 0), 5,5,glm::vec2(5,0), 1, 40, 1, glm::vec4(0,0,1,1));
	Box * box2 = new Box(glm::vec2(10, 10), 5, 5, glm::vec2(-5, 0), 1, 40, 1, glm::vec4(0, 1, 1, 1));

	m_physicsScene->addActor(box1);
	m_physicsScene->addActor(box2);

	Plane * plane = new Plane(glm::vec2(0, -1), 35, glm::vec4(0,1,1,1));
	Plane * plane1 = new Plane(glm::vec2(0, -1), -35, glm::vec4(0, 1, 1, 1));
	Plane * plane2 = new Plane(glm::vec2(1, 0), 95, glm::vec4(0, 1, 1, 1));
	Plane * plane3 = new Plane(glm::vec2(1, 0), -95, glm::vec4(0, 1, 1, 1));

	m_physicsScene->addActor(plane);
	m_physicsScene->addActor(plane1);
	m_physicsScene->addActor(plane2);
	m_physicsScene->addActor(plane3);
}

void GameLoop::Loop(float deltaTime)
{
	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	//setUpContinuousDemo(glm::vec2(-15, 0), 25.0f, 10.0f, -9.8f);
}

void GameLoop::Render(aie::Renderer2D *)
{
	// draw your stuff here! 
	static float aspectRatio = 16 / 9.f; 
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));
}

void GameLoop::setUpContinuousDemo(glm::vec2 startPos, float inclnation, float speed, float gravity)
{
	float t = 0;
	float tstep = 0.5f;
	float radius = 1.0f;
	int segments = 12;
	glm::vec4 colour = glm::vec4(1, 1, 0, 1);

	while (t <= 5)
	{
		aie::Gizmos::add2DCircle(glm::vec2(startPos.x + (speed * t), startPos.y + (inclnation * t) + ((gravity * (std::pow(t,2))) * 0.5f)), radius, segments, colour);
		t += tstep;
	}
}
