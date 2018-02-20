#pragma once
#include "PhysicsObject.h"

class RigidBody;

class Spring :
	public PhysicsObject
{
public:
	Spring(RigidBody * body1, RigidBody * body2, float restLength, float springCoefficient, float damping = 0.1f,
		glm::vec2 contact1 = glm::vec2(0.0f, 0.0f), glm::vec2 contact2 = glm::vec2(0.0f, 0.0f));

	~Spring();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {};
	virtual void makeGizmo();

private:
	RigidBody * m_body1;
	RigidBody * m_body2;

	glm::vec2 m_contact1;
	glm::vec2 m_contact2;

	float m_damping;
	float m_restLength;
	float m_springCoefficient;
};

