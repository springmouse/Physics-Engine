#include "GameLoop.h"
#include "Circle.h"

#include <glm\ext.hpp>
#include <Gizmos.h>


GameLoop::GameLoop()
{
}


GameLoop::~GameLoop()
{
}

void GameLoop::Init()
{
	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(glm::vec2(0, -9.8f));
	m_physicsScene->SetTimeStep(0.01f);

	Circle * circle1 = new Circle(glm::vec2(-20, 25), glm::vec2(10, 0), 5, glm::vec4(1, 0, 0, 1));
	circle1->SetElasticity(1);
	circle1->SetMass(5);

	Circle * circle2 = new Circle(glm::vec2(20, 25), glm::vec2(-10, 0), 5, glm::vec4(0, 1, 0, 1));
	circle2->SetElasticity(1);
	circle2->SetMass(5);

	m_physicsScene->AddActor(circle1);
	m_physicsScene->AddActor(circle2);


}

void GameLoop::Loop(float deltaTime)
{
	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->UpdateGizmos();
}

void GameLoop::Render(aie::Renderer2D * renderer)
{
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	m_physicsScene->UpdateRender(renderer);
}
