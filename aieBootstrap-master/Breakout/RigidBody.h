#pragma once

#include "PhysicsObject.h"

class RigidBody :
	public PhysicsObject
{
public:
	RigidBody(eShapeType shapeID, glm::vec2 position, glm::vec2 velocity,
		float rotation, float mass, float elasticity);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {};
	void applyForce(glm::vec2 force);
	void applyForceToActor(RigidBody * actor2, glm::vec2 force);

	void resolveCollision(RigidBody * actor2);

	virtual bool checkCollision(PhysicsObject * pOther) = 0;

	float getRotation() { return m_roation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	float getElasticity() { return m_elasticity; }

protected:
	float m_roation;
	glm::vec2 m_velocity;
	float m_mass;
	float m_elasticity;
};

