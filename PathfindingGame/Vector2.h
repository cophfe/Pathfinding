#pragma once
#include "mlib.h"

namespace mlib
{
	struct M_Vector2
	{
		float x, y;

		M_Vector2(float x, float y);
		float dot(M_Vector2* v);
		static float dot(M_Vector2* a, M_Vector2* b);
		void rotate(float angle);

		M_Vector2 rotated(float angle);

		float magnitude();

		float magnitudeSquared();

		void normalize();

		M_Vector2 normalised();

		M_Vector2 perpendicular();

		static float getAngle(M_Vector2& a, M_Vector2& b);

		float getAngle(M_Vector2& v);

		M_Vector2 lerp(M_Vector2& a, M_Vector2& b, float t);

		M_Vector2 operator+ (M_Vector2 const& v);
		M_Vector2 operator- (M_Vector2 const& v);
		M_Vector2 operator* (M_Vector2 const& v);
		M_Vector2 operator* (float f);
		M_Vector2 operator/ (float f);
		M_Vector2 operator/ (M_Vector2 const& v);

	};

}
