#pragma once

#include "Renderer2D.h"
#include "RigidBody.h"

#include <vector>

class Convex : RigidBody
{
public:
	Convex();
	~Convex();
	
	virtual void Renderer(aie::Renderer2D * renderer) {};
	virtual void MakeGizmo();

	void AddPoint(glm::vec2 Point);

protected:
	std::vector<glm::vec2> m_points;
};

