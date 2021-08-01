#pragma once
#include "mlib.h"

namespace mlib
{
	//simple integer version of vector2 class
	struct Vector2i
	{
		int x, y;
	};

	//simple Vector2 class
	struct Vector2
	{
		float x, y;

		Vector2(float x = 0, float y = 0);

		// functions for getting dot value 
		float dot(const Vector2& v);
		static float dot(const Vector2& a, const Vector2& b);
		
		//rotate this vector
		void rotate(float angle);
		//return this vector if it was rotated
		Vector2 rotated(float angle);

		//get the magnitude of this vector
		float magnitude() const;
		//get squared magnitude (for performance)
		float magnitudeSquared() const;

		//normalise vector
		void normalize();
		//get normalised vector
		Vector2 normalised() const;

		//get perpendicular vector
		Vector2 perpendicular() const;

		//get angle between 2 vectors
		static float getAngle(Vector2& a, Vector2& b);
		float getAngle(Vector2 v);

		//linearly interpolate between 2 vectors
		static Vector2 lerp(Vector2& a, Vector2& b, float t);

		//mathmatical operators for vectors
		Vector2 operator+ (const Vector2& v) const;
		Vector2 operator- (const Vector2& v) const;
		Vector2 operator* (const Vector2& v) const;
		Vector2 operator* (float f) const;
		Vector2 operator/ (float f) const;
		Vector2 operator/ (const Vector2& v) const;

		//convert between Vector2s from third party libraries
		static Vector2 fromRL(const RLVector2& vec);
		static Vector2 fromB2(const b2Vec2& vec);
		operator RLVector2() const;
		operator b2Vec2() const;
	};

}
