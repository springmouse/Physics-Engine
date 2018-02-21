#include "Sphere.h"
#include <iostream>

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, float elasticity, float rotation, float linearDrag, float angularDrag, glm::vec4 colour)
	:RigidBody(eShapeType::SPHERE, position, velocity, rotation, mass, elasticity, linearDrag, angularDrag)
{
	m_radius = radius;
	m_colour = colour;

	m_moment = 0.5f * mass * radius;
}

Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	glm::vec2 end = glm::vec2(std::cos(m_roation), std::sin(m_roation)) * m_radius;
	
	aie::Gizmos::add2DCircle(m_currPos, m_radius, 32, m_colour);
	aie::Gizmos::add2DLine(m_currPos, m_currPos + end, glm::vec4(1,1,1,1));
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
