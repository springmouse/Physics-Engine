#pragma once

#include "PhysicsObject.h"
#include "RigidBody.h"

#include <glm\ext.hpp>
#include <vector>
#include <list>
#include <iostream>
#include <functional>

class PhysicsScene
{
public:
	PhysicsScene();
	virtual ~PhysicsScene();

	void constructor(float timeStep, glm::vec2 gravity) { m_timeStep = timeStep; m_gravity = gravity; m_accumulatedTime = 0.0f; }

	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void update(float dt);
	void updateGizmos();

	void debugScene();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	void checkForCollision();

	static bool plane2Plane(PhysicsObject* ObjectA, PhysicsObject* ObjectB) { return false; };
	static bool plane2Sphere(PhysicsObject* ObjectA, PhysicsObject* ObjectB);
	static bool plane2Box(PhysicsObject* ObjectA, PhysicsObject* ObjectB);

	static bool sphere2Plane(PhysicsObject* ObjectA, PhysicsObject* ObjectB);
	static bool sphere2Sphere(PhysicsObject* ObjectA, PhysicsObject* ObjectB);
	static bool sphere2Box(PhysicsObject* ObjectA, PhysicsObject* ObjectB);

	static bool box2Plane(PhysicsObject* ObjectA, PhysicsObject* ObjectB);
	static bool box2Sphere(PhysicsObject* ObjectA, PhysicsObject* ObjectB);
	static bool box2Box(PhysicsObject* ObjectA, PhysicsObject* ObjectB);

protected: 
	float m_accumulatedTime = 0.0f;
	glm::vec2 m_gravity; 
	float m_timeStep; 
	std::vector<PhysicsObject*> m_actors;	
};

