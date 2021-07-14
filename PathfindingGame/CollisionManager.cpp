#include "CollisionManager.h"
#include "Game.h"

CollisionManager::CollisionManager() : world(b2World(b2Vec2(0,0)))
{
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body* groundBody = world.CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	body = world.CreateBody(&bodyDef);
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);
	
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
	collisionTimer += Game::getDeltaTime();

	while (collisionTimer >= timeStep)
	{
		world.Step(timeStep, velocityIterations, positionIterations);
		collisionTimer = 0;

		printf("X: %4.2f Y: %4.2f Rot: %4.2f\n", body->GetPosition().x, body->GetPosition().y, body->GetAngle());
	}
}

void CollisionManager::debugDraw()
{
	world.DebugDraw();
}


