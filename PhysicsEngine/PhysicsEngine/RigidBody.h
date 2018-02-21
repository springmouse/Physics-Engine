#pragma once
#include "PhsicsObject.h"
class RigidBody :
	public PhsicsObject
{
public:
	RigidBody(eShapeType shapeType);
	~RigidBody();

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void Debug() {};

	void ApplyForce(glm::vec2 force, glm::vec2 pos);

	void ResolveCollision(RigidBody * actor2, glm::vec2 contact, glm::vec2 * collisionNormal = nullptr);

	float GetRotation() { return m_roation; }
	float GetAngularVelocity() { return m_angularVelocity; }
	glm::vec2 GetVelocity() { return (m_isKinematic) ? glm::vec2(0,0) : m_velocity; }
	float GetMass() { return (m_isKinematic) ? INT_MAX : m_mass; }
	void SetMass(float mass) { m_mass = mass; }
	float GetElasticity() { return m_elasticity; }
	void SetElasticity(float amount) { m_elasticity = amount; }
	float GetMoment() { return m_moment; }

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

