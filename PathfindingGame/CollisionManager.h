#pragma once
#include "NecessaryHeaders.h"
#include "PhysicsDebugDraw.h"
#include <iostream>

class Game;

class CollisionManager
{
public:
	CollisionManager();
	//		Copy and Move constructors and assigners (deleted)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//copy
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;
	//move
	CollisionManager(CollisionManager&&) = delete;
	CollisionManager& operator=(CollisionManager&&) = delete;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	~CollisionManager();
	
	void update();

	void debugDraw();

	inline b2World* getWorld() { return &world; }

private:
	b2World world;
	b2Body* body;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
#ifdef DRAW_DEBUG
	PhysicsDebugDraw* debugDrawer;
#endif // DRAW_DEBUG
};

