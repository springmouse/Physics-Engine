#include "RigidBody.h"



RigidBody::RigidBody(eShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass, float elasticity) : PhysicsObject(shapeID)
{
	m_ShapeID = shapeID;
	m_currPos = position;
	m_velocity = velocity;
	m_roation = rotation;
	m_mass = mass;
	m_elasticity = elasticity;
}

RigidBody::~RigidBody()
{
}

void RigidBody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	applyForce(gravity * m_mass * timeStep);
	m_currPos += m_velocity * timeStep;
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
