#include "PhysicsDebugDraw.h"

PhysicsDebugDraw::PhysicsDebugDraw()
{
	SetFlags(e_shapeBit);
}

PhysicsDebugDraw::~PhysicsDebugDraw()
{
}

void PhysicsDebugDraw::b2DebugDrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	Color raylibColor = convertColorToRaylib(color);
	b2Vec2 vertex = PHYSICS_UNIT_SCALE * vertices[0];
	b2Vec2 vertex2 = PHYSICS_UNIT_SCALE * vertices[vertexCount - 1];
	DrawLineEx({ vertex.x, vertex.y }, { vertex2.x, -vertex2.y }, THICKNESS, raylibColor);

	for (int i = 0; i < vertexCount - 1; i++)
	{
		auto vertex = PHYSICS_UNIT_SCALE * vertices[i];
		auto vertex2 = PHYSICS_UNIT_SCALE * vertices[i + 1];
		DrawLineEx({ vertex.x, vertex.y }, { vertex2.x, -vertex2.y }, THICKNESS, raylibColor);
	}
}

void PhysicsDebugDraw::b2DebugDrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	Color raylibColor = convertColorToRaylib(color);
	b2Vec2 vertex = PHYSICS_UNIT_SCALE * vertices[0];
	b2Vec2 vertex2 = PHYSICS_UNIT_SCALE * vertices[vertexCount - 1];
	DrawLineEx({ vertex.x, -vertex.y }, { vertex2.x, -vertex2.y }, THICKNESS, raylibColor);

	for (int i = 0; i < vertexCount - 1; i++)
	{
		auto vertex = PHYSICS_UNIT_SCALE * vertices[i];
		auto vertex2 = PHYSICS_UNIT_SCALE * vertices[i + 1];
		DrawLineEx({ vertex.x, -vertex.y }, { vertex2.x, -vertex2.y }, THICKNESS, raylibColor);
	}

	
	//DrawTriangleFanEx((RLVector2*)vertices, vertexCount, raylibColor);
}

void PhysicsDebugDraw::b2DebugDrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	DrawCircleLines(center.x * PHYSICS_UNIT_SCALE, center.y * -PHYSICS_UNIT_SCALE, radius * PHYSICS_UNIT_SCALE, convertColorToRaylib(color));
}

void PhysicsDebugDraw::b2DebugDrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	DrawCircle(center.x * PHYSICS_UNIT_SCALE, center.y * -PHYSICS_UNIT_SCALE, radius * PHYSICS_UNIT_SCALE, convertColorToRaylib(color));
}

void PhysicsDebugDraw::b2DebugDrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	b2Vec2 vertex = PHYSICS_UNIT_SCALE * p1;
	b2Vec2 vertex2 = PHYSICS_UNIT_SCALE * p2;
	DrawLineEx({ vertex.x, -vertex.y }, { vertex2.x, -vertex2.y }, THICKNESS, convertColorToRaylib(color));
}

void PhysicsDebugDraw::b2DebugDrawTransform(const b2Transform& xf)
{
}

void PhysicsDebugDraw::b2DebugDrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	DrawCircle(p.x * PHYSICS_UNIT_SCALE, p.y * -PHYSICS_UNIT_SCALE, size * PHYSICS_UNIT_SCALE, convertColorToRaylib(color));

}

Color PhysicsDebugDraw::convertColorToRaylib(const b2Color& color)
{
	return Color{ (unsigned char)(color.r * 255),(unsigned char)(color.g * 255) ,(unsigned char)(color.b * 255) ,(unsigned char)(color.a * 255) };
}
