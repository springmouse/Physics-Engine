#pragma once

#include "RigidBody.h"

class Box :
	public RigidBody
{
public:
	Box(glm::vec2 center, float length, float height, glm::vec2 velocity, float rotation, float mass, float elasticity, float linearDrag, float angularDrag, glm::vec4 colour);

	~Box();

	virtual void makeGizmo();
	virtual bool checkCollision(PhysicsObject * pOther) { return false; };
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);

	glm::vec2 getMax() { return m_currPos + m_extends; };
	glm::vec2 getMin() { return m_currPos - m_extends; };

	glm::vec2 getLocalX() { return m_localX; }
	glm::vec2 getLocalY() { return m_localY; }

	float getWidth() { return m_extends.x; }
	float getHeight() { return m_extends.y; }

	const glm::vec2 getExtends() { return m_extends; }

	bool checkBoxCorners(Box& box, glm::vec2& contact, int & numContacts, glm::vec2& edgeNormal, glm::vec2 & contactForce);

protected:
	glm::vec2 m_extends;

	glm::vec2 m_localX;
	glm::vec2 m_localY;
};

