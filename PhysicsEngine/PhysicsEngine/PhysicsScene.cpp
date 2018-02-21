#include "PhysicsScene.h"
#include "Circle.h"


typedef bool(*fn)(PhsicsObject*, PhsicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::CircleToCircle,  PhysicsScene::CircleToComplex,
	PhysicsScene::ComplexToCircle, PhysicsScene::ComplexToComplex,
};

PhysicsScene::PhysicsScene()
{
	m_acumulatedTime = 0.0f;
}


PhysicsScene::~PhysicsScene()
{
}

void PhysicsScene::Update(float dt)
{
	m_acumulatedTime += dt;

	if (m_acumulatedTime > m_timeStep)
	{
		m_acumulatedTime -= m_timeStep;

		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}

		CheckForCollision();
	}

}

void PhysicsScene::UpdateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->MakeGizmo();
	}
}

void PhysicsScene::UpdateRender(aie::Renderer2D * renderer)
{
	for (auto pActor : m_actors)
	{
		pActor->Renderer(renderer);
	}
}

void PhysicsScene::CheckForCollision()
{
	int actorCount = (int)m_actors.size();

	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhsicsObject * object1 = m_actors[outer];
			PhsicsObject * object2 = m_actors[inner];
			int shapeId1 = object1->GetShapeID();
			int shapeId2 = object2->GetShapeID();

			if (shapeId1 < 0 || shapeId2 < 0)
			{
				continue;
			}

			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPointer = collisionFunctionArray[functionIdx];
			if (collisionFunctionArray != nullptr)
			{
				collisionFunctionPointer(object1, object2);
			}

		}
	}
}

bool PhysicsScene::CircleToCircle(PhsicsObject * ObjectA, PhsicsObject * ObjectB)
{
	Circle * circle1 = dynamic_cast<Circle*>(ObjectA);
	Circle * circle2 = dynamic_cast<Circle*>(ObjectB);

	if (circle1 != nullptr && circle2 != nullptr)
	{
		glm::vec2 delta = circle2->GetPos() - circle1->GetPos();
		float distance = glm::length(delta);
		float intersection = circle1->getRadius() + circle2->getRadius() - distance;

		if (intersection > 0)
		{
			glm::vec2 contactForce = 0.5f * (distance - (circle1->getRadius() + circle2->getRadius()))*delta / distance;

			circle1->ResolveCollision(circle2, 0.5f * (circle1->GetPos() + circle2->GetPos()));
		}
	}

	return false;
}
