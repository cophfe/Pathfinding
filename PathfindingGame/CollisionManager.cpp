#include "CollisionManager.h"

CollisionManager::CollisionManager() : world(b2World(b2Vec2(0,-10.0f)))
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

#ifdef DEBUG
	world.SetDebugDraw();
#endif // DEBUG


}

void CollisionManager::update()
{
	world.Step(timeStep, velocityIterations, positionIterations);
	b2Vec2 position = body->GetPosition();
	float angle = body->GetAngle();
	std::cout << position.x << ' ' << position.y << ' ' << angle << '\n';
}

void CollisionManager::debugDraw()
{
//	
//	world.DebugDraw();
}


