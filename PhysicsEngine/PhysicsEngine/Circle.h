#pragma once
#include "RigidBody.h"
#include "PhsicsObject.h"
#include "Renderer2D.h"

class Circle :
	public RigidBody
{
public:
	Circle(glm::vec2 pos, glm::vec2 velocity, float radius, glm::vec4 colour);
	~Circle();


	float getRadius() { return m_radius; }

protected:
	float m_radius;
};

