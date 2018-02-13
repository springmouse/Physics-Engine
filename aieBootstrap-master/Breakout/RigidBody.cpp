#include "RigidBody.h"



RigidBody::RigidBody(eShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity, float linearDrag, float angularDrag) : PhysicsObject(shapeID)
{
	m_ShapeID = shapeID;
	m_currPos = position;
	m_velocity = velocity;
	m_roation = rotation;
	m_mass = mass;
	m_elasticity = elasticity;
	m_linearDrag = linearDrag;
	m_angularDrag = m_angularDrag;
}

RigidBody::~RigidBody()
{
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	applyForce(gravity * m_mass * timeStep);
	m_currPos += m_velocity * timeStep;

	if (m_linearDrag > 0)
	{
		m_velocity -= m_velocity * m_linearDrag * timeStep;

		if (glm::length(m_velocity) < 0.01f)
		{
			m_velocity = glm::vec2(0, 0);
		}
	}

	if (m_angularDrag > 0)
	{
		m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

		if (glm::abs(m_angularVelocity) < 0.01f)
		{
			m_angularVelocity = 0;
		}
	}
}

void RigidBody::applyForce(glm::vec2 force)
{
	m_velocity += force / m_mass;
}

void RigidBody::applyForceToActor(RigidBody * actor2, glm::vec2 force)
{
	actor2->applyForce(force);
	applyForce(-force);
}

void RigidBody::resolveCollision(RigidBody * actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPos() - m_currPos);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float j = glm::dot(-(1 + (m_elasticity * actor2->getElasticity())) * (relativeVelocity), normal);
	j /= (1 / m_mass) + (1 / actor2->getMass());

	glm::vec2 force = normal * j;

	applyForceToActor(actor2, force);
}
