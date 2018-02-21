#pragma once

#include "Renderer2D.h"
#include "PhysicsScene.h"

class GameLoop
{
private:
public:
	GameLoop();
	~GameLoop();

	void Init();

	void Loop(float deltaTime);
	void Render(aie::Renderer2D *);

	void setUpContinuousDemo(glm::vec2 startPos, float inclnation, float speed, float gravity);

	PhysicsScene * m_physicsScene;

};

