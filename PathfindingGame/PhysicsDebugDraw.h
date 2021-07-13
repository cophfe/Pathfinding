#pragma once
#include "NecessaryHeaders.h"

#define LENGTH_SCALE 10
#define THICKNESS 3

class PhysicsDebugDraw : public b2Draw
{
public:
	PhysicsDebugDraw();

	~PhysicsDebugDraw();

	/*enum
	{
		e_shapeBit = 0x0001,	///< draw shapes
		e_jointBit = 0x0002,	///< draw joint connections
		e_aabbBit = 0x0004,	///< draw axis aligned bounding boxes
		e_pairBit = 0x0008,	///< draw broad-phase pairs
		e_centerOfMassBit = 0x0010	///< draw center of mass frame
	};*/

	/// Draw a closed polygon provided in CCW order.
	virtual void b2DebugDrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a solid closed polygon provided in CCW order.
	virtual void b2DebugDrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a circle.
	virtual void b2DebugDrawCircle(const b2Vec2& center, float radius, const b2Color& color);

	/// Draw a solid circle.
	virtual void b2DebugDrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);

	/// Draw a line segment.
	virtual void b2DebugDrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void b2DebugDrawTransform(const b2Transform& xf);

	/// Draw a point.
	virtual void b2DebugDrawPoint(const b2Vec2& p, float size, const b2Color& color);

private:

	Color convertColorToRaylib(const b2Color& color);
};

