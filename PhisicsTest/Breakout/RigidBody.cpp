#include "RigidBody.h"
#include <iostream>


RigidBody::RigidBody(eShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity, float linearDrag, float angularDrag) : PhysicsObject(shapeID)
{
	m_ShapeID = shapeID;
	m_currPos = position;
	m_velocity = velocity;
	m_roation = rotation;
	m_mass = mass;
	m_elasticity = elasticity;
	m_linearDrag = linearDrag;
	m_angularDrag = angularDrag;

	m_angularVelocity = 0.0f;

	m_isKinematic = false;
}

RigidBody::~RigidBody()
{
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	if (m_isKinematic)
	{
		return;
	}

	m_velocity += gravity * m_mass * timeStep;
	m_currPos += m_velocity * timeStep;

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
}

void RigidBody::applyForce(glm::vec2 force, glm::vec2 pos)
{
	m_velocity += force / m_mass;
	m_angularVelocity += ((force.y * pos.x) - (force.x * pos.y)) / (m_moment);

	//std::cout << "angular velocity = " << m_angularVelocity << " \n";
}

void RigidBody::resolveCollision(RigidBody * actor2, glm::vec2 contact, glm::vec2 * collisionNormal)
{
	/*glm::vec2 normal = glm::normalize(actor2->getPos() - m_currPos);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float j = glm::dot(-(1 + (m_elasticity * actor2->getElasticity())) * (relativeVelocity), normal);
	j /= (1 / m_mass) + (1 / actor2->getMass());

	glm::vec2 force = normal * j;

	applyForceToActor(actor2, force);*/

	glm::vec2 normal = collisionNormal ? *collisionNormal : glm::normalize(actor2->getPos() - m_currPos);

	glm::vec2 perp(normal.y, -normal.x);


	float r1 = glm::dot(contact - m_currPos, - perp);
	float r2 = glm::dot(contact - actor2->getPos(), perp);
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	float v2 = glm::dot(actor2->getVelocity(), normal) + r2 * actor2->getAngularVelocity();

	if (v1 > v2)
	{
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->getMass() + (r2 * r2) / actor2->m_moment);

		float elasticity = (m_elasticity + actor2->getElasticity()) * 0.5f;

		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 / (mass1 + mass2) * (v1 - v2) * normal;

		//float j = glm::dot((-(1 + elasticity)) * (v1 * v2), normal);

		//glm::vec2 force = normal *;

		applyForce(-force, contact - m_currPos);
		actor2->applyForce(force, contact - actor2->getPos());
	}
}
