#include "RigidBody.h"



RigidBody::RigidBody(eShapeType shapeType) : PhsicsObject (shapeType)
{
	m_isKinematic = false;
}


RigidBody::~RigidBody()
{
}

void RigidBody::FixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isKinematic)
	{
		return;
	}
	
	if (m_angularVelocity > 0)
	{
		m_roation += m_angularVelocity;
	}

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

	m_velocity += gravity/* * m_mass */* timeStep;
	m_currPos += m_velocity * timeStep;
}

void RigidBody::ApplyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / m_mass;
	//m_angularVelocity += ((force.y * pos.x) - (force.x * pos.y)) / (m_moment);
}

void RigidBody::ResolveCollision(RigidBody * actor2, glm::vec2 contact, glm::vec2 * collisionNormal)
{
	float elasticity = (m_elasticity + actor2->GetElasticity()) * 0.5f;
	glm::vec2 relativeVelocity = actor2->GetVelocity() - m_velocity;
	glm::vec2 normal = glm::normalize(actor2->GetPos() - m_currPos);

	float j = glm::dot(-((1 + elasticity) * (relativeVelocity)), normal);
	j /= glm::dot(normal, normal * ((1/m_mass) + (1/actor2->GetMass())));

	glm::vec2 force = normal * j;

	ApplyForce(-force, contact);
	actor2->ApplyForce(force, contact);
}
