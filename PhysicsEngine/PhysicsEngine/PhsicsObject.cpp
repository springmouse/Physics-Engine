#include "PhsicsObject.h"


bool PhsicsObject::operator==(PhsicsObject & RHS)
{
	if (m_currPos == RHS.GetPos())
	{
		return true;
	}

	return false;
}

PhsicsObject::~PhsicsObject()
{
}
