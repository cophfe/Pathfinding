#pragma once

#include "Vector3.h"
#include "Vector2.h"
#include <cmath>
#include <memory>

namespace mlib
{
	struct Matrix3
	{
		union
		{
			float m[9];
			float mm[3][3];
			Vector3 vec[3];
		};

		Matrix3(float m00 = 1, float m10 = 0, float m20 = 0,
			float m01 = 0, float m11 = 1, float m21 = 0,
			float m02 = 0, float m12 = 0, float m22 = 1);

		//return the inverse matrix
		Matrix3 inverse();

		//get values from matrix (assumed translation matrix)
		Vector2 getScale();	
		float getScaleX();
		Vector2 getTranslation();
		float getZRotation();
		void getAllTransformations(Vector2* position, Vector2* scale, float* rotation);
		void getAllTransformations(Vector2* position, float* scale, float* rotation);

		//get axis vectors
		Vector2 getUpVector();
		Vector2 getRightVector();
		
		//static translation matrix generators
		static Matrix3 getPositionMatrix(Vector2& position);
		static Matrix3 getRotationMatrix2D(float angle);
		static Matrix3 getScaleMatrix(float scale);

		// math operators
		Matrix3 operator* (Matrix3 const& m);
		Matrix3 operator+ (float f);
		Matrix3 operator+ (Matrix3 const& m);
		Matrix3 operator- (Matrix3 const& m);
	
	};

	// more math operators
	Vector3 operator* (const Matrix3& m, const Vector3& vector);
	Vector2 operator*(const Matrix3& m, const Vector2& vector);
}
