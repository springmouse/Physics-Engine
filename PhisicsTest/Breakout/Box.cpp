#include "Box.h"

Box::Box(glm::vec2 center, float length, float height, glm::vec2 velocity, float rotation, float mass, float elasticity, float linearDrag, float angularDrag, glm::vec4 colour) 
	: RigidBody(eShapeType::BOX, center, velocity, rotation, mass, elasticity, linearDrag, angularDrag)
{
	m_extends = glm::vec2(length, height);

	m_colour = colour;

	m_moment = 1.0f / 12.0f * mass * length * height;


	float cs = cosf(m_roation);
	float sn = sinf(m_roation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

Box::~Box()
{
}

void Box::makeGizmo()
{
	glm::vec2 p1 = m_currPos - m_localX * m_extends.x - m_localY * m_extends.y;
	glm::vec2 p2 = m_currPos + m_localX * m_extends.x - m_localY * m_extends.y;
	glm::vec2 p3 = m_currPos - m_localX * m_extends.x + m_localY * m_extends.y;
	glm::vec2 p4 = m_currPos + m_localX * m_extends.x + m_localY * m_extends.y;
	aie::Gizmos::add2DTri(p1, p2, p4, m_colour); 
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}

void Box::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	RigidBody::fixedUpdate(gravity, timeStep);

	float cs = cosf(m_roation);
	float sn = sinf(m_roation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));
}

bool Box::checkBoxCorners(Box & box, glm::vec2 & contact, int & numContacts, glm::vec2 & edgeNormal, glm::vec2 & contactForce)
{
	float boxW = box.getExtends().x * 2; 
	float boxH = box.getExtends().y * 2; 
	float penetration = 0; 
	
	for (float x = -box.getExtends().x; x < boxW; x += boxW) 
	{
		for (float y = -box.getExtends().y; y < boxH; y += boxH) 
		{ 
			// pos in worldspace 
			glm::vec2 p = box.getPos() + x * box.m_localX + y * box.m_localY; 
			// position in our box's space 
			glm::vec2 p0(glm::dot(p - m_currPos, m_localX), glm::dot(p - m_currPos, m_localY)); 

			float w2 = m_extends.x, h2 = m_extends.y; 
			if (p0.y < h2 && p0.y > -h2) 
			{ 
				if (p0.x > 0 && p0.x < w2) 
				{ 
					numContacts++; contact += m_currPos + w2 * m_localX + p0.y * m_localY;
					edgeNormal = m_localX;
					penetration = w2 - p0.x; 
				} 
				if (p0.x < 0 && p0.x > -w2) 
				{ 
					numContacts++; 
					contact += m_currPos - w2 * m_localX + p0.y * m_localY;
					edgeNormal = -m_localX; penetration = w2 + p0.x; 
				} 
			} 
			
			if (p0.x < w2 && p0.x > -w2) 
			{ 
				if (p0.y > 0 && p0.y < h2)
				{ 
					numContacts++; 
					contact += m_currPos + p0.x * m_localX + h2 * m_localY;
					float pen0 = h2 - p0.y; 
					if (pen0 < penetration || penetration == 0) 
					{ 
						penetration = pen0; edgeNormal = m_localY; 
					} 
				} 
				if (p0.y < 0 && p0.y > -h2) 
				{ 
					numContacts++; 
					contact += m_currPos + p0.x * m_localX - h2 * m_localY;
					float pen0 = h2 + p0.y; 
					if (pen0 < penetration || penetration == 0) 
					{
						penetration = pen0; edgeNormal = -m_localY; 
					} 
				} 
			}
		}
	} 
	contactForce = penetration * edgeNormal; 
	return (penetration != 0);
}
