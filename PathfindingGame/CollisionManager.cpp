#include "CollisionManager.h"
#include "Game.h"

CollisionManager::CollisionManager() : world(b2World(b2Vec2(0,0)))
{
	contactListener = new ContactListener;
	world.SetContactListener(contactListener);
#ifdef DRAW_DEBUG
	debugDrawer = new PhysicsDebugDraw();
	world.SetDebugDraw(debugDrawer);
#endif // DRAW_DEBUG

}

CollisionManager::~CollisionManager()
{
#ifdef DRAW_DEBUG
	delete debugDrawer;
#endif // DRAW_DEBUG
	delete contactListener;
}

void CollisionManager::update()
{	
	world.Step(PHYSICS_TIME_STEP, velocityIterations, positionIterations);
}

void CollisionManager::debugDraw()
{
	world.DebugDraw();
}


