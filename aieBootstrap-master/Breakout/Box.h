#pragma once

#include "RigidBody.h"

class Box :
	public RigidBody
{
public:
	Box(glm::vec2 center, float length, float height, glm::vec2 velocity, float rotation, float mass, float elasticity, glm::vec4 colour);

	~Box();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject * pOther) { return false; };

	glm::vec2 getMax() { return m_currPos + m_extends; };
	glm::vec2 getMin() { return m_currPos - m_extends; };

protected:
	glm::vec2 m_extends;
};

