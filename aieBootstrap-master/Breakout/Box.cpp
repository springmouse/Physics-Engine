#include "Box.h"

Box::Box(glm::vec2 center, float length, float height, glm::vec2 velocity, float rotation, float mass, float elasticity, float linearDrag, float angularDrag, glm::vec4 colour) : RigidBody(eShapeType::BOX, center, velocity, rotation, mass, elasticity, linearDrag, angularDrag)
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
