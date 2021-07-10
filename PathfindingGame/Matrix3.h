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

		//const mlib::Matrix3 identity = Matrix3();
		//const mlib::Matrix3 zero = Matrix3(0,0,0,0,0,0,0,0,0);

		Matrix3(float m00 = 1, float m10 = 0, float m20 = 0,
			float m01 = 0, float m11 = 1, float m21 = 0,
			float m02 = 0, float m12 = 0, float m22 = 1);

		Matrix3 inverse();

		Vector2 getScale();

		float getScaleX();

		Vector2 getTranslation();

		float getZRotation();

		void getAllTransformations(Vector2* position, Vector2* scale, float* rotation);

		void getAllTransformations(Vector2* position, float* scale, float* rotation);

		Vector2 getUpVector();

		Vector2 getRightVector();
		
		//static matrix generators
		static Matrix3 getPositionMatrix(Vector2& position);
		static Matrix3 getRotationMatrix2D(float angle);
		static Matrix3 getScaleMatrix(float scale);

		Matrix3 operator* (Matrix3 const& m);
		Matrix3 operator+ (float f);

		Matrix3 operator+ (Matrix3 const& m);
		Matrix3 operator- (Matrix3 const& m);
	
	};

	Vector3 operator* (const Matrix3& m, const Vector3& vector);

	Vector2 operator*(const Matrix3& m, const Vector2& vector);
}
