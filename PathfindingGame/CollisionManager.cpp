#include "CollisionManager.h"
#include "Game.h"

CollisionManager::CollisionManager() : world(b2World(b2Vec2(0,0)))
{
	//		Test with this:
	/*
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	b2BodyDef bodyDef;
	bodyDef.linearDamping = 0.4f;
	bodyDef.angularDamping= 0.4f;
	bodyDef.Type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 20.0f;
	
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);
	*/
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
}

void CollisionManager::update()
{	
	world.Step(PHYSICS_TIME_STEP, velocityIterations, positionIterations);
}

void CollisionManager::debugDraw()
{
	world.DebugDraw();
}


