#include "Box.h"

Box::Box(glm::vec2 center, float length, float height, glm::vec2 velocity, float rotation, float mass, float elasticity, glm::vec4 colour) : RigidBody(eShapeType::BOX, center, velocity, rotation, mass, elasticity)
{
	m_extends = glm::vec2(length, height);

	m_colour = colour;
}

Box::~Box()
{
}

void Box::makeGizmo()
{
	aie::Gizmos::add2DAABB(m_currPos, m_extends, m_colour);
}
