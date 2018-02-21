#pragma once

#include "Renderer2D.h"

#include <glm\ext.hpp>

enum eShapeType
{
	JOINT = -1,
	SPHERE = 0,
	COMPLEX = 1
};

static int SHAPE_COUNT = eShapeType::COMPLEX + 1;

class PhsicsObject
{
protected:
	PhsicsObject(eShapeType a_shapeID) : m_ShapeID(a_shapeID) {}

public:

	virtual void FixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void Debug() = 0;
	virtual void MakeGizmo() = 0;
	virtual void Renderer(aie::Renderer2D * renderer) = 0;

	const glm::vec2 GetPos() { return m_currPos; }
	glm::vec4 GetColour() { return m_colour; }

	bool operator== (PhsicsObject& RHS);

	int GetShapeID() { return m_ShapeID; }

protected:

	virtual ~PhsicsObject();
	
	glm::vec2 m_currPos;
	glm::vec2 m_prevPos;

	glm::vec4 m_colour;

	eShapeType m_ShapeID;
};

