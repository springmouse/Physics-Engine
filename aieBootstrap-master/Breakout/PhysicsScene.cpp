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

	int actorCount = (int)m_actors.size();

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
			// contact force 
			//sphere->setPosition(sphere->getPos() + plane->getNormal() * (sphere->getRadius() - sphereToPlane));

			glm::vec2 contact = sphere->getPos() + (collisionNormal * -sphere->getRadius());
			plane->resolveCollision(sphere, contact);
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
		/*glm::vec2 E = (box->getMax() - box->getMin()) * 0.5f;
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
			glm::vec2 contact = box->getPos() + (N * -fRadius);
			plane->resolveCollision(box, contact);
			return true;
		}*/

		int numContacts = 0;
		glm::vec2 contatct(0, 0);
		float contactV = 0;
		float radius = 0.5f * std::fminf(box->getWidth(), box->getHeight());
		float penetration = 0;

		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();
		float comFromPlane = glm::dot(box->getPos() - planeOrigin, plane->getNormal());

		for (float x = -box->getWidth(); x <= box->getWidth(); x += box->getWidth())
		{
			for (float y = -box->getHeight(); y <= box->getHeight(); y += box->getHeight())
			{
				glm::vec2 p = box->getPos() + x * box->getLocalX() + y * box->getLocalY();

				float distanceFromPlane = glm::dot(p - planeOrigin, plane->getNormal());

				float velocityInToPlane = glm::dot(box->getVelocity() + box->getRotation() * (-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());

				if ((distanceFromPlane > 0 && comFromPlane < 0 && velocityInToPlane > 0) ||
					(distanceFromPlane < 0 && comFromPlane > 0 && velocityInToPlane < 0))
				{
					numContacts++;
					contatct += p;
					contactV += velocityInToPlane;

					if (comFromPlane >= 0) 
					{ 
						if (penetration > distanceFromPlane)
							penetration = distanceFromPlane;
					}
					else 
					{ 
						if (penetration < distanceFromPlane)
						{
							penetration = distanceFromPlane;
						}
					}
				}
			}
		}

		if (numContacts > 0)
		{
			float collisionV = contactV / (float)numContacts;

			glm::vec2 acceleration = -plane->getNormal() * ((1.0f + box->getElasticity()) * collisionV);

			glm::vec2 localContact = (contatct / (float)numContacts) - box->getPos();

			float r = glm::dot(localContact, glm::vec2(plane->getNormal().y, -plane->getNormal().x));

			float mass0 = 1.0f / (1.0 / box->getMass() + (r * r) / box->getMoment());

			box->applyForce(acceleration * mass0, localContact);

			//box->setPosition(box->getPos() - plane->getNormal() * penetration);
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
			// contact force 
			//sphere->setPosition(sphere->getPos() + plane->getNormal() * (sphere->getRadius() - sphereToPlane));

			glm::vec2 contact = sphere->getPos() + (collisionNormal * -sphere->getRadius());
			plane->resolveCollision(sphere, contact);
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
		glm::vec2 delta = sphere2->getPos() - sphere1->getPos();
		float distance = glm::length(delta); 
		float intersection = sphere1->getRadius() + sphere2->getRadius() - distance; 
		
		if (intersection > 0) 
		{
			glm::vec2 contactForce = 0.5f * (distance - (sphere1->getRadius() + sphere2->getRadius()))*delta / distance;
			sphere1->setPosition(sphere1->getPos() + contactForce);
			sphere2->setPosition(sphere2->getPos() - contactForce);

			// respond to the collision 
			sphere1->resolveCollision(sphere2, 0.5f * (sphere1->getPos() + sphere2->getPos()));

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
		/*glm::vec2 center = box->getPos() - sphere->getPos();
		glm::vec2 normal = glm::normalize(center);
		
		glm::vec2 point = sphere->getPos() + (normal * sphere->getRadius());

		if (point.x < box->getMax().x && point.x > box->getMin().x && point.y < box->getMax().y && point.y > box->getMin().y)
		{
			//sphere->resolveCollision(box);

			return true;
		} */

		glm::vec2 circlePos = sphere->getPos() - box->getPos();
		float w2 = box->getWidth() / 2, h2 = box->getHeight() / 2;
		int numContacts = 0; glm::vec2 contact(0, 0);

		// contact is in our box coordinates 
		// check the four corners to see if any of them are inside the circle 
		for (float x = -w2; x <= w2; x += box->getWidth())
		{
			for (float y = -h2; y <= h2; y += box->getHeight())
			{
				glm::vec2 p = x * box->getLocalX() + y * box->getLocalY();
				glm::vec2 dp = p - circlePos;
				if (dp.x*dp.x + dp.y*dp.y < sphere->getRadius()*sphere->getRadius())
				{
					numContacts++; contact += glm::vec2(x, y);
				}

				glm::vec2* direction = nullptr;
				// get the local position of the circle centre 
				glm::vec2 localPos(glm::dot(box->getLocalX(), circlePos), glm::dot(box->getLocalY(), circlePos));
				if (localPos.y < h2 && localPos.y > -h2)
				{
					if (localPos.x > 0 && localPos.x < w2 + sphere->getRadius())
					{
						numContacts++; contact += glm::vec2(w2, localPos.y);
						direction = new glm::vec2(box->getLocalX());
					}
					if (localPos.x < 0 && localPos.x > -(w2 + sphere->getRadius()))
					{
						numContacts++; contact += glm::vec2(-w2, localPos.y);
						direction = new glm::vec2(-box->getLocalX());
					}
				}

				if (localPos.x < w2 && localPos.x > -w2)
				{
					if (localPos.y > 0 && localPos.y < h2 + sphere->getRadius())
					{
						numContacts++; contact += glm::vec2(localPos.x, h2);
						direction = new glm::vec2(box->getLocalY());
					}
					if (localPos.y < 0 && localPos.y > -(h2 + sphere->getRadius()))
					{
						numContacts++; contact += glm::vec2(localPos.x, -h2); direction = new glm::vec2(-box->getLocalY());
					}
				}

				if (numContacts > 0)
				{
					// average, and convert back into world coords 
					contact = box->getPos() + (1.0f / numContacts) * (box->getLocalX() *contact.x + box->getLocalY()*contact.y);
					box->resolveCollision(sphere, contact, direction);
				}

				delete direction;
			}
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
		/*glm::vec2 E = (box->getMax() - box->getMin()) * 0.5f;
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
			glm::vec2 contact = box->getPos() + (N * -fRadius);
			plane->resolveCollision(box, contact);

			return true;
		}*/

		int numContacts = 0;
		glm::vec2 contatct(0, 0);
		float contactV = 0;
		float radius = 0.5f * std::fminf(box->getWidth(), box->getHeight());
		float penetration = 0;

		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();
		float comFromPlane = glm::dot(box->getPos() - planeOrigin, plane->getNormal());

		for (float x = -box->getWidth(); x <= box->getWidth(); x += box->getWidth())
		{
			for (float y = -box->getHeight(); y <= box->getHeight(); y += box->getHeight())
			{
				glm::vec2 p = box->getPos() + x * box->getLocalX() + y * box->getLocalY();

				float distanceFromPlane = glm::dot(p - planeOrigin, plane->getNormal());

				float velocityInToPlane = glm::dot(box->getVelocity() + box->getRotation() * (-y * box->getLocalX() + x * box->getLocalY()), plane->getNormal());

				if ((distanceFromPlane > 0 && comFromPlane < 0 && velocityInToPlane > 0) ||
					(distanceFromPlane < 0 && comFromPlane > 0 && velocityInToPlane < 0))
				{
					numContacts++;
					contatct += p;
					contactV += velocityInToPlane;

					if (comFromPlane >= 0)
					{
						if (penetration > distanceFromPlane)
							penetration = distanceFromPlane;
					}
					else
					{
						if (penetration < distanceFromPlane)
						{
							penetration = distanceFromPlane;
						}
					}
				}
			}
		}

		if (numContacts > 0)
		{
			float collisionV = contactV / (float)numContacts;

			glm::vec2 acceleration = -plane->getNormal() * ((1.0f + box->getElasticity()) * collisionV);

			glm::vec2 localContact = (contatct / (float)numContacts) - box->getPos();

			float r = glm::dot(localContact, glm::vec2(plane->getNormal().y, -plane->getNormal().x));

			float mass0 = 1.0f / (1.0 / box->getMass() + (r * r) / box->getMoment());

			box->applyForce(acceleration * mass0, localContact);

			//box->setPosition(box->getPos() - plane->getNormal() * penetration);
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
		/* glm::vec2 center = box->getPos() - sphere->getPos();
		glm::vec2 normal = glm::normalize(center);

		glm::vec2 point = sphere->getPos() + (normal * sphere->getRadius());

		if (point.x < box->getMax().x && point.x > box->getMin().x && point.y < box->getMax().y && point.y > box->getMin().y)
		{
			//sphere->resolveCollision(box);

			return true;
		} */
		glm::vec2 circlePos = sphere->getPos() - box->getPos();
		float w2 = box->getWidth() / 2, h2 = box->getHeight() / 2; 
		int numContacts = 0; glm::vec2 contact(0, 0); 
		
		// contact is in our box coordinates 
		// check the four corners to see if any of them are inside the circle 
		for (float x = -w2; x <= w2; x += box->getWidth())
		{
			for (float y = -h2; y <= h2; y += box->getHeight())
			{
				glm::vec2 p = x * box->getLocalX() + y * box->getLocalY();
				glm::vec2 dp = p - circlePos;
				if (dp.x*dp.x + dp.y*dp.y < sphere->getRadius()*sphere->getRadius())
				{
					numContacts++; contact += glm::vec2(x, y);
				}

				glm::vec2* direction = nullptr; 
				// get the local position of the circle centre 
				glm::vec2 localPos( glm::dot(box->getLocalX(), circlePos), glm::dot(box->getLocalY(), circlePos)); 
				if (localPos.y < h2 && localPos.y > -h2) 
				{ 
					if (localPos.x > 0 && localPos.x < w2 + sphere->getRadius()) 
					{ 
						numContacts++; contact += glm::vec2(w2, localPos.y); 
						direction = new glm::vec2(box->getLocalX()); 
					} 
					if (localPos.x < 0 && localPos.x > -(w2 + sphere->getRadius())) 
					{
						numContacts++; contact += glm::vec2(-w2, localPos.y); 
						direction = new glm::vec2(-box->getLocalX()); 
					} 
				} 
				
				if (localPos.x < w2 && localPos.x > -w2) 
				{ 
					if (localPos.y > 0 && localPos.y < h2 + sphere->getRadius()) 
					{ 
						numContacts++; contact += glm::vec2(localPos.x, h2); 
						direction = new glm::vec2(box->getLocalY()); 
					} 
					if (localPos.y < 0 && localPos.y > -(h2 + sphere->getRadius())) 
					{ 
						numContacts++; contact += glm::vec2(localPos.x, -h2); direction = new glm::vec2(-box->getLocalY()); 
					} 
				}

				if (numContacts > 0) 
				{ 
					// average, and convert back into world coords 
					contact = box->getPos() + (1.0f / numContacts) * (box->getLocalX() *contact.x + box->getLocalY()*contact.y); 
					box->resolveCollision(sphere, contact, direction); 
				} 
				
				delete direction;
			}
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
		/*if (box1->getMin().x < box2->getMax().x 
			&& box1->getMax().x > box2->getMin().x
			&& box1->getMin().y < box2->getMax().y
			&& box1->getMax().y >  box2->getMin().y)
		{
			//box1->resolveCollision(box2);

			return true;
		} */

		glm::vec2 boxPos = box2->getPos() - box1->getPos();

		glm::vec2 normal; 
		glm::vec2 contactForce1, contactForce2;
		glm::vec2 contact(0, 0); 
		int numContacts = 0; 
		
		box1->checkBoxCorners(*box2, contact, numContacts, normal, contactForce1);
		
		if (box2->checkBoxCorners(*box1, contact, numContacts, normal, contactForce2))
		{ 
			normal = -normal; 
		} 
		if (numContacts > 0) 
		{ 
			glm::vec2 contactForce = 0.5f*(contactForce1 - contactForce2); box1->setPosition(box1->getPos() - contactForce); 
			//box2->setPosition(box2->getPos() + contactForce); 
			box1->resolveCollision(box2, contact / float(numContacts), &normal); 
			return true; 
		}

		 return false;
	}

	return false;
}
