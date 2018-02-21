#include "Plane.h"
#include "RigidBody.h"

Plane::Plane() : PhysicsObject(PLANE)
{
	m_distanceToOrigin = 0;
	m_normal = glm::normalize(glm::vec2(0,1));
	m_colour = glm::vec4(1, 1, 1, 1);
}

Plane::Plane(glm::vec2 normal, float distance, glm::vec4 colour) :PhysicsObject(PLANE)
{
	m_colour = colour;
	m_normal = glm::normalize(normal);
	m_distanceToOrigin = distance;
}


Plane::~Plane()
{
}

void Plane::makeGizmo()
{
	float length = 300;

	glm::vec2 centerPoint = m_normal * m_distanceToOrigin;

	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec2 start = centerPoint + (parallel * length);
	glm::vec2 end = centerPoint - (parallel * length);

	aie::Gizmos::add2DLine(start, end, m_colour);
}

void Plane::resolveCollision(RigidBody * actor2, glm::vec2 contact)
{
	float j = glm::dot(-(1 + (actor2->getElasticity())) * (actor2->getVelocity()), m_normal) / (1 / actor2->getMass());

	glm::vec2 force = m_normal * j;

	actor2->applyForce(force, contact - actor2->getPos());
}
