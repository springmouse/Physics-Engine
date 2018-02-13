#include "PhysicsScene.h"

#include "Sphere.h"
#include "Box.h"

#include "Plane.h"
#include "Input.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::plane2Plane,  PhysicsScene::plane2Sphere, PhysicsScene::plane2Box,
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,
	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2Box
};

PhysicsScene::PhysicsScene()
{
}


PhysicsScene::~PhysicsScene()
{
	for (auto pActors : m_actors)
	{
		//delete pActors;
	}

	m_actors.clear();
}

void PhysicsScene::addActor(PhysicsObject * actor)
{
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject * actor)
{
	for (auto it = m_actors.begin(); it != m_actors.end();)
	{
		if (*it == actor)
		{
			m_actors.erase(it);
			break;
		}
	}
}

void PhysicsScene::update(float dt)
{
	//static std::list<PhysicsObject*> dirty;

	m_accumulatedTime += dt;

	while (m_accumulatedTime >= m_timeStep)
	{
		aie::Input* input = aie::Input::getInstance();

		if (input->isKeyDown('E') == true)
		{
			RigidBody * pRigid = dynamic_cast<RigidBody*>(m_actors.front());
			pRigid->applyForce(glm::vec2(5.0f, 0.0f) * m_timeStep);
		}

		for (auto pActor : m_actors) 
		{ 
			pActor->fixedUpdate(m_gravity, m_timeStep); 
		} 

		m_accumulatedTime -= m_timeStep;

		/*for (auto pActor : m_actors) 
		{ 
			for (auto pOther : m_actors) 
			{ 
				if (pActor == pOther) 
					continue; 

				if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() && std::find(dirty.begin(), dirty.end(), pOther) != dirty.end()) 
					continue; 

				Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor); 
				if (pRigid->checkCollision(pOther) == true) 
				{
					pRigid->applyForceToActor(dynamic_cast<Rigidbody*>(pOther), pRigid->getVelocity() * pRigid->getMass());
					dirty.push_back(pRigid); 
					dirty.push_back(pOther); 
				} 
			} 
		} 
		dirty.clear();*/

		checkForCollision();

		//dirty.clear();
	}
}



void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

void PhysicsScene::debugScene()
{

	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene::checkForCollision()
{

	int actorCount = m_actors.size();

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer+1; inner < actorCount; inner++)
		{
			PhysicsObject * object1 = m_actors[outer];
			PhysicsObject * object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPointer = collisionFunctionArray[functionIdx];
			if (collisionFunctionArray != nullptr)
			{
				collisionFunctionPointer(object1, object2);
			}

		}
	}
}

bool PhysicsScene::plane2Sphere(PhysicsObject * ObjectA, PhysicsObject * ObjectB)
{
	Plane * plane = dynamic_cast<Plane*>(ObjectA);
	Sphere * sphere = dynamic_cast<Sphere*>(ObjectB);

	if (plane != nullptr && sphere != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPos(), plane->getNormal()) - plane->getDistance();

		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			plane->resolveCollision(sphere);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::plane2Box(PhysicsObject * ObjectA, PhysicsObject * ObjectB)
{
	Plane * plane = dynamic_cast<Plane*>(ObjectA);
	Box * box = dynamic_cast<Box*>(ObjectB);

	if (box != nullptr && plane != nullptr)
	{
		glm::vec2 E = (box->getMax() - box->getMin()) * 0.5f;
		glm::vec2 N = plane->getNormal();

		float fRadius = glm::abs(N.x * E.x) + glm::abs(N.y * E.y);

		float sphereToPlane = glm::dot(box->getPos(), plane->getNormal()) - plane->getDistance();

		if (sphereToPlane < 0)
		{
			N *= -1;
			sphereToPlane *= -1;
		}

		float intersection = fRadius - sphereToPlane;
		if (intersection > 0)
		{
			plane->resolveCollision(box);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject * ObjectA, PhysicsObject * ObjectB)
{
	Sphere * sphere = dynamic_cast<Sphere*>(ObjectA);
	Plane * plane = dynamic_cast<Plane*>(ObjectB);

	if (plane != nullptr && sphere != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPos(), plane->getNormal()) - plane->getDistance();

		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			plane->resolveCollision(sphere);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject * ObjectA, PhysicsObject * ObjectB)
{
	Sphere * sphere1 = dynamic_cast<Sphere*>(ObjectA);
	Sphere * sphere2 = dynamic_cast<Sphere*>(ObjectB);

	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		if ((glm::distance(sphere2->getPos(), sphere1->getPos())) < (sphere2->getRadius() + sphere1->getRadius()))
		{
			sphere1->resolveCollision(sphere2);

			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject * ObjectA, PhysicsObject * ObjectB)
{
	Box * box = dynamic_cast<Box*>(ObjectB);
	Sphere * sphere = dynamic_cast<Sphere*>(ObjectA);

	if (box != nullptr && sphere != nullptr)
	{
		glm::vec2 center = box->getPos() - sphere->getPos();
		glm::vec2 normal = glm::normalize(center);
		
		glm::vec2 point = sphere->getPos() + (normal * sphere->getRadius());

		if (point.x < box->getMax().x && point.x > box->getMin().x && point.y < box->getMax().y && point.y > box->getMin().y)
		{
			sphere->resolveCollision(box);

			return true;
		}
	}

	return false;
}

bool PhysicsScene::box2Plane(PhysicsObject * ObjectA, PhysicsObject * ObjectB)
{
	Box * box = dynamic_cast<Box*>(ObjectA);
	Plane * plane = dynamic_cast<Plane*>(ObjectB);

	if (box != nullptr && plane != nullptr)
	{
		glm::vec2 E = (box->getMax() - box->getMin()) * 0.5f;
		glm::vec2 N = plane->getNormal();

		float fRadius = glm::abs(N.x * E.x) + glm::abs(N.y * E.y);

		float sphereToPlane = glm::dot(box->getPos(), plane->getNormal()) - plane->getDistance();

		if (sphereToPlane < 0)
		{
			N *= -1;
			sphereToPlane *= -1;
		}

		float intersection = fRadius - sphereToPlane;
		if (intersection > 0)
		{
			plane->resolveCollision(box);

			return true;
		}
	}

	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject * ObjectA, PhysicsObject * ObjectB)
{
	Box * box = dynamic_cast<Box*>(ObjectA);
	Sphere * sphere = dynamic_cast<Sphere*>(ObjectB);

	if (box != nullptr && sphere != nullptr)
	{
		glm::vec2 center = box->getPos() - sphere->getPos();
		glm::vec2 normal = glm::normalize(center);

		glm::vec2 point = sphere->getPos() + (normal * sphere->getRadius());

		if (point.x < box->getMax().x && point.x > box->getMin().x && point.y < box->getMax().y && point.y > box->getMin().y)
		{
			sphere->resolveCollision(box);

			return true;
		}
	}

	return false;
}

bool PhysicsScene::box2Box(PhysicsObject * ObjectA, PhysicsObject * ObjectB)
{
	Box * box1 = dynamic_cast<Box*>(ObjectA);
	Box * box2 = dynamic_cast<Box*>(ObjectB);

	if (box1 != nullptr && box2 != nullptr)
	{
		if (box1->getMin().x < box2->getMax().x 
			&& box1->getMax().x > box2->getMin().x
			&& box1->getMin().y < box2->getMax().y
			&& box1->getMax().y >  box2->getMin().y)
		{
			box1->resolveCollision(box2);

			return true;
		}
	}

	return false;
}
