#pragma once
#include "NecessaryHeaders.h"

#define THICKNESS 5

// This class is used for drawing debug info from box2d in raylib
//header copied straight from b2Draw
class PhysicsDebugDraw : public b2Draw
{
public:
	PhysicsDebugDraw();

	~PhysicsDebugDraw();

	/// draw a closed polygon provided in CCW order.
	virtual void b2DebugDrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// draw a solid closed polygon provided in CCW order.
	virtual void b2DebugDrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// draw a circle.
	virtual void b2DebugDrawCircle(const b2Vec2& center, float radius, const b2Color& color);

	/// draw a solid circle.
	virtual void b2DebugDrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);

	/// draw a line segment.
	virtual void b2DebugDrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	/// draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void b2DebugDrawTransform(const b2Transform& xf);

	/// draw a point.
	virtual void b2DebugDrawPoint(const b2Vec2& p, float size, const b2Color& color);

private:
	Color convertColorToRaylib(const b2Color& color);
};

