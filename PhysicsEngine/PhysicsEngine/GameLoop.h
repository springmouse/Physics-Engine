#pragma once

#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Texture.h"

class GameLoop
{
public:
	GameLoop();
	~GameLoop();

	void Init();
	void Loop(float deltaTime);

	void Render(aie::Renderer2D * renderer);

private:
	PhysicsScene * m_physicsScene;
};

