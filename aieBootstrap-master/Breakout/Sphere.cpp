#include "Sphere.h"

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float elasticity, glm::vec4 colour)
	:RigidBody(eShapeType::SPHERE, position, velocity, 0.0f, mass, elasticity)
{
	m_radius = radius;
	m_colour = colour;
}

Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_currPos, m_radius, 32, m_colour);
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	if(dynamic_cast<Sphere*>(pOther) != nullptr)
	{
		if ((glm::distance(pOther->getPos(), m_currPos)) < (dynamic_cast<Sphere*>(pOther)->getRadius() + m_radius))
		{
			return true;
		}
	}

	return false;
}
