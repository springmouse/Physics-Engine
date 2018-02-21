#include "PhysicsObject.h"


PhysicsObject::~PhysicsObject()
{
}

const glm::vec2 PhysicsObject::getPos()
{
	return m_currPos;
}

void PhysicsObject::AddToPos(glm::vec2 increase)
{
	m_currPos += increase;
}

bool PhysicsObject::operator==(PhysicsObject & RHS)
{
	if (m_currPos == RHS.getPos())
	{
		return true;
	}

	return false;
}
