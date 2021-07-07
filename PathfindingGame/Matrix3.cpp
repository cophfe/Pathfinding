#include "Matrix3.h"

namespace mlib
{

	Matrix3::Matrix3(float m00, float m10, float m20,
		float m01, float m11, float m21,
		float m02, float m12, float m22)
	{
		mm[0][0] = m00;
		mm[0][1] = m01;
		mm[0][2] = m02;
		mm[1][0] = m10;
		mm[1][1] = m11;
		mm[1][2] = m12;
		mm[2][0] = m20;
		mm[2][1] = m21;
		mm[2][2] = m22;
	}

	Matrix3 Matrix3::inverse()
	{
		float d0 = mm[1][1] * mm[2][2] - mm[2][1] * mm[1][2];
		float d3 = mm[1][0] * mm[2][2] - mm[1][2] * mm[2][0];
		float d6 = mm[1][0] * mm[2][1] - mm[1][1] * mm[2][0];
		float det = mm[0][0] * d0 - mm[0][1] * d3 + mm[0][2] * d6;

		Matrix3 i = Matrix3();
		if (det == 0)
			return i;

		float iDet = 1 / det;
		i.mm[0][0] = d0 * iDet;
		i.mm[1][0] = -d3 * iDet;
		i.mm[2][0] = d6 * iDet;
		i.mm[0][1] = -(mm[0][1] * mm[2][2] - mm[0][2] * mm[2][1]) * iDet;
		i.mm[1][1] = (mm[0][0] * mm[2][2] - mm[0][2] * mm[2][0]) * iDet;
		i.mm[2][1] = -(mm[0][0] * mm[2][1] - mm[0][1] * mm[2][0]) * iDet;
		i.mm[0][2] = (mm[0][1] * mm[1][2] - mm[0][2] * mm[1][1]) * iDet;
		i.mm[1][2] = -(mm[0][0] * mm[1][2] - mm[0][2] * mm[1][0]) * iDet;
		i.mm[2][2] = (mm[0][0] * mm[1][1] - mm[0][0] * mm[1][0]) * iDet;

		return i;
	}

	Matrix3 Matrix3::getRotateZ(float angle)
	{
		float sin = sinf(angle);
		float cos = cosf(angle);
		return Matrix3(cos, -sin, 0,
			sin, cos, 0,
			0, 0, 1);
	}

	M_Vector2 Matrix3::getScale()
	{
		return M_Vector2(sqrtf(mm[0][0] * mm[0][0] + mm[1][0] * mm[1][0]), sqrtf(mm[0][1] * mm[0][1] + mm[1][1] * mm[1][1]));
	}

	float Matrix3::getScaleX()
	{
		return sqrtf(mm[0][0] * mm[0][0] + mm[1][0] * mm[1][0]);
	}

	M_Vector2 Matrix3::getTranslation()
	{
		return M_Vector2(mm[0][2], mm[1][2]);
	}

	float Matrix3::getZRotation()
	{
		float scaleX = sqrtf(mm[0][0] * mm[0][0] + mm[1][0] * mm[1][0]);
		return atan2(mm[1][0] / scaleX, mm[0][0] / scaleX);
	}

	void Matrix3::getAllTransformations(M_Vector2* position, M_Vector2* scale, float* rotation)
	{
		*position = M_Vector2(mm[0][2], mm[1][2]);
		*scale = M_Vector2(sqrtf(mm[0][0] * mm[0][0] + mm[1][0] * mm[1][0]), sqrtf(mm[0][1] * mm[0][1] + mm[1][1] * mm[1][1]));
		*rotation = atan2(mm[1][0] / scale->x, mm[0][0] / scale->x);
	}

	void Matrix3::getAllTransformations(M_Vector2* position, float* scale, float* rotation)
	{
		*position = M_Vector2(mm[0][2], mm[1][2]);
		*scale = sqrtf(mm[0][0] * mm[0][0] + mm[1][0] * mm[1][0]);
		*rotation = atan2(mm[1][0] / *scale, mm[0][0] / *scale);
	}

