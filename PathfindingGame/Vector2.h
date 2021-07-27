#pragma once
#include "mlib.h"

namespace mlib
{
	struct Vector2i
	{
		int x, y;
	};

	struct Vector2
	{
		float x, y;

		Vector2(float x = 0, float y = 0);

		float dot(const Vector2& v);
		static float dot(const Vector2& a, const Vector2& b);
		
		void rotate(float angle);
		Vector2 rotated(float angle);

		float magnitude() const;
		float magnitudeSquared() const;

		void normalize();
		Vector2 normalised() const;

		Vector2 perpendicular() const;

		static float getAngle(Vector2& a, Vector2& b);
		float getAngle(Vector2 v);

		static Vector2 lerp(Vector2& a, Vector2& b, float t);

		Vector2 operator+ (const Vector2& v) const;
		Vector2 operator- (const Vector2& v) const;
		Vector2 operator* (const Vector2& v) const;
		Vector2 operator* (float f) const;
		Vector2 operator/ (float f) const;
		Vector2 operator/ (const Vector2& v) const;

		operator RLVector2() const;
		
		operator b2Vec2() const;
	};

}
