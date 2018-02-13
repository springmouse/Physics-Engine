#pragma once

#include <glm\ext.hpp>
#include <Gizmos.h>

enum eShapeType
{
	PLANE = 0,
	SPHERE = 1,
	BOX = 2
};

static int SHAPE_COUNT = eShapeType::BOX +1;

class PhysicsObject
{
protected:
	PhysicsObject(eShapeType a_shapeID) : m_ShapeID(a_shapeID) {}

public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

	const glm::vec2 getPos();
	void AddToPos(glm::vec2 increase);

	glm::vec4 GetColour() { return m_colour; }

	bool operator== (PhysicsObject& RHS);
	
	int getShapeID() { return m_ShapeID; }

protected:
	virtual ~PhysicsObject();

	glm::vec2 m_currPos;
	glm::vec2 m_prevPos;

	glm::vec4 m_colour;

	eShapeType m_ShapeID;

};