	M_Vector2 Matrix3::getUpVector()
	{
		return M_Vector2(mm[0][1], mm[1][1]);
	}

	M_Vector2 Matrix3::getRightVector()
	{
		return M_Vector2(mm[0][0], mm[1][0]);
	}

	Matrix3 Matrix3::operator* (Matrix3 const& m)
	{
		return Matrix3((mm[0][0] * m.mm[0][0]) + (mm[0][1] * m.mm[1][0]) + (mm[0][2] * m.mm[2][0]),    //m11
			(mm[1][0] * m.mm[0][0]) + (mm[1][1] * m.mm[1][0]) + (mm[1][2] * m.mm[2][0]),    //m21
			(mm[2][0] * m.mm[0][0]) + (mm[2][1] * m.mm[1][0]) + (mm[2][2] * m.mm[2][0]),    //m31
			(mm[0][0] * m.mm[0][1]) + (mm[0][1] * m.mm[1][1]) + (mm[0][2] * m.mm[2][1]),    //m12 
			(mm[1][0] * m.mm[0][1]) + (mm[1][1] * m.mm[1][1]) + (mm[1][2] * m.mm[2][1]),    //m22
			(mm[2][0] * m.mm[0][1]) + (mm[2][1] * m.mm[1][1]) + (mm[2][2] * m.mm[2][1]),    //m32 
			(mm[0][0] * m.mm[0][2]) + (mm[0][1] * m.mm[1][2]) + (mm[0][2] * m.mm[2][2]),    //m13
			(mm[1][0] * m.mm[0][2]) + (mm[1][1] * m.mm[1][2]) + (mm[1][2] * m.mm[2][2]),    //m23
			(mm[2][0] * m.mm[0][2]) + (mm[2][1] * m.mm[1][2]) + (mm[2][2] * m.mm[2][2]));   //m33
	}

	Matrix3 Matrix3::operator+ (float f)
	{
		return Matrix3(mm[0][0] + f, mm[1][0] + f, mm[2][0] + f,
			mm[0][1] + f, mm[1][1] + f, mm[2][1] + f,
			mm[0][2] + f, mm[1][2] + f, mm[2][2] + f);
	}

	Matrix3 Matrix3::operator+ (Matrix3 const& m)
	{
		return Matrix3(mm[0][0] + mm[0][0], mm[1][0] + mm[1][0], mm[2][0] + mm[2][0],
			mm[0][1] + mm[0][1], mm[1][1] + mm[1][1], mm[2][1] + mm[2][1],
			mm[0][2] + mm[0][2], mm[1][2] + mm[1][2], mm[2][2] + mm[2][2]);
	}

	Matrix3 Matrix3::operator- (Matrix3 const& m)
	{
		return Matrix3(mm[0][0] - mm[0][0], mm[1][0] - mm[1][0], mm[2][0] - mm[2][0],
			mm[0][1] - mm[0][1], mm[1][1] - mm[1][1], mm[2][1] - mm[2][1],
			mm[0][2] - mm[0][2], mm[1][2] - mm[1][2], mm[2][2] - mm[2][2]);
	}

	mlib::Vector3 operator* (const mlib::Matrix3& m, const mlib::Vector3& vector)
	{
		return mlib::Vector3(m.mm[0][0] * vector.x + m.mm[0][1] * vector.y + m.mm[0][2] * vector.z,
			m.mm[1][0] * vector.x + m.mm[1][1] * vector.y + m.mm[1][2] * vector.z,
			m.mm[2][0] * vector.x + m.mm[2][1] * vector.y + m.mm[2][2] * vector.z);
	}

	mlib::M_Vector2 operator*(const mlib::Matrix3& m, const mlib::M_Vector2& vector)
	{
		return mlib::M_Vector2(m.mm[0][0] * vector.x + m.mm[0][1] * vector.y,
			m.mm[1][0] * vector.x + m.mm[1][1] * vector.y);
	}
}