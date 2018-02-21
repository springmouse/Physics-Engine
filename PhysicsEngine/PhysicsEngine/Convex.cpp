#include "Convex.h"



Convex::Convex()
{
}


Convex::~Convex()
{
}

void Convex::MakeGizmo()
{
}

void Convex::AddPoint(glm::vec2 Point)
{
	m_points.push_back(Point);
}
