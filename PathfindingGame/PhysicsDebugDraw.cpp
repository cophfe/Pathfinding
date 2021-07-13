#include "PhysicsDebugDraw.h"

PhysicsDebugDraw::PhysicsDebugDraw()
{
	lengthScale = 10.0f;
}

PhysicsDebugDraw::~PhysicsDebugDraw()
{
}

void PhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	for (int i = 0; i < vertexCount; i++)
	{

	}
}

void PhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
}

void PhysicsDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	DrawCircleLines(center.x, center.y, radius, {color.r, color.g, color.b, color.a});
}

void PhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	
}

void PhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
}

void PhysicsDebugDraw::DrawTransform(const b2Transform& xf)
{
}

void PhysicsDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
}
