#include "Circle.h"

#include <Gizmos.h>

Circle::Circle(glm::vec2 pos, glm::vec2 velocity, float radius,  glm::vec4 colour) : RigidBody(eShapeType::SPHERE)
{
	m_currPos = pos;
	m_velocity = velocity;
	m_radius = radius;
	m_colour = colour;
}

Circle::~Circle()
{
}

void Circle::MakeGizmo()
{
	aie::Gizmos::add2DCircle(m_currPos, m_radius, 32, m_colour);
	aie::Gizmos::
}
