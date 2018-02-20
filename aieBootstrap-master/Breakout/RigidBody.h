#pragma once

#include "PhysicsObject.h"
//#define Max_Int 

class RigidBody :
	public PhysicsObject
{
public:
	RigidBody(eShapeType shapeID, glm::vec2 position, glm::vec2 velocity,
		float rotation, float mass, float elasticity, float linearDrag, float angularDrag);
	~RigidBody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug() {};
	void applyForce(glm::vec2 force, glm::vec2 pos);

	void setPosition(glm::vec2 pos) { m_currPos = pos; }

	void setKinematic(bool state) { m_isKinematic = state; }
	bool isKinematic() { return m_isKinematic; }

	void resolveCollision(RigidBody * actor2, glm::vec2 contact, glm::vec2 * collisionNormal = nullptr);

	virtual bool checkCollision(PhysicsObject * pOther) = 0;

	float getRotation() { return m_roation; }
	float getAngularVelocity() { return m_angularVelocity; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return (m_isKinematic) ? INT_MAX : m_mass; }
	float getElasticity() { return m_elasticity; }
	float getMoment() { return m_moment; }

protected:
	float m_roation;
	glm::vec2 m_velocity;
	float m_mass;
	float m_elasticity;

	bool m_isKinematic;

	float m_angularVelocity;

	float m_linearDrag;
	float m_angularDrag;

	float m_moment;
};

