#pragma once
#include "NecessaryHeaders.h"
#include "PhysicsDebugDraw.h"
#include "ContactListener.h"
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
	
	//update world
	void update();

	//draw debug info
	void debugDraw();

	inline b2World* getWorld() { return &world; }

private:
	//holds all the physics information
	b2World world;
#ifdef DRAW_DEBUG
	PhysicsDebugDraw* debugDrawer;
#endif // DRAW_DEBUG
	ContactListener* contactListener;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
};

