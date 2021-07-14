#pragma once
#include "mlib.h"

namespace mlib
{
	struct Vector2
	{
		float x, y;

		Vector2(float x = 0, float y = 0);

		float dot(Vector2& v);
		static float dot(Vector2& a, Vector2& b);
		
		void rotate(float angle);
		Vector2 rotated(float angle);

		float magnitude();
		float magnitudeSquared();

		void normalize();
		Vector2 normalised();

		Vector2 perpendicular();

		static float getAngle(Vector2& a, Vector2& b);
		float getAngle(Vector2& v);

		Vector2 lerp(Vector2& a, Vector2& b, float t);

		Vector2 operator+ (Vector2 const& v);
		Vector2 operator- (Vector2 const& v);
		Vector2 operator* (Vector2 const& v);
		Vector2 operator* (float f);
		Vector2 operator/ (float f);
		Vector2 operator/ (Vector2 const& v);

		operator RLVector2();
		
		operator b2Vec2();
	};

}
