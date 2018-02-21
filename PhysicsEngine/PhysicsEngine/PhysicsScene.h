#pragma once

#include "Renderer2D.h"
#include "PhsicsObject.h"

#include <glm\ext.hpp>
#include <functional>
#include <vector>
#include <functional>

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();


	void Update(float dt);
	void UpdateGizmos();
	void UpdateRender(aie::Renderer2D * renderer);

	void AddActor(PhsicsObject * actor) { m_actors.push_back(actor); };

	void SetGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 GetGravity() const { return m_gravity; }


	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float GetTimeStep() const { return m_timeStep; }


	void CheckForCollision();

	static bool CircleToCircle(PhsicsObject* ObjectA, PhsicsObject* ObjectB);
	static bool CircleToComplex(PhsicsObject* ObjectA, PhsicsObject* ObjectB) { return false; }
	static bool ComplexToCircle(PhsicsObject* ObjectA, PhsicsObject* ObjectB) { return false; }
	static bool ComplexToComplex(PhsicsObject* ObjectA, PhsicsObject* ObjectB) { return false; }

private:
	float m_acumulatedTime;
	float m_timeStep;

	glm::vec2 m_gravity;
	std::vector<PhsicsObject *> m_actors;
};

