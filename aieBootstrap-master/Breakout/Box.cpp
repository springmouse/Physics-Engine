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

bool Box::checkBoxCorners(Box & box, glm::vec2 & contact, int & numContacts, float & pen, glm::vec2 & edgeNormal)
{

	float minX, maxX, minY, maxY;
	float boxW = box.getExtends().x * 2;
	float boxH = box.getExtends().y * 2;
	int numLocalContacts = 0;
	glm::vec2 localContact(0, 0);
	bool first = true;

	for (float x = -box.getExtends().x; x < boxW; x += boxW)
	{
		for (float y = -box.getExtends().y; y < boxH; y += boxH)
		{
			// position in worldspace 
			glm::vec2 p = box.getPos() + x * box.m_localX + y * box.m_localY;
			// position in our box's space 
			glm::vec2 p0(glm::dot(p - m_currPos, m_localX), glm::dot(p - m_currPos, m_localY));

			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;

			if (p0.x >= -m_extends.x && p0.x <= m_extends.x && p0.y >= -m_extends.y && p0.y <= m_extends.y)
			{
				numLocalContacts++;
				localContact += p0;
			}

			first = false;
		}
	}

	if (maxX <-m_extends.x || minX >m_extends.x || maxY<-m_extends.y || minY >m_extends.y)
		return false;
	if (numLocalContacts == 0)
		return false;

	bool res = false;
	contact += m_currPos + (localContact.x*m_localX + localContact.y*m_localY) / (float)numLocalContacts;
	numContacts++;
	float pen0 = m_extends.x - minX;

	if (pen0 > 0 && (pen0 < pen || pen == 0))
	{
		edgeNormal = m_localX; pen = pen0; res = true;
	}
	pen0 = maxX + m_extends.x;

	if (pen0 > 0 && (pen0 < pen || pen == 0)) 
	{ 
		edgeNormal = -m_localX; 
		pen = pen0; res = true; 
	} 
	pen0 = m_extends.y - minY; 
	if (pen0 > 0 && (pen0 < pen || pen == 0)) 
	{ 
		edgeNormal = m_localY; 
		pen = pen0; res = true; 
	} 
	pen0 = maxY + m_extends.y; 
	if (pen0 > 0 && (pen0 < pen || pen == 0)) 
	{ 
		edgeNormal = -m_localY; pen = pen0; res = true; 
	} 
	
	return res;
}
